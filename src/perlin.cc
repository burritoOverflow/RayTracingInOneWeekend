#include "perlin.h"
#include <math.h>
#include <cmath>
#include "config.h"
#include "vec3.h"

double Perlin::Noise(const Point3& point) const {
    auto u = point.x() - floor(point.x());
    auto v = point.y() - floor(point.y());
    auto w = point.z() - floor(point.z());

    const auto i = int(floor(point.x()));
    const auto j = int(floor(point.y()));
    const auto k = int(floor(point.z()));

    Vector3 c[2][2][2];

    for (int di = 0; di < 2; di++)
        for (int dj = 0; dj < 2; dj++)
            for (int dk = 0; dk < 2; dk++)
                c[di][dj][dk] = this->randvec_[perm_x_[(i + di) & 255] ^ perm_y_[(j + dj) & 255] ^
                                               perm_z_[(k + dk) & 255]];

    return PerlinInterpolation(c, u, v, w);
}

double Perlin::Turbulence(const Point3& point, const double depth) const {
    double accum{0.0};
    Point3 temp_point = point;
    double weight{1.0};

    for (int i = 0; i < depth; ++i) {
        accum += weight * Noise(temp_point);
        weight *= 0.5;
        temp_point *= 2;
    }

    return fabs(accum);
}

double Perlin::TrilinearInterpolation(const double c[2][2][2], double u, double v, double w) {
    double accumulator = 0.0;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
                accumulator += (i * u + (1 - i) * (1 - u)) * (j * v + (1 - j) * (1 - v)) *
                               (k * w + (1 - k) * (1 - w)) * c[i][j][k];

    return accumulator;
}

double Perlin::PerlinInterpolation(const Vector3 c[2][2][2], const double u, const double v, const double w) {
    const auto uu = u * u * (3 - 2 * u);
    const auto vv = v * v * (3 - 2 * v);
    const auto ww = w * w * (3 - 2 * w);

    auto accum = 0.0;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++) {
                Vector3 weight_v(u - i, v - j, w - k);

                accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) *
                         (k * ww + (1 - k) * (1 - ww)) * Dot(c[i][j][k], weight_v);
            }

    return accum;
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