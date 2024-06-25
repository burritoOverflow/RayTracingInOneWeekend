#ifndef RAYTRACINGINONEWEEKEND_METAL_H
#define RAYTRACINGINONEWEEKEND_METAL_H

#include "material.h"

class Metal final : public Material {
   public:
    explicit Metal(const color::Color& color, const double fuzz_factor)
        : albedo_(color), fuzz_(fuzz_factor < 1 ? fuzz_factor : 1) {}

    inline bool Scatter(const Ray& ray,
                        const HitRecord& hit_record,
                        color::Color& attenuation,
                        Ray& scattered) const override {
        Vector3 reflected = Reflect(UnitVector(ray.direction()), hit_record.normal_);
        scattered = Ray(hit_record.point_, reflected, ray.time());
        attenuation = albedo_;

        return (Dot(scattered.direction(), hit_record.normal_) > 0);
    }

   private:
    color::Color albedo_;
    double fuzz_;  // see 10.6 fuzzy reflection (0 is no perturbation)
};

#endif  // RAYTRACINGINONEWEEKEND_METAL_H
