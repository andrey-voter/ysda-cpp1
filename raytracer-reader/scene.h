#pragma once

#include <material.h>
#include <vector.h>
#include <object.h>
#include <light.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

class Scene {
public:
    Scene(std::vector<Object> objects, std::vector<SphereObject> sphere_objects,
          std::vector<Light> lights, std::unordered_map<std::string, Material> materials,
          std::vector<std::unique_ptr<Vector>> normals_pointers)
        : objects_(std::move(objects)),
          sphere_objects_(std::move(sphere_objects)),
          lights_(std::move(lights)),
          materials_(std::move(materials)),
          normals_pointers_(std::move(normals_pointers)) {
    }

    const std::vector<Object>& GetObjects() const {
        return objects_;
    }
    const std::vector<SphereObject>& GetSphereObjects() const {
        return sphere_objects_;
    }
    const std::vector<Light>& GetLights() const {
        return lights_;
    }
    const std::unordered_map<std::string, Material>& GetMaterials() const {
        return materials_;
    }

private:
    std::vector<Object> objects_;
    std::vector<SphereObject> sphere_objects_;
    std::vector<Light> lights_;
    std::unordered_map<std::string, Material> materials_;

    std::vector<std::unique_ptr<Vector>> normals_pointers_;
};

std::vector<std::string> ParseLine(const std::string& line) {
    std::istringstream iss(line);
    std::vector<std::string> parsed_line;

    for (std::string s; iss >> s;) {
        parsed_line.push_back(s);
    }
    return parsed_line;
}

std::pair<int, int> ParsePointTriplet(const std::string& line) {
    std::istringstream iss(line);
    std::vector<std::string> parsed_line;
    std::string s;

    while (std::getline(iss, s, '/')) {
        if (!s.empty()) {
            parsed_line.push_back(s);
        } else {
            parsed_line.emplace_back("0");
        }
    }
    if (parsed_line.size() < 3) {
        return {std::stod(parsed_line[0]), 0};  // zero means no normal
    }

    return {std::stod(parsed_line[0]), std::stod(parsed_line[2])};
}

Vector GetThreeNumbers(const std::vector<std::string>& attributes, int begin = 1) {
    return {std::stod(attributes[begin]), std::stod(attributes[begin + 1]),
            std::stod(attributes[begin + 2])};
}

std::unordered_map<std::string, Material> ReadMaterials(const std::filesystem::path& path) {
    std::unordered_map<std::string, Material> materials;

    std::string line;
    bool inside_material = false;
    Material current_material;

    std::ifstream file(path);

    while (std::getline(file, line)) {
        std::vector<std::string> attributes = ParseLine(line);
        if (attributes.empty()) {
            continue;
        }

        if (attributes[0] == "newmtl") {
            if (inside_material) {
                materials[current_material.name] = current_material;
            }
            current_material = Material{};
            inside_material = true;
            current_material.name = attributes[1];
        }

        if (attributes[0] == "Ka") {
            current_material.ambient_color = GetThreeNumbers(attributes);
        }

        if (attributes[0] == "Kd") {
            current_material.diffuse_color = GetThreeNumbers(attributes);
        }

        if (attributes[0] == "Ks") {
            current_material.specular_color = GetThreeNumbers(attributes);
        }

        if (attributes[0] == "Ke") {
            current_material.intensity = GetThreeNumbers(attributes);
        }

        if (attributes[0] == "Ns") {
            current_material.specular_exponent = std::stod(attributes[1]);
        }

        if (attributes[0] == "Ni") {
            current_material.refraction_index = std::stod(attributes[1]);
        }

        if (attributes[0] == "al") {
            current_material.albedo = GetThreeNumbers(attributes);
        }
    }

    materials[current_material.name] = current_material;

    file.close();
    return materials;
}

Scene ReadScene(const std::filesystem::path& path) {
    std::vector<Object> objects;
    std::vector<SphereObject> sphere_objects;
    std::vector<Light> lights;
    std::unordered_map<std::string, Material> materials;
    std::vector<std::unique_ptr<Vector>> normal_pointers;
    std::vector<Vector> normal;

    std::vector<Vector> vertices;
    std::string line;
    Material* current_material = nullptr;

    std::ifstream file(path);

    while (std::getline(file, line)) {
        std::vector<std::string> attributes = ParseLine(line);
        if (attributes.empty()) {
            continue;
        }

        if (attributes[0] == "v") {
            vertices.push_back(GetThreeNumbers(attributes));
        }

        if (attributes[0] == "vt") {
            continue;
        }

        if (attributes[0] == "vn") {
            normal_pointers.push_back(std::make_unique<Vector>(GetThreeNumbers(attributes)));
            continue;
        }

        if (attributes[0] == "f") {
            size_t number_of_vertices = attributes.size() - 1;
            for (size_t i = 0; i < number_of_vertices - 2; ++i) {
                std::vector<std::pair<int, int>> indices = {ParsePointTriplet(attributes[1]),
                                                            ParsePointTriplet(attributes[i + 2]),
                                                            ParsePointTriplet(attributes[i + 3])};

                std::vector<Vector*> normals(3, nullptr);
                for (int j = 0; j < 3; ++j) {
                    if (indices[j].first < 1) {
                        indices[j].first = static_cast<int>(vertices.size()) + indices[j].first;
                    } else {
                        indices[j].first--;
                    }
                    if (indices[j].second == 0) {  // no normal
                        normals[j] = nullptr;
                    } else {
                        if (indices[j].second < 1) {
                            normals[j] =
                                (normal_pointers[normal_pointers.size() + indices[j].second].get());
                        } else {
                            normals[j] = (normal_pointers[indices[j].second - 1].get());
                        }
                    }
                }

                Object object{current_material,
                              Triangle{vertices[indices[0].first], vertices[indices[1].first],
                                       vertices[indices[2].first]},
                              {normals[0], normals[1], normals[2]}};
                objects.push_back(object);
            }
            continue;
        }

        if (attributes[0] == "mtllib") {
            materials = ReadMaterials(path.parent_path() / attributes[1]);
            continue;
        }

        if (attributes[0] == "usemtl") {
            current_material = &materials.at(attributes[1]);
            continue;
        }

        if (attributes[0] == "S") {
            sphere_objects.push_back(
                {current_material, Sphere{GetThreeNumbers(attributes), std::stod(attributes[4])}});
            continue;
        }

        if (attributes[0] == "P") {
            lights.push_back({GetThreeNumbers(attributes), GetThreeNumbers(attributes, 4)});
            continue;
        }
    }
    file.close();
    return {std::move(objects), std::move(sphere_objects), std::move(lights), std::move(materials),
            std::move(normal_pointers)};
}
