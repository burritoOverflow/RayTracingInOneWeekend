#ifndef RAYTRACINGINONEWEEKEND_LAMBERTIAN_H
#define RAYTRACINGINONEWEEKEND_LAMBERTIAN_H

#include <memory>
#include "material.h"
#include "texture.h"

class Lambertian final : public Material {
   public:
    explicit Lambertian(const color::Color& albedo) : tex_(std::make_shared<SolidColor>(albedo)) {}
    explicit Lambertian(const std::shared_ptr<Texture>& texture) : tex_(texture){};

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
        attenuation = this->tex_->Value(hit_record.u_, hit_record.v_, hit_record.point_);

        return true;
    }

   private:
    std::shared_ptr<Texture> tex_;
};

#endif  // RAYTRACINGINONEWEEKEND_LAMBERTIAN_H
