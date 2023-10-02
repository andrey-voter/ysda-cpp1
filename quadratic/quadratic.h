#pragma once

#include <stdexcept>
#include <math.h>

enum class RootCount { kZero, kOne, kTwo, kInf };

struct Roots {
    RootCount count;
    double first;
    double second;
};

double GetDiscriminant(int a, int b, int c) {
    return b * b - 4 * a * c;
}

std::pair<double, double> GetRoots(double discriminant, int a, int b) {
    double x1 = (-b + sqrt(discriminant)) / (2 * a);
    double x2 = (-b - sqrt(discriminant)) / (2 * a);
    if (x1 > x2) {
        std::swap(x1, x2);
    }
    return std::pair{x1, x2};
}

Roots SolveQuadratic(int a, int b, int c) {
    if (a == 0) {
        if (b == 0) {
            if (c == 0) {
                return Roots{RootCount::kInf, 0, 0};
            } else {
                return Roots{RootCount::kZero, 0, 0};
            }
        } else {
            double root = -c / static_cast<double>(b);
            return Roots{RootCount::kOne, root, 0};
        }
    } else {
        double discriminant = GetDiscriminant(a, b, c);
        if (discriminant < 0) {
            return Roots{RootCount::kZero, 0, 0};
        } else {
            if (discriminant > 0) {
                auto roots = GetRoots(discriminant, a, b);
                return Roots{RootCount::kTwo, roots.first, roots.second};
            } else {
                double root = -b / static_cast<double>(2 * a);
                return Roots{RootCount::kOne, root, 0};
            }
        }
    }
}
