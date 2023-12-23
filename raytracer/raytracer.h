#pragma once

#include <image.h>
#include <options/camera_options.h>
#include <options/render_options.h>

#include <filesystem>

// Image Render(const std::filesystem::path& path, const CameraOptions& camera_options,
//              const RenderOptions& render_options) {
//     throw std::runtime_error{"Not implemented"};
// }

#include <string>
#include <vector>

#include <vector.h>
#include <illumination.h>
// #include <image.h>
// #include "options/camera_options.h"
// #include "options/render_options.h"
#include <raycaster.h>

void ToneMappingAndGammaCorrection(std::vector<std::vector<Vector>>& pixels) {
    double coefficient = 0;
    for (size_t i = 0; i < pixels.size(); ++i) {
        for (size_t j = 0; j < pixels[0].size(); ++j) {
            for (int color_index = 0; color_index < 3; ++color_index) {
                coefficient = std::max(coefficient, pixels[i][j][color_index]);
            }
        }
    }

    for (size_t i = 0; i < pixels.size(); ++i) {
        for (size_t j = 0; j < pixels[0].size(); ++j) {
            for (int color_index = 0; color_index < 3; ++color_index) {
                pixels[i][j][color_index] =
                    pixels[i][j][color_index] *
                    (1 + pixels[i][j][color_index] / (coefficient * coefficient)) /
                    (1 + pixels[i][j][color_index]);
                pixels[i][j][color_index] = std::pow(pixels[i][j][color_index], 1 / 2.2);
            }
        }
    }
}

class Raytracer {
public:
    Raytracer(const std::string& filename, const CameraOptions& camera_options,
              const RenderOptions& render_options)
        : scene_(ReadScene(filename)),
          render_options_(render_options),
          ray_caster_(camera_options) {
    }

public:
    Image Render() {
        switch (render_options_.mode) {
            case RenderMode::kDepth:
                return RenderDepth();
            case RenderMode::kNormal:
                return RenderNormal();
            case RenderMode::kFull:
                return RenderFull();
            default:
                throw std::runtime_error("Bad render mode");
        }
    }

private:
    Image RenderDepth() {
        Image image(ray_caster_.screen_width_, ray_caster_.screen_height_);
        //                image.PrepareImage(image.Width(), image.Height());

        std::vector<std::vector<double>> depths(image.Width(), std::vector<double>(image.Width()));
        double max_depth = 0;

        for (int i = 0; i < image.Width(); ++i) {
            for (int j = 0; j < image.Height(); ++j) {
                auto cast_ray = ray_caster_(i, j);
                auto possible_intersection =
                    FindClosestIntersectionAndMaterial(scene_, cast_ray).first;

                if (possible_intersection) {
                    depths[i][j] = possible_intersection.value().GetDistance();
                    max_depth = std::max(depths[i][j], max_depth);
                } else {
                    depths[i][j] = 0;
                }
            }
        }
        // Normalize
        for (int i = 0; i < image.Width(); ++i) {
            for (int j = 0; j < image.Height(); ++j) {
                if (depths[i][j] == 0) {
                    depths[i][j] = 1;
                } else {
                    depths[i][j] /= max_depth;
                }
            }
        }

        // Build pixels
        for (int i = 0; i < image.Width(); ++i) {
            for (int j = 0; j < image.Height(); ++j) {
                int brightness = static_cast<int>((depths[i][j] - kEpsilon) * 256);
                image.SetPixel({brightness, brightness, brightness}, j, i);
            }
        }

        return image;
    }

    Image RenderNormal() {
        Image image(ray_caster_.screen_width_, ray_caster_.screen_height_);
        //                image.PrepareImage(image.Width(), image.Height());

        std::vector<std::vector<Vector>> normals(image.Width(), std::vector<Vector>(image.Width()));

        for (int i = 0; i < image.Width(); ++i) {
            for (int j = 0; j < image.Height(); ++j) {
                auto cast_ray = ray_caster_(i, j);
                // Check all possible intersections
                auto possible_intersection =
                    FindClosestIntersectionAndMaterial(scene_, cast_ray).first;

                if (possible_intersection) {
                    // Save pixel value
                    normals[i][j] = possible_intersection.value().GetNormal();
                } else {
                    normals[i][j] = {-1, -1, -1};
                }
            }
        }
        // Normalize
        for (int i = 0; i < image.Width(); ++i) {
            for (int j = 0; j < image.Height(); ++j) {
                normals[i][j] /= 2;
                normals[i][j] += Vector{0.5, 0.5, 0.5};
            }
        }

        // Build pixels
        for (int i = 0; i < image.Width(); ++i) {
            for (int j = 0; j < image.Height(); ++j) {
                auto normal = normals[i][j];

                int red = static_cast<int>((normal[0] - kEpsilon) * 256);
                int green = static_cast<int>((normal[1] - kEpsilon) * 256);
                int blue = static_cast<int>((normal[2] - kEpsilon) * 256);
                image.SetPixel({red, green, blue}, j, i);
            }
        }

        return image;
    }

    Image RenderFull() {
        Image image(ray_caster_.screen_width_, ray_caster_.screen_height_);
        //                image.PrepareImage(image.Width(), image.Height());

        std::vector<std::vector<Vector>> pseudo_pixels(image.Width(),
                                                       std::vector<Vector>(image.Width()));

        for (int i = 0; i < image.Width(); ++i) {
            for (int j = 0; j < image.Height(); ++j) {
                auto cast_ray = ray_caster_(i, j);
                pseudo_pixels[i][j] =
                    CalculateIllumination(scene_, cast_ray, false, render_options_.depth);
            }
        }
        // Normalize
        ToneMappingAndGammaCorrection(pseudo_pixels);
        // Build pixels
        for (int i = 0; i < image.Width(); ++i) {
            for (int j = 0; j < image.Height(); ++j) {
                auto pseudo_pixel = pseudo_pixels[i][j];

                int red = static_cast<int>((pseudo_pixel[0] - kEpsilon) * 256);
                int green = static_cast<int>((pseudo_pixel[1] - kEpsilon) * 256);
                int blue = static_cast<int>((pseudo_pixel[2] - kEpsilon) * 256);
                image.SetPixel({red, green, blue}, j, i);
            }
        }

        return image;
    }

private:
    Scene scene_;
    RenderOptions render_options_;
    RayCaster ray_caster_;
};

Image Render(const std::string& filename, const CameraOptions& camera_options,
             const RenderOptions& render_options) {
    return Raytracer(filename, camera_options, render_options).Render();
}
