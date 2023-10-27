#pragma once

#include <array>
#include <cstddef>
#include <cmath>
#include <vector>

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
        data_[1] +=  rhs.data_[1];
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

    Vector operator/=(const double num) {
        data_[0] /= num;
        data_[1] /=  num;
        data_[2] /= num;
        return *this;
    }

    bool Zero() const {
        return GetLength() == 0;
    }

    Vector operator-() {
        return Vector{-data_[0], -data_[1], -data_[2]};
    }

private:
    std::array<double, 3> data_;
};

Vector operator*(const Vector& lhs, const Vector& rhs) {
    return Vector{lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2]};
}

Vector operator*(double number, const Vector& vector) {
    Vector result = vector;
    result *= number;
    return result;
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

Vector LinearCombination(const Vector& coefficients, const std::vector<Vector*>& vectors) {
    Vector result = {0, 0, 0};
    for (size_t i = 0; i < vectors.size(); ++i) {
        result += coefficients[i] * (*(vectors[i]));
    }
    return result;
}

Vector LinearCombination(const Vector& coefficients, const std::array<Vector*, 3>& vectors) {
    Vector result = {0, 0, 0};
    for (size_t i = 0; i < 3; ++i) {
        result += coefficients[i] * (*(vectors[i]));
    }
    return result;
}
