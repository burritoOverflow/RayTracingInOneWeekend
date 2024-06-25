#ifndef RAYTRACINGINONEWEEKEND_DIELECTRIC_H
#define RAYTRACINGINONEWEEKEND_DIELECTRIC_H

#include "config.h"
#include "material.h"

class Dielectric final : public Material {
   public:
    Dielectric(const double index_of_refraction) : ir_(index_of_refraction) {}

    inline bool Scatter(const Ray& ray,
                        const HitRecord& hit_record,
                        color::Color& attenuation,
                        Ray& scattered) const override {
        attenuation = color::Color(1.0, 1.0, 1.0);
        const double refraction_ratio = hit_record.has_front_face_ ? (1.0 / ir_) : ir_;

        const Vector3 unit_direction = UnitVector(ray.direction());
        const double cos_theta = fmin(Dot(-unit_direction, hit_record.normal_), 1.0);
        const double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        Vector3 direction{};

        // see 11.3 for details
        // check if equality between the two sides of the equation is broken
        // if so, it must reflect (solution does not exist)
        const bool cannot_refract = (refraction_ratio * sin_theta) > 1.0;

        if (cannot_refract || Reflectance(cos_theta, refraction_ratio) > config::GetRandomDouble()) {
            direction = Reflect(unit_direction, hit_record.normal_);
        } else {
            direction = Refract(unit_direction, hit_record.normal_, refraction_ratio);
        }

        scattered = Ray(hit_record.point_, direction, ray.time());

        return true;
    }

   private:
    double ir_;

    static double Reflectance(const double cosine, const double ref_idx) {
        // Schlick's approximation (see 11.4)
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif  // RAYTRACINGINONEWEEKEND_DIELECTRIC_H
