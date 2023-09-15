#ifndef RAYTRACINGINONEWEEKEND_SPHERE_H
#define RAYTRACINGINONEWEEKEND_SPHERE_H

#include "hittable.h"
#include "material.h"
#include "vec3.h"

class Sphere final : public Hittable {
   public:
    Sphere(Point3 center, double radius, std::shared_ptr<Material> material)
        : center_(center), radius_(radius), material_(std::move(material)){};

    bool Hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const override;

   private:
    Point3 center_;
    double radius_;
    std::shared_ptr<Material> material_;
};

#endif  // RAYTRACINGINONEWEEKEND_SPHERE_H
