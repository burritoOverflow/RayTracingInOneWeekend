#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct HitRecord {
    Point3 point_;
    Vector3 normal_;
    double t_;
    bool front_face_;

    // pre-condition: outward_normal is assumed to have unit length
    void SetFaceNormal(const Ray& ray, const Vector3& outward_normal) {
        front_face_ = Dot(ray.direction(), outward_normal) < 0;
        normal_ = front_face_ ? outward_normal : -outward_normal;
    }
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