#pragma once

#include <stdexcept>
#include <iostream>

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
        int min_x = std::min(std::min(t.a.x, t.b.x), t.c.x);
        int max_x = std::max(std::max(t.a.x, t.b.x), t.c.x);

        int max_x_y, min_x_y;
        if (min_x == t.a.x) {
            min_x_y = t.a.y;
        } else {
            if (min_x == t.b.x) {
                min_x_y = t.b.y;
            } else {
                min_x_y = t.c.y;
            }
        }
        if (max_x == t.a.x) {
            max_x_y = t.a.y;
        } else {
            if (max_x == t.b.x) {
                max_x_y = t.b.y;
            } else {
                max_x_y = t.c.y;
            }
        }

        if (max_x == min_x) {
            if (max_x_y == min_x_y) {
                return ((pt.x == max_x) && (pt.y == max_x_y));
            } else {
                return ((pt.x == max_x) && ((min_x_y <= pt.y) && (pt.y <= max_x_y)));
            }
        } else {
            if (max_x_y == min_x_y) {
                return ((pt.y == max_x_y) && ((min_x <= pt.x) && (pt.x <= max_x)));
            } else {
                double a =
                    (static_cast<double>(max_x_y) - min_x_y) / (static_cast<double>(max_x) - min_x);
                double b = static_cast<double>(min_x_y) - a * static_cast<double>(min_x);
                if ((std::abs(a * static_cast<double>(pt.x) + b - static_cast<double>(pt.y)) <
                     1e-30) &&
                    ((min_x <= pt.x) && (pt.x <= max_x))) {
                    return true;
                }
            }
        }
        return false;
    }
    double s1 = TriangleArea(t.a, t.b, pt);
    double s2 = TriangleArea(t.a, t.c, pt);
    double s3 = TriangleArea(t.c, t.b, pt);
    return std::abs(total_area - s1 - s2 - s3) < 1e-30;
}
