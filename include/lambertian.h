#ifndef RAYTRACINGINONEWEEKEND_LAMBERTIAN_H
#define RAYTRACINGINONEWEEKEND_LAMBERTIAN_H

#include "material.h"

class Lambertian final : public Material {
   public:
    explicit Lambertian(const color::Color& color) : albedo_(color) {}

    inline bool Scatter(const Ray& ray,
                        const HitRecord& hit_record,
                        color::Color& attenuation,
                        Ray& scattered) const override {
        auto scatter_direction = hit_record.normal_ + RandomUnitVector();

        // avoid the case where the random unit vector is exactly opposite the normal vector
        if (scatter_direction.IsNearZero()) {
            scatter_direction = hit_record.normal_;
        }

        scattered = Ray(hit_record.point_, scatter_direction, ray.time());
        attenuation = albedo_;
        return true;
    }

   private:
    color::Color albedo_;
};

#endif  // RAYTRACINGINONEWEEKEND_LAMBERTIAN_H
