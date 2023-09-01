#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable {
   public:
    Sphere(Point3 center, double radius) : center_(center), radius_(radius){};

    bool Hit(const Ray& ray,
             double ray_tmin,
             double ray_tmax,
             HitRecord& hit_record) const override;

   private:
    Point3 center_;
    double radius_;
};

#endif  // SPHERE_H