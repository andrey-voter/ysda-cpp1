#pragma once

#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>
#include <vector.h>
#include <image.h>

#include <options/camera_options.h>
#include <options/render_options.h>
#include <ray.h>

class MyPixel {
public:
    MyPixel() = default;
    MyPixel(const Vector& color) : color_(color){};
    void operator=(const Vector& pixel);
    void ToneMapping(RenderMode mode, double coef);
    void GammaCorrection();
    RGB ToRGB() const;
    double MaxValue() const;

private:
    Vector color_;
};

void MyPixel::operator=(const Vector& color) {
    color_ = color;
}

void MyPixel::ToneMapping(RenderMode mode, double coef) {
    if (coef == 0) {
        coef = 1.0;
    }

    if (mode == RenderMode::kDepth) {
        if (color_.IsZero()) {
            color_ = Vector(1.0, 1.0, 1.0);
        } else {
            color_ = color_ / coef;
        }
        return;
    }
    if (mode == RenderMode::kNormal) {
        if (!color_.IsZero()) {
            color_ = 0.5 * color_ + 0.5;
        }
    }
    if (mode == RenderMode::kFull) {
        color_ = (color_ * (1 + color_ / (coef * coef))) / (1 + color_);
    }
}

void MyPixel::GammaCorrection() {
    color_ = Pow(color_, 1.0 / 2.2);
}

RGB MyPixel::ToRGB() const {
    return {static_cast<int>(color_[0] * 255), static_cast<int>(color_[1] * 255),
            static_cast<int>(color_[2] * 255)};
}

double MyPixel::MaxValue() const {
    double max_01 = std::max(color_[0], color_[1]);
    return std::max(max_01, color_[2]);
}

class Screen {
public:
    Screen(size_t height, size_t width) : pixels_(height, std::vector<MyPixel>(width)){};

    size_t GetHeight() const;
    size_t GetWidth() const;
    void SetPixel(const MyPixel& pixel, size_t y, size_t x);
    MyPixel GetPixel(size_t y, size_t x) const;
    void ToneMapping(RenderMode mode);
    void GammaCorrection();
    Image ToImage() const;

private:
    std::vector<std::vector<MyPixel>> pixels_;
    double max_pixel_value_ = 0;
};

size_t Screen::GetHeight() const {
    return pixels_.size();
}

size_t Screen::GetWidth() const {
    return pixels_[0].size();
}

void Screen::SetPixel(const MyPixel& pixel, size_t y, size_t x) {
    pixels_[y][x] = pixel;
    if (pixel.MaxValue() > max_pixel_value_) {
        max_pixel_value_ = pixel.MaxValue();
    }
}

MyPixel Screen::GetPixel(size_t y, size_t x) const {
    return pixels_[y][x];
}

void Screen::ToneMapping(RenderMode mode) {
    for (size_t y = 0; y < GetHeight(); ++y) {
        for (size_t x = 0; x < GetWidth(); ++x) {
            pixels_[y][x].ToneMapping(mode, max_pixel_value_);
        }
    }
}

void Screen::GammaCorrection() {
    for (size_t y = 0; y < GetHeight(); ++y) {
        for (size_t x = 0; x < GetWidth(); ++x) {
            pixels_[y][x].GammaCorrection();
        }
    }
}

Image Screen::ToImage() const {
    Image image(GetWidth(), GetHeight());
    for (size_t y = 0; y < GetHeight(); ++y) {
        for (size_t x = 0; x < GetWidth(); ++x) {
            image.SetPixel(GetPixel(y, x).ToRGB(), y, x);
        }
    }

    return image;
}

class MyCam {
public:
    explicit MyCam(const CameraOptions& camera_options);

    size_t GetHeight() const;
    size_t GetWidth() const;

    void SetPixel(const MyPixel& pixel, int y, int x);
    Ray GenerateRay(int y, int x) const;
    void ToneMapping(RenderMode mode);
    void GammaCorrection();
    Image GetImage() const;

private:
    Screen screen_;
    Vector position_;
    Vector right_;
    Vector up_;
    Vector forward_;
    double fov_;
};

MyCam::MyCam(const CameraOptions& camera_options)
    : screen_(camera_options.screen_height, camera_options.screen_width),
      position_(camera_options.look_from),
      fov_(camera_options.fov) {

    Vector up(0.0, 1.0, 0.0);

    forward_ = camera_options.look_from - camera_options.look_to;
    forward_.Normalize();
    right_ = CrossProduct(up, forward_);
    if (right_.IsZero()) {
        if (forward_[1] < 0) {
            right_ = {-1, 0, 0};
            up_ = {0, 0, 1};
        } else {
            right_ = {1, 0, 0};
        }
    } else {
        right_.Normalize();
    }
    if (up_.IsZero()) {
        up_ = CrossProduct(forward_, right_);
    }
}

size_t MyCam::GetWidth() const {
    return screen_.GetWidth();
}

size_t MyCam::GetHeight() const {
    return screen_.GetHeight();
}

void MyCam::SetPixel(const MyPixel& pixel, int y, int x) {
    screen_.SetPixel(pixel, y, x);
}

Ray MyCam::GenerateRay(int y, int x) const {
    int width = GetWidth();
    int height = GetHeight();
    double aspect_ratio = static_cast<double>(width) / height;

    double camera_x =
        (2 * ((x + 0.5) / static_cast<double>(width)) - 1) * tan(fov_ * 0.5) * aspect_ratio;
    double camera_y = (1 - 2 * ((y + 0.5) / static_cast<double>(height))) * tan(fov_ * 0.5);

    Vector homo_coords(camera_x, camera_y, -1);

    Vector rotation_0(right_[0], up_[0], forward_[0]);
    Vector rotation_1(right_[1], up_[1], forward_[1]);
    Vector rotation_2(right_[2], up_[2], forward_[2]);

    Vector direction(DotProduct(rotation_0, homo_coords), DotProduct(rotation_1, homo_coords),
                     DotProduct(rotation_2, homo_coords));
    Vector origin = position_;
    return Ray(origin, direction);
}

void MyCam::ToneMapping(RenderMode mode) {
    screen_.ToneMapping(mode);
}

void MyCam::GammaCorrection() {
    screen_.GammaCorrection();
}

Image MyCam::GetImage() const {
    return screen_.ToImage();
}