#ifndef HITTABLE_H
#define HITTABLE_H

#include "interval.h"
#include "ray.h"

struct HitRecord {
    Point3 point_;
    Vector3 normal_;
    double t_;
    bool front_face_;

    // pre-condition: outward_normal is assumed to have unit length
    void SetFaceNormal(const Ray& ray, const Vector3& outward_normal) {
        this->front_face_ = Dot(ray.Direction(), outward_normal) < 0;
        this->normal_ = this->front_face_ ? outward_normal : -outward_normal;
    }
};

class Hittable {
   public:
    virtual bool Hit(const Ray& ray, Interval ray_t, HitRecord& hitRecord) const = 0;

    virtual ~Hittable() = default;
};

#endif  // HITTABLE_H