#pragma once

#include <triangle.h>
#include <material.h>
#include <sphere.h>
#include <vector.h>
#include <geometry.h>

struct Object {
    const Material* material = nullptr;
    Triangle polygon;

    const Vector* GetNormal(size_t index) const {
        return normals[index];
    }

    std::array<Vector*, 3> normals = {nullptr, nullptr, nullptr};

    bool CorrectNormals() {
        return normals[0] != nullptr && normals[1] != nullptr && normals[2] != nullptr;
    }

    Vector GetWeightedNormals(const Vector& point) const {
        Vector coords = GetBarycentricCoords(polygon, point);
        return *normals[0] * coords[0] + *normals[1] * coords[1] + *normals[2] * coords[2];
    }
};

struct SphereObject {
    const Material* material = nullptr;
    Sphere sphere;
};
