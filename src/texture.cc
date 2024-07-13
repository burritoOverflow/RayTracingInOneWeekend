#include "texture.h"
#include "color.h"
#include "interval.h"

color::Color CheckerTexture::Value(double u, double v, const Point3& point) const {
    const auto x_int = static_cast<int>(std::floor(this->inv_scale_ * point.x()));
    const auto y_int = static_cast<int>(std::floor(this->inv_scale_ * point.y()));
    const auto z_int = static_cast<int>(std::floor(this->inv_scale_ * point.z()));

    const bool is_even = (x_int + y_int + z_int) % 2 == 0;

    return is_even ? this->even_->Value(u, v, point) : this->odd_->Value(u, v, point);
}

color::Color ImageTexture::Value(double u, double v, const Point3& point) const {
    if (this->image_.Height() <= 0) {
        return {0, 1, 1};
    }

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = Interval(0, 1).Clamp(u);
    v = 1 - Interval(0, 1).Clamp(v);  // flip v to image coordinates

    const auto i = int(u * this->image_.Width());
    const auto j = int(v * this->image_.Height());

    const auto pixel = this->image_.PixelData(i, j);
    const auto color_scale = 1.0 / 255.0;

    return {color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]};
}

color::Color NoiseTexture::Value(double u, double v, const Point3& point) const {
    return color::Color{1, 1, 1} * this->noise_.Turbulence(point, 7);
}