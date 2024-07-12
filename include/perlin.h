#ifndef PERLIN_H
#define PERLIN_H

#include <memory>
#include "config.h"
#include "vec3.h"
class Perlin {
   public:
    Perlin() {
        this->random_float_ = std::make_unique<double[]>(this->POINT_COUNT_);
        for (int i = 0; i < this->POINT_COUNT_; ++i) {
            this->random_float_[i] = config::GetRandomDouble();
        }
        this->perm_x_ = PerlinGeneratePerm();
        this->perm_y_ = PerlinGeneratePerm();
        this->perm_z_ = PerlinGeneratePerm();
    };

    double Noise(const Point3& point) const;

   private:
    static constexpr int POINT_COUNT_ = 256;
    std::unique_ptr<double[]> random_float_;
    std::unique_ptr<int[]> perm_x_;
    std::unique_ptr<int[]> perm_y_;
    std::unique_ptr<int[]> perm_z_;

    static std::unique_ptr<int[]> PerlinGeneratePerm();

    static void Permute(std::unique_ptr<int[]>& p, const int n);
};

#endif  // PERLIN_H