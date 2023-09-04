#ifndef RAYTRACINGINONEWEEKEND_MATERIAL_H
#define RAYTRACINGINONEWEEKEND_MATERIAL_H

#include "color.h"
#include "hittable.h"

/*
 * Per the docs Material needs to do two things:
 * 1. produce a scattered ray (or say absorbed incident ray)
 * 2. if the scattered ray is produced, say how much the ray should be attenuated
 */
class Material {
   public:
    virtual ~Material() = default;

    virtual bool Scatter(const Ray& ray,
                         const HitRecord& hit_record,
                         color::Color& attenuation,
                         Ray& scattered) const = 0;
};

#endif  // RAYTRACINGINONEWEEKEND_MATERIAL_H
