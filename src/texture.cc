#include "texture.h"

color::Color CheckerTexture::Value(const double u, const double v, const Point3& point) const {
    const auto x_int = static_cast<int>(std::floor(this->inv_scale_ * point.x()));
    const auto y_int = static_cast<int>(std::floor(this->inv_scale_ * point.y()));
    const auto z_int = static_cast<int>(std::floor(this->inv_scale_ * point.z()));

    const bool is_even = (x_int + y_int + z_int) % 2 == 0;

    return is_even ? this->even_->Value(u, v, point) : this->odd_->Value(u, v, point);
}