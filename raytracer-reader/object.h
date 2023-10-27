#pragma once

#include <triangle.h>
#include <material.h>
#include <sphere.h>
#include <vector.h>

struct Object {
    const Material* material = nullptr;
    Triangle polygon;

    const Vector* GetNormal(size_t index) const {
        return normals[index];
    }

    std::array<Vector*, 3> normals = {nullptr, nullptr, nullptr};
};

struct SphereObject {
    const Material* material = nullptr;
    Sphere sphere;
};
