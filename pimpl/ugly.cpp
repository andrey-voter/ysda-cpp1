#include "ugly.h"
#include "bad.h"

class Spline::SplineImpl {
public:
    SplineImpl(std::vector<double> x, std::vector<double> y, double a, double b)
        : x_(std::move(x)), y_(std::move(y)), a_(a), b_(b), size_(x_.size()) {
        y2_.resize(size_);
        Initialize();
    }

    double Interpolate(double x) const {
        double ans;
        int ret = mySplintCube(x_.data(), y_.data(), y2_.data(), size_, x, &ans);
        if (ret == -1) {
            throw std::runtime_error("runtime error");
        }
        return ans;
    }

private:
    void Initialize() {
        int ret = mySplineSnd(x_.data(), y_.data(), size_, a_, b_, y2_.data());
        if (ret == -1) {
            throw std::bad_alloc();
        } else if (ret == -2) {
            throw std::invalid_argument("invalid argument");
        }
    }

    std::vector<double> x_;
    std::vector<double> y_;
    std::vector<double> y2_;
    double a_;
    double b_;
    size_t size_;
};

Spline::Spline(std::vector<double> x, std::vector<double> y, double a, double b)
    : impl_(std::make_unique<SplineImpl>(std::move(x), std::move(y), a, b)) {
}

Spline::~Spline() = default;

double Spline::Interpolate(double x) const {
    return impl_->Interpolate(x);
}
