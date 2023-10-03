#pragma once

#include <stdexcept>
#include <iostream>

struct Point {
    int x, y;
};

struct Triangle {
    Point a, b, c;
};

int64_t DoubleTriangleArea(Point a, Point b, Point c) {
    return std::abs(a.x * (static_cast<int64_t>(b.y) - c.y) +
                    b.x * (static_cast<int64_t>(c.y) - a.y) +
                    c.x * (static_cast<int64_t>(a.y) - b.y));
}

bool IsPointInTriangle(const Triangle& t, const Point& pt) {
    if (((pt.x == t.a.x) && (pt.y == t.a.y)) || ((pt.x == t.b.x) && (pt.y == t.b.y)) ||
        ((pt.x == t.c.x) && (pt.y == t.c.y))) {
        return true;
    }
    int64_t double_total_area = DoubleTriangleArea(t.a, t.b, t.c);
    if (double_total_area == 0) {
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
                return ((pt.x == max_x) && ((min_x_y < pt.y) && (pt.y < max_x_y)));
            }
        } else {
            if (max_x_y == min_x_y) {
                return ((pt.y == max_x_y) && ((min_x < pt.x) && (pt.x < max_x)));
            } else {
                int64_t a = -(static_cast<int64_t>(max_x_y) - min_x_y);
                int64_t b = static_cast<int64_t>(max_x) - min_x;
                int64_t c = -(a * min_x + b * min_x_y);
                if ((a * pt.x + b * pt.y + c == 0) && ((min_x < pt.x) && (pt.x < max_x))) {
                    return true;
                }
            }
        }
        return false;
    }
    int64_t s1 = DoubleTriangleArea(t.a, t.b, pt);
    int64_t s2 = DoubleTriangleArea(t.a, t.c, pt);
    int64_t s3 = DoubleTriangleArea(t.c, t.b, pt);
    return ((double_total_area - s1 - s2 - s3) == 0);
}
