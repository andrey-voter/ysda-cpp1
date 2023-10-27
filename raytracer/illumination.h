//#ifndef SHAD_CPP0_ILLUMINATION_H
//#define SHAD_CPP0_ILLUMINATION_H
//
//#endif  // SHAD_CPP0_ILLUMINATION_H
//
//#pragma once
//
//#include <string>
//
//#include <vector.h>
//#include <intersection.h>
//#include <ray.h>
//#include <geometry.h>
//#include <material.h>
//#include <light.h>
//#include <object.h>
//#include <scene.h>
//#include <algorithm>
//
//const double kEpsilon = 0.0001;
//
//std::pair<std::optional<Intersection>, const Material*> GetIntersectionAndMaterial(
//    const Ray& ray, const Object& object) {
//    auto trivial_normal_intersection = GetIntersection(ray, object.polygon);
//
//    if (!trivial_normal_intersection) {
//        return {trivial_normal_intersection, object.material};
//    }
//
//    for (size_t i = 0; i < object.normals.size(); ++i) {
//        if (object.normals[i] == nullptr) {
//            return {trivial_normal_intersection, object.material};
//        }
//    }
//    auto true_normal = LinearCombination(
//        GetBarycentricCoords(object.polygon, trivial_normal_intersection.value().GetPosition()),
//        object.normals);
//
//    return {Intersection{trivial_normal_intersection->GetPosition(), true_normal,
//                         trivial_normal_intersection->GetDistance()},
//            object.material};
//}
//
//inline std::pair<std::optional<Intersection>, const Material*> GetIntersectionAndMaterial(
//    const Ray& ray, const SphereObject& sphere_object) {
//    return {GetIntersection(ray, sphere_object.sphere), sphere_object.material};
//}
//
//std::vector<std::pair<Intersection, const Material*>> FindAllIntersectionsAndMaterials(
//    const Scene& scene, const Ray& ray) {
//    std::vector<std::pair<Intersection, const Material*>> intersections;
//    for (const auto& object : scene.GetObjects()) {
//        auto [possible_intersection, material] = GetIntersectionAndMaterial(ray, object);
//        if (possible_intersection) {
//            intersections.emplace_back(possible_intersection.value(), material);
//        }
//    }
//
//    for (const auto& sphere_object : scene.GetSphereObjects()) {
//        auto [possible_intersection, material] = GetIntersectionAndMaterial(ray, sphere_object);
//        if (possible_intersection) {
//            intersections.emplace_back(possible_intersection.value(), material);
//        }
//    }
//    return intersections;
//}
//
//std::pair<std::optional<Intersection>, const Material*> FindClosestIntersectionAndMaterial(
//    std::vector<std::pair<Intersection, const Material*>> intersections) {
//    if (!intersections.empty()) {
//        return *std::min_element(
//            intersections.begin(), intersections.end(),
//            [](auto& lhs, auto& rhs) { return lhs.first.GetDistance() < rhs.first.GetDistance(); });
//
//    } else {
//        return {{}, nullptr};
//    }
//}
//
//std::pair<std::optional<Intersection>, const Material*> FindClosestIntersectionAndMaterial(
//    const Scene& scene, const Ray& ray) {
//    return FindClosestIntersectionAndMaterial(FindAllIntersectionsAndMaterials(scene, ray));
//}
//
//inline bool ReachThroughPossible(const Material* material) {
//    return material->refraction_index == 1 && material->albedo[2] != 0;
//}
//
//Vector LightReach(const Scene& scene, const Light& light, const Vector& position, int ttl) {
//    if (ttl < 0) {
//        return {0, 0, 0};
//    }
//    Vector ray_direction = position - light.position;
//    ray_direction.Normalize();
//    Ray ray(light.position, ray_direction);
//    auto [closest_intersection, material] = FindClosestIntersectionAndMaterial(scene, ray);
//
//    if (!closest_intersection) {
//        return {0, 0, 0};
//    }
//
//    if (Length(closest_intersection.value().GetPosition() - position) < kEpsilon) {
//        return light.intensity;
//    }
//
//    if (ReachThroughPossible(material)) {
//        Light new_light = {closest_intersection.value().GetPosition(),
//                           material->albedo[2] * material->specular_color * light.intensity};
//        new_light.position += ray_direction * kEpsilon;
//        return LightReach(scene, new_light, position, ttl - 1);
//    } else {
//        return {0, 0, 0};
//    }
//}
//
//Vector CalculateDiffuse(const Scene& scene, const Intersection& intersection, int ttl) {
//    Vector total_diffusive_illumination = {0, 0, 0};
//    for (const auto& light : scene.GetLights()) {
//        auto illumination = LightReach(scene, light, intersection.GetPosition(), ttl);
//        if (!illumination.Zero()) {
//            auto light_direction = (light.position - intersection.GetPosition());
//            light_direction.Normalize();
//            total_diffusive_illumination +=
//                illumination * std::max(0.0, DotProduct(light_direction, intersection.GetNormal()));
//        }
//    }
//    return total_diffusive_illumination;
//}
//
//Vector CalculateSpecular(const Scene& scene, const Intersection& intersection,
//                         const Material* material, const Ray& ray, int ttl) {
//    Vector total_specular_illumination{0, 0, 0};
//    for (const auto& light : scene.GetLights()) {
//        auto illumination = LightReach(scene, light, intersection.GetPosition(), ttl);
//        if (!illumination.Zero()) {
//            auto light_direction = (light.position - intersection.GetPosition());
//            light_direction.Normalize();
//            auto reflection_direction = Reflect(-light_direction, intersection.GetNormal());
//            reflection_direction.Normalize();
//            double cos_sigma = -DotProduct(reflection_direction, ray.GetDirection());
//
//            total_specular_illumination +=
//                illumination * pow(std::max(0.0, cos_sigma), material->specular_exponent);
//        }
//    }
//    return total_specular_illumination;
//}
//
//Vector CalculateIllumination(const Scene& scene, const Ray& ray, bool inside, int ttl) {
//    if (ttl < 0) {
//        return Vector{0, 0, 0};
//    }
//    auto [possible_intersection, material] = FindClosestIntersectionAndMaterial(scene, ray);
//    if (possible_intersection) {
//        auto intersection = possible_intersection.value();
//
//        // Ambient
//        auto illumination_ambient = material->ambient_color + material->intensity;
//
//        // Diffusive
//        auto illumination_diffusive = material->diffuse_color *
//                                      CalculateDiffuse(scene, intersection, ttl - 1) *
//                                      material->albedo[0];
//
//        // Specular
//        auto illumination_specular =
//            material->specular_color *
//            CalculateSpecular(scene, intersection, material, ray, ttl - 1) * material->albedo[0];
//
//        // Reflected
//        Ray reflected_ray = {intersection.GetPosition(),
//                             Reflect(ray.GetDirection(), intersection.GetNormal())};
//        reflected_ray.Propell(kEpsilon);
//        Vector illumination_reflected;
//        if (material->albedo[1] != 0 && !inside) {
//            illumination_reflected = material->specular_color *
//                                     CalculateIllumination(scene, reflected_ray, false, ttl - 1) *
//                                     material->albedo[1];
//        } else {
//            illumination_reflected = {0, 0, 0};
//        }
//
//        // Refracted
//        Vector illumination_refracted;
//        std::optional<Vector> refracted_ray_direction;
//        if (!inside) {
//            refracted_ray_direction = Refract(ray.GetDirection(), intersection.GetNormal(),
//                                              1 / material->refraction_index);
//        } else {
//            refracted_ray_direction =
//                Refract(ray.GetDirection(), intersection.GetNormal(), material->refraction_index);
//        }
//
//        if (refracted_ray_direction && (material->albedo[2] != 0)) {
//            Ray refracted_ray = {intersection.GetPosition(), refracted_ray_direction.value()};
//            refracted_ray.Propell(kEpsilon);
//
//            illumination_refracted = material->specular_color *
//                                     CalculateIllumination(scene, refracted_ray, true, ttl - 1) *
//                                     material->albedo[2];
//        } else {
////            if (refracted_ray_direction) {
////                Ray refracted_ray = {intersection.GetPosition(), refracted_ray_direction.value()};
////                refracted_ray.Propell(kEpsilon);
////                illumination_refracted =
////                    material->specular_color *
////                    CalculateIllumination(scene, refracted_ray, false, ttl - 1) *
////                    material->albedo[2];
////            } else {
//                illumination_refracted = {0, 0, 0};
////            }
//        }
//
//        if (inside && material->albedo[2] != 0) {
//
//            illumination_refracted *=
//                (material->albedo[2] + material->albedo[1]) / material->albedo[2];
//        }
//        std::cout << "----refraction-----" << std::endl;
//        illumination_refracted.Print();
//        return illumination_ambient + illumination_diffusive + illumination_specular +
//               illumination_reflected + illumination_refracted;
//    } else {
//        return Vector{0, 0, 0};
//    }
//}
//
//// enum class StrongestDirection { kFront, kBack, kTop, kBottom, kRight, kLeft };
////
//// StrongestDirection GetStrongestDirection( const Vector& direction) {
////     size_t max_abs = std::max_element(direction.GetData().begin(), direction.GetData().end(),
////                                       [](auto a, auto b) { return fabs(a) < fabs(b); }) -
////                      direction.GetData().begin();
////     switch (max_abs) {
////         case 0:
////             return (direction[0] > 0) ? StrongestDirection::kFront : StrongestDirection::kBack;
////         case 1:
////             return (direction[1] > 0) ? StrongestDirection::kTop : StrongestDirection::kBottom;
////         case 2:
////             return (direction[2] > 0) ? StrongestDirection::kRight : StrongestDirection::kLeft;
////         default:
////             //            throw std::runtime_error("Direction guessing gone wrong");
////             return StrongestDirection::kFront;
////     }
//// }
//
////
//// Vector Trace(const Ray& ray, Image * image) {
////    if (!image) {
////        std::cout << "1" << std::endl;
////        return {0, 0, 0};
////    }
////    std::cout << "2" << std::endl;
////    int block_size = image->Width() / 4;
////    auto cube_v = ray.GetDirection();
////    cube_v =
////        cube_v / std::fabs(*std::max_element(cube_v.GetData().begin(), cube_v.GetData().end(),
////                                             [](auto a, auto b) { return fabs(a) < fabs(b); }));
////    RGB color{};
////    switch (GetStrongestDirection(ray.GetDirection())) {
////        int x, y;
////        case StrongestDirection::kFront:
////            x = (3 * block_size) / 2 + static_cast<int>(block_size * cube_v[2] / 2);
////            y = (3 * block_size) / 2 + static_cast<int>(block_size * -cube_v[1] / 2);
////            color = image->GetPixel(y, x);
////            break;
////        case StrongestDirection::kBack:
////            x = (7 * block_size) / 2 + static_cast<int>(block_size * -cube_v[2] / 2);
////            y = (3 * block_size) / 2 + static_cast<int>(block_size * -cube_v[1] / 2);
////            color = image->GetPixel(y, x);
////            break;
////        case StrongestDirection::kTop:
////            x = (3 * block_size) / 2 + static_cast<int>(block_size * cube_v[2] / 2);
////            y = (1 * block_size) / 2 + static_cast<int>(block_size * cube_v[0] / 2);
////            color = image->GetPixel(y, x);
////            break;
////        case StrongestDirection::kBottom:
////            x = (3 * block_size) / 2 + static_cast<int>(block_size * cube_v[2] / 2);
////            y = (5 * block_size) / 2 + static_cast<int>(block_size * -cube_v[0] / 2);
////            color = image->GetPixel(y, x);
////            break;
////        case StrongestDirection::kRight:
////            x = (5 * block_size) / 2 + static_cast<int>(block_size * -cube_v[0] / 2);
////            y = (3 * block_size) / 2 + static_cast<int>(block_size * -cube_v[1] / 2);
////            color = image->GetPixel(y, x);
////            break;
////        case StrongestDirection::kLeft:
////            x = (1 * block_size) / 2 + static_cast<int>(block_size * cube_v[0] / 2);
////            y = (3 * block_size) / 2 + static_cast<int>(block_size * -cube_v[1] / 2);
////            color = image->GetPixel(y, x);
////            break;
////    }
////    std::cout << "3" << std::endl;
////    return {static_cast<double>(color.r) / 256, static_cast<double>(color.g) / 256,
////            static_cast<double>(color.b) / 256};
////}
