#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct HitRecord {
    Point3 point_;
    Vector3 normal_;
    double t_;
};

class Hittable {
   public:
    virtual bool Hit(const Ray& ray,
                     double ray_tmin,
                     double ray_tmax,
                     HitRecord& hitRecord) const = 0;

    virtual ~Hittable() = default;
};

#endif  // HITTABLE_H