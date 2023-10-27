#pragma once

#include <array>
#include <cstddef>
#include <cmath>
#include <vector>
#include <iostream>

class Vector {
public:
    Vector() : data_({0, 0, 0}) {
    }
    Vector(double x, double y, double z) : data_({x, y, z}) {
    }
    Vector(std::array<double, 3> vector) : data_(vector) {
    }
    Vector(std::initializer_list<double> initializer_list) {
        std::copy(initializer_list.begin(), initializer_list.end(), data_.begin());
    }

    double& operator[](size_t ind) {
        return data_[ind];
    }
    double operator[](size_t ind) const {
        return data_[ind];
    }

    void Normalize() {
        double len = GetLength();
        data_[0] /= len;
        data_[1] /= len;
        data_[2] /= len;
    }

    double GetLength() const {
        return std::sqrt(data_[0] * data_[0] + data_[1] * data_[1] + data_[2] * data_[2]);
    }

    std::array<double, 3> GetData() const {
        return data_;
    }

    bool IsZero() const {
        if (data_[0] == 0.0 && data_[1] == 0.0 && data_[2] == 0.0) {
            return true;
        }
        return false;
    }

    Vector operator-(const Vector& rhs) const {
        Vector cpy(this->data_);
        cpy -= rhs;
        return cpy;
    }

    Vector operator-=(const Vector& rhs) {
        data_[0] -= rhs.data_[0];
        data_[1] -= rhs.data_[1];
        data_[2] -= rhs.data_[2];
        return *this;
    }

    Vector operator+(const Vector& rhs) const {
        Vector cpy(this->data_);
        cpy += rhs;
        return cpy;
    }

    Vector operator+=(const Vector& rhs) {
        data_[0] += rhs.data_[0];
        data_[1] += rhs.data_[1];
        data_[2] += rhs.data_[2];
        return *this;
    }

    Vector operator*(const double num) const {
        Vector cpy = Vector(this->data_);
        cpy *= num;
        return cpy;
    }

    Vector operator*=(const double num) {
        data_[0] *= num;
        data_[1] *= num;
        data_[2] *= num;
        return *this;
    }

    Vector operator/(const double num) const {
        Vector cpy = Vector(this->data_);
        cpy /= num;
        return cpy;
    }

    Vector operator/=(const double num) {
        data_[0] /= num;
        data_[1] /= num;
        data_[2] /= num;
        return *this;
    }

    Vector operator-() {
        return Vector{-data_[0], -data_[1], -data_[2]};
    }

    Vector operator-() const {
        Vector result = *this;
        result[0] = -result[0];
        result[1] = -result[1];
        result[2] = -result[2];
        return result;
    }

private:
    std::array<double, 3> data_;
};

Vector operator/(Vector lhs, const Vector& rhs) {
    lhs[0] /= rhs[0];
    lhs[1] /= rhs[1];
    lhs[2] /= rhs[2];
    return lhs;
}

Vector operator*(const Vector& lhs, const Vector& rhs) {
    return Vector{lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2]};
}

Vector operator*(double number, const Vector& vector) {
    Vector result = vector;
    result *= number;
    return result;
}

Vector operator+(Vector lhs, double rhs) {
    lhs[0] += rhs;
    lhs[1] += rhs;
    lhs[2] += rhs;
    return lhs;
}

Vector operator+(double lhs, Vector rhs) {
    rhs[0] += lhs;
    rhs[1] += lhs;
    rhs[2] += lhs;
    return rhs;
}

double DotProduct(const Vector& a, const Vector& b) {
    double prod = 0;
    for (int i = 0; i < 3; ++i) {
        prod += a[i] * b[i];
    }
    return prod;
}

Vector CrossProduct(const Vector& a, const Vector& b) {
    Vector cross_prod;
    cross_prod[0] = a[1] * b[2] - a[2] * b[1];
    cross_prod[1] = a[2] * b[0] - a[0] * b[2];
    cross_prod[2] = a[0] * b[1] - a[1] * b[0];
    return cross_prod;
}

double Length(const Vector& v) {
    return v.GetLength();
}

Vector Pow(Vector lhs, double degree) {
    lhs[0] = pow(lhs[0], degree);
    lhs[1] = pow(lhs[1], degree);
    lhs[2] = pow(lhs[2], degree);

    return lhs;
}

Vector Normalize(Vector vector) {
    vector.Normalize();
    return vector;
}
