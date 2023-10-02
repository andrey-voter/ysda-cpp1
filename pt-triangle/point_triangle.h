#pragma once

#include <stdexcept>

struct Point {
    int x, y;
};

struct Triangle {
    Point a, b, c;
};

double TriangleArea(Point a, Point b, Point c) {
    return 0.5 * std::abs(a.x * (static_cast<double>(b.y) - c.y) +
                          b.x * (static_cast<double>(c.y) - a.y) +
                          c.x * (static_cast<double>(a.y) - b.y));
}

bool IsPointInTriangle(const Triangle& t, const Point& pt) {
    double total_area = TriangleArea(t.a, t.b, t.c);
    if (std::abs(total_area) < 1e-30) {
        if ((std::abs(t.a.x - pt.x) < 1e-30 && std::abs(t.b.x - pt.x) < 1e-30 &&
             std::abs(t.c.x - pt.x) < 1e-30) ||
            (std::abs(t.a.y - pt.y) < 1e-30 && std::abs(t.b.y - pt.y) < 1e-30 &&
             std::abs(t.c.y - pt.y) < 1e-30)) {
            return true;
        }
        return false;
    }
    double s1 = TriangleArea(t.a, t.b, pt);
    double s2 = TriangleArea(t.a, t.c, pt);
    double s3 = TriangleArea(t.c, t.b, pt);
    return std::abs(total_area - s1 - s2 - s3) < 1e-30;
}
