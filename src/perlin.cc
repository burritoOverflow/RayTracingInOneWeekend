#include "perlin.h"
#include <math.h>
#include <cmath>
#include "config.h"

double Perlin::Noise(const Point3& point) const {
    auto u = point.x() - floor(point.x());
    auto v = point.y() - floor(point.y());
    auto w = point.z() - floor(point.z());

    // Hermitian smoothing - section 5.3
    u = u * u * (3 - 2 * u);
    v = v * v * (3 - 2 * v);
    w = w * w * (3 - 2 * w);

    const auto i = int(floor(point.x()));
    const auto j = int(floor(point.y()));
    const auto k = int(floor(point.z()));

    double c[2][2][2];

    for (int di = 0; di < 2; di++)
        for (int dj = 0; dj < 2; dj++)
            for (int dk = 0; dk < 2; dk++)
                c[di][dj][dk] = this->random_float_[perm_x_[(i + di) & 255] ^ perm_y_[(j + dj) & 255] ^
                                                    perm_z_[(k + dk) & 255]];
    return TrilinearInterpolation(c, u, v, w);
}

double Perlin::TrilinearInterpolation(double c[2][2][2], double u, double v, double w) {
    double accumulator = 0.0;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
                accumulator += (i * u + (1 - i) * (1 - u)) * (j * v + (1 - j) * (1 - v)) *
                               (k * w + (1 - k) * (1 - w)) * c[i][j][k];

    return accumulator;
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