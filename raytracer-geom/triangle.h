#pragma once

#include <vector.h>
#include <parameters.h>

class Triangle {
public:
    Triangle(const Vector& a, const Vector& b, const Vector& c) {
        triangle_[0] = a;
        triangle_[1] = b;
        triangle_[2] = c;
    }

    Triangle(std::initializer_list<Vector> list) {
        std::copy(list.begin(), list.end(), triangle_.begin());
    }

    const Vector& operator[](size_t ind) const {
        return triangle_[ind];
    }
    double Area() const {
        return 0.5 * (Length(CrossProduct((triangle_[1] - triangle_[0]), triangle_[2] - triangle_[0])));
    }



private:
    std::array<Vector, 3> triangle_;
};
