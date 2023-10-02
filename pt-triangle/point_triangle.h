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
    double s1 = TriangleArea(t.a, t.b, pt);
    double s2 = TriangleArea(t.a, t.c, pt);
    double s3 = TriangleArea(t.c, t.b, pt);
    return std::abs(TriangleArea(t.a, t.b, t.c) - s1 - s2 - s3) < 1e-30;
}
