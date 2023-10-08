#pragma once

#include <vector>
#include <stdexcept>

class Matrix {
public:
    explicit Matrix(int n_rows, int n_cols)
        : matrix_(std::vector<std::vector<double>>(n_rows, std::vector<double>(n_cols))) {
    }
    explicit Matrix(int n_row_cols)
        : matrix_(std::vector<std::vector<double>>(n_row_cols, std::vector<double>(n_row_cols))) {
    }
    explicit Matrix(std::vector<std::vector<double>> data) : matrix_(data) {
    }

    size_t Rows() const {
        return matrix_.size();
    }

    size_t Columns() const {
        return !matrix_.empty() ? matrix_[0].size() : 0;
    }

    double& operator()(size_t row, size_t col) {
        if (row < Rows() and col < Columns()) {
            return matrix_[row][col];
        }
        throw std::out_of_range("Index out of range");
    }

    double operator()(size_t row, size_t col) const {
        if (row < Rows() and col < Columns()) {
            return matrix_[row][col];
        }
        throw std::out_of_range("Index out of range");
    }

    Matrix& operator+=(const Matrix& rhs) {
        if (Rows() != rhs.Rows() or Columns() != rhs.Columns()) {
            throw std::logic_error("Inconsistent sizes");
        }
        for (size_t i = 0; i < Rows(); ++i) {
            for (size_t j = 0; j < Columns(); ++j) {
                matrix_[i][j] += rhs.matrix_[i][j];
            }
        }
        return *this;
    }

    Matrix operator+(const Matrix& rhs) const {
        Matrix cpy(this->matrix_);
        cpy += rhs;
        return cpy;
    }

    Matrix& operator-=(const Matrix& rhs) {
        if (Rows() != rhs.Rows() or Columns() != rhs.Columns()) {
            throw std::logic_error("Inconsistent sizes");
        }
        for (size_t i = 0; i < Rows(); ++i) {
            for (size_t j = 0; j < Columns(); ++j) {
                matrix_[i][j] -= rhs.matrix_[i][j];
            }
        }
        return *this;
    }

    Matrix operator-(const Matrix& rhs) const {
        Matrix cpy(this->matrix_);
        cpy -= rhs;
        return cpy;
    }

    Matrix operator*=(const Matrix& rhs) {
        if (Columns() != rhs.Rows()) {
            throw std::logic_error("Inconsistent sizes");
        }
        Matrix result(Rows(), rhs.Columns());
        for (size_t i = 0; i < Rows(); ++i) {
            for (size_t j = 0; j < rhs.Columns(); ++j) {
                double el = 0;
                for (size_t s = 0; s < Columns(); ++s) {
                    el += (*this)(i, s) * rhs(s, j);
                }
                result(i, j) = el;
            }
        }
        *this = result;
        return *this;
    }

    Matrix operator*(const Matrix& rhs) const {
        Matrix cpy(this->matrix_);
        cpy *= rhs;
        return cpy;
    }

private:
    std::vector<std::vector<double>> matrix_;
};

Matrix Transpose(const Matrix& matrix) {
    Matrix res(matrix.Columns(), matrix.Rows());
    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Columns(); ++j) {
            res(j, i) = matrix(i, j);
        }
    }
    return res;
}

Matrix Identity(int n) {
    Matrix res(n);
    for (int i = 0; i < n; ++i) {
        res(i, i) = 1;
    }
    return res;
}
