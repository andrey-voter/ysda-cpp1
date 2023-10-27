#pragma once

#include <image.h>
#include <options/camera_options.h>
#include <options/render_options.h>

#include <filesystem>

#include <string>
#include <vector>

#include <geometry.h>
#include <scene.h>
#include <camera.h>
#include <vector.h>

constexpr double kEps = 1e-6;

bool LightSeen(const Vector& point, const Light& light, const Scene& scene) {
    Vector light_direction = light.position - point;
    double distance = Length(light_direction);
    light_direction.Normalize();
    Vector origin = point + kEps * light_direction;
    Ray ray(origin, light_direction);
    for (const Object& object : scene.GetObjects()) {
        std::optional<Intersection> intersection = GetIntersection(ray, object.polygon);
        if (intersection.has_value() && (intersection->GetDistance() < distance)) {
            return false;
        }
    }

    for (const SphereObject& sphere_object : scene.GetSphereObjects()) {
        std::optional<Intersection> intersection = GetIntersection(ray, sphere_object.sphere);
        if (intersection.has_value() && (intersection->GetDistance() < distance)) {
            return false;
        }
    }

    return true;
}

Vector CastRay(const Ray& ray, const Scene& scene, RenderMode mode, int recursion_depth,
               bool is_inside) {

    if (recursion_depth == -1) {
        return Vector(0, 0, 0);
    }

    std::optional<Intersection> closest_object_intersection = Intersection();
    std::optional<Object> closest_object;
    for (const Object& object : scene.GetObjects()) {
        std::optional<Intersection> intersection = GetIntersection(ray, object.polygon);
        if (intersection.has_value()) {
            if (intersection->GetDistance() < closest_object_intersection->GetDistance()) {
                closest_object_intersection = intersection;
                closest_object = object;
            }
        }
    }

    std::optional<Intersection> closest_sphere_intersection = Intersection();
    std::optional<SphereObject> closest_sphere_object;
    for (const SphereObject& sphere : scene.GetSphereObjects()) {
        std::optional<Intersection> intersection = GetIntersection(ray, sphere.sphere);
        if (intersection.has_value()) {
            if (intersection->GetDistance() < closest_sphere_intersection->GetDistance()) {
                closest_sphere_intersection = intersection;
                closest_sphere_object = sphere;
            }
        }
    }

    if (mode == RenderMode::kDepth) {
        double depth = 0;
        if (closest_object.has_value()) {
            if (closest_sphere_object.has_value()) {
                if (closest_object_intersection->GetDistance() <
                    closest_sphere_intersection->GetDistance()) {

                    depth = closest_object_intersection->GetDistance();
                } else {
                    depth = closest_sphere_intersection->GetDistance();
                }
            } else {
                depth = closest_object_intersection->GetDistance();
            }
        } else if (closest_sphere_object.has_value()) {
            depth = closest_sphere_intersection->GetDistance();
        }

        return Vector(depth, depth, depth);
    }

    if (mode == RenderMode::kNormal) {
        Vector normal;
        if (closest_object.has_value()) {
            if (closest_sphere_object.has_value()) {
                if (closest_object_intersection->GetDistance() <
                    closest_sphere_intersection->GetDistance()) {
                    if (closest_object->CorrectNormals()) {
                        normal = closest_object->GetWeightedNormals(
                            closest_object_intersection->GetPosition());
                    } else {
                        normal = closest_object_intersection->GetNormal();
                    }
                } else {
                    normal = closest_sphere_intersection->GetNormal();
                }
            } else {
                if (closest_object->CorrectNormals()) {
                    normal = closest_object->GetWeightedNormals(
                        closest_object_intersection->GetPosition());
                } else {
                    normal = closest_object_intersection->GetNormal();
                }
            }
        } else if (closest_sphere_object.has_value()) {
            normal = closest_sphere_intersection->GetNormal();
        }

        return normal;
    }

    if (mode == RenderMode::kFull) {
        std::optional<Intersection> closest_intersection;
        const Material* closest_material;
        Vector normal;
        if (closest_object.has_value()) {
            if (closest_sphere_object.has_value()) {
                if (closest_object_intersection->GetDistance() <
                    closest_sphere_intersection->GetDistance()) {

                    closest_intersection = closest_object_intersection;
                    if (closest_object->CorrectNormals()) {
                        normal =
                            closest_object->GetWeightedNormals(closest_intersection->GetPosition());
                    } else {
                        normal = closest_intersection->GetNormal();
                    }
                    closest_material = closest_object->material;

                } else {
                    closest_intersection = closest_sphere_intersection;
                    normal = closest_intersection->GetNormal();
                    closest_material = closest_sphere_object->material;
                }
            } else {
                closest_intersection = closest_object_intersection;
                if (closest_object->CorrectNormals()) {
                    normal =
                        closest_object->GetWeightedNormals(closest_intersection->GetPosition());
                } else {
                    normal = closest_intersection->GetNormal();
                }
                closest_material = closest_object->material;
            }
        } else if (closest_sphere_object.has_value()) {
            closest_intersection = closest_sphere_intersection;
            normal = closest_intersection->GetNormal();
            closest_material = closest_sphere_object->material;
        }

        if (!closest_intersection.has_value()) {
            return Vector(0, 0, 0);
        }

        Vector result = closest_material->ambient_color + closest_material->intensity;

        Vector v_light, v_eye, v_r, l_d, l_s;
        for (const Light& light : scene.GetLights()) {
            if (LightSeen(closest_intersection->GetPosition(), light, scene)) {
                v_light = Normalize(light.position - closest_intersection->GetPosition());
                l_d = light.intensity * std::max(0.0, DotProduct(normal, v_light));
                v_eye = -ray.GetDirection();
                v_r = 2 * (DotProduct(normal, v_light) * normal) - v_light;
                l_s = light.intensity * pow(std::max(0.0, DotProduct(v_eye, v_r)),
                                            closest_material->specular_exponent);
                result += closest_material->albedo[0] * (closest_material->diffuse_color * l_d +
                                                         closest_material->specular_color * l_s);
            }
        }

        if (closest_material->albedo[1] && !is_inside) {
            Vector reflected_direction = Reflect(ray.GetDirection(), normal);
            Vector origin = closest_intersection->GetPosition() + kEps * normal;
            result += closest_material->albedo[1] * CastRay(Ray(origin, reflected_direction), scene,
                                                            mode, recursion_depth - 1, is_inside);
        }

        if (closest_material->albedo[2]) {
            double eta = 1.0 / closest_material->refraction_index;
            double albedo = closest_material->albedo[2];
            if (is_inside) {
                eta = 1.0 / eta;
                albedo = 1.0;
            }
            std::optional<Vector> refracted_direction = Refract(ray.GetDirection(), normal, eta);
            if (refracted_direction.has_value()) {
                Vector origin = closest_intersection->GetPosition() - kEps * normal;
                result += albedo * CastRay(Ray(origin, *refracted_direction), scene, mode,
                                           recursion_depth - 1, !is_inside);
            }
        }

        return result;
    }

    return Vector(0, 0, 0);
}

Image Render(const std::filesystem::path& path, const CameraOptions& camera_options,
             const RenderOptions& render_options) {
    MyCam camera(camera_options);
    Scene scene = ReadScene(path);
    bool is_inside = false;

    for (size_t y = 0; y < camera.GetHeight(); ++y) {
        for (size_t x = 0; x < camera.GetWidth(); ++x) {
            Ray ray(camera.GenerateRay(y, x));
            Vector color =
                CastRay(ray, scene, render_options.mode, render_options.depth, is_inside);
            camera.SetPixel(color, y, x);
        }
    }

    camera.ToneMapping(render_options.mode);
    if (render_options.mode == RenderMode::kFull) {
        camera.GammaCorrection();
    }

    return camera.GetImage();
}