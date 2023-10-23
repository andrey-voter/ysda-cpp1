#pragma once

#include <vector.h>
#include <sphere.h>
#include <intersection.h>
#include <triangle.h>
#include <ray.h>

#include <optional>
#include <parameters.h>

std::optional<Intersection> GetIntersection(const Ray& ray, const Sphere& sphere) {
    Vector center = sphere.GetCenter() - ray.GetOrigin();

    double center_ray_projection = DotProduct(center, ray.GetDirection());
    center_ray_projection /= Length(ray.GetDirection());

    Vector normalized_ray_direction = ray.GetDirection();
    Vector tc = normalized_ray_direction;
    tc.Normalize();
    tc *= center_ray_projection;

    Vector d = tc - center;

    double d_length = Length(d);

    if (d_length > sphere.GetRadius()) {
        return {};
    }

    double delta_t = std::sqrt(sphere.GetRadius() * sphere.GetRadius() - d_length * d_length);

    Vector relative_position;
    if (center_ray_projection > 0) {
        if (center_ray_projection - delta_t > 0) {
            relative_position = tc - normalized_ray_direction * delta_t;
        } else {
            relative_position = tc + normalized_ray_direction * delta_t;
        }
    } else {
        if (center_ray_projection + delta_t > 0) {
            relative_position = tc + normalized_ray_direction * delta_t;
        } else {
            return {};
        }
    }

    Vector normal;
    if (Length(center) > sphere.GetRadius()) {
        normal = (relative_position - center);
    } else {
        normal = (center - relative_position);
    }
    normal.Normalize();
    return Intersection(relative_position + ray.GetOrigin(), normal, Length(relative_position));
};

std::optional<Intersection> GetIntersection(const Ray& ray, const Triangle& triangle) {
    const double eps = 1e-16;
    Vector vertex0 = triangle[0];
    Vector vertex1 = triangle[1];
    Vector vertex2 = triangle[2];
    Vector edge1, edge2, h, s, q;
    double a, f, u, v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = CrossProduct(ray.GetDirection(), edge2);
    a = DotProduct(edge1, h);

    if (std::abs(a) < eps) {
        return {};  // The ray is parallel to the triangle.
    }

    f = 1.0 / a;
    s = ray.GetOrigin() - vertex0;
    u = f * DotProduct(s, h);
    if (u < 0.0 || u > 1.0) {
        return {};
    }
    q = CrossProduct(s, edge1);
    v = f * DotProduct(ray.GetDirection(), q);
    if (v < 0.0 || u + v > 1.0) {
        return {};
    }
    // At this stage we can compute t to find out where the intersection point is on the line.
    double t = f * DotProduct(edge2, q);
    if (t > eps)  // ray intersection
    {
        Vector intersection_point = ray.GetOrigin() + ray.GetDirection() * t;
        Vector normal = CrossProduct(edge1, edge2);
        normal.Normalize();
        if (DotProduct(normal, ray.GetDirection()) > 0) {
            normal -= normal * 2;
        }
        return Intersection{intersection_point, normal,
                            Length(intersection_point - ray.GetOrigin())};
    } else {
        return {};  // This means that there is a line intersection but not a ray intersection.
    }
}

Vector Reflect(const Vector& ray, const Vector& normal) {
    return ray - normal * DotProduct(normal, ray) * 2;
}

std::optional<Vector> Refract(const Vector& ray, const Vector& normal, double eta) {
    Vector normalized_ray = ray;
    normalized_ray.Normalize();

    double c = -DotProduct(normal, normalized_ray);
    if (eta * eta * (1 - c * c) > 1) {
        return {};
    }
    return normalized_ray * eta + normal * (eta * c - std::sqrt(1 - eta * eta * (1 - c * c)));
}
Vector GetBarycentricCoords(const Triangle& triangle, const Vector& point) {
    double s_bcx = Triangle({triangle[1], triangle[2], point}).Area() * 0.5;
    double s_cax = Triangle({triangle[2], triangle[0], point}).Area() * 0.5;
    double s_abx = Triangle({triangle[0], triangle[1], point}).Area() * 0.5;
    return Vector{triangle[0] * s_bcx + triangle[1] * s_cax + triangle[2] * s_abx};
}
