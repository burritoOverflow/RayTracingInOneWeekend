#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class Sphere final : public Hittable {
   public:
    Sphere(Point3 center, double radius) : center_(center), radius_(radius){};

    bool Hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const override;

   private:
    Point3 center_;
    double radius_;
};

#endif  // SPHERE_H