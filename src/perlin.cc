#include "perlin.h"

double Perlin::Noise(const Point3& point) const {
    const auto i = int(4 * point.x()) & 255;
    const auto j = int(4 * point.y()) & 255;
    const auto k = int(4 * point.z()) & 255;

    return this->random_float_[this->perm_x_[i] ^ this->perm_y_[j] ^ this->perm_z_[k]];
}

std::unique_ptr<int[]> Perlin::PerlinGeneratePerm() {
    auto p = std::make_unique<int[]>(POINT_COUNT_);
    for (int i = 0; i < POINT_COUNT_; ++i) {
        p[i] = i;
    }
    Permute(p, POINT_COUNT_);
    return p;
}

void Perlin::Permute(std::unique_ptr<int[]>& p, const int n) {
    for (int i = n - 1; i > 0; --i) {
        const int target = config::GetRandomInt(0, i);
        int temp = p[i];
        p[i] = p[target];
        p[target] = temp;
    }
}