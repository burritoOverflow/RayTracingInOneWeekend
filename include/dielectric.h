#ifndef RAYTRACINGINONEWEEKEND_DIELECTRIC_H
#define RAYTRACINGINONEWEEKEND_DIELECTRIC_H

#include "material.h"

class Dielectric final : public Material {
   public:
    Dielectric(const double index_of_refraction) : ir_(index_of_refraction) {}

    inline bool Scatter(const Ray& ray,
                        const HitRecord& hit_record,
                        color::Color& attenuation,
                        Ray& scattered) const override {
        attenuation = color::Color(1.0, 1.0, 1.0);

        double refraction_ratio = hit_record.front_face_ ? (1.0 / ir_) : ir_;
        const Vector3 unit_direction = UnitVector(ray.direction());
        const double cos_theta = fmin(Dot(-unit_direction, hit_record.normal_), 1.0);
        const double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        Vector3 direction{};

        // see 11.3 for details
        // check if equality between the two sides of the equation is broken
        // if so, it must reflect (solution does not exist)
        const bool cannot_refract = refraction_ratio * sin_theta > 1.0;

        if (cannot_refract) {
            direction = Reflect(unit_direction, hit_record.normal_);
        } else {
            direction = Refract(unit_direction, hit_record.normal_, refraction_ratio);
        }

        scattered = Ray(hit_record.point_, direction);

        return true;
    }

   private:
    double ir_;
};

#endif  // RAYTRACINGINONEWEEKEND_DIELECTRIC_H
