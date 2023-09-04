#ifndef RAYTRACINGINONEWEEKEND_METAL_H
#define RAYTRACINGINONEWEEKEND_METAL_H

#include "material.h"

class Metal final : public Material {
   public:
    explicit Metal(const color::Color& color) : albedo_(color) {}

    inline bool Scatter(const Ray& ray,
                        const HitRecord& hit_record,
                        color::Color& attenuation,
                        Ray& scattered) const override {
        Vector3 reflected = Reflect(UnitVector(ray.direction()), hit_record.normal_);
        scattered = Ray(hit_record.point_, reflected);
        attenuation = albedo_;

        return true;
    }

   private:
    color::Color albedo_;
};

#endif  // RAYTRACINGINONEWEEKEND_METAL_H
