#ifndef RAYTRACINGINONEWEEKEND_HITTABLE_H
#define RAYTRACINGINONEWEEKEND_HITTABLE_H

#include <memory>
#include "interval.h"
#include "ray.h"

class Material;

struct HitRecord {
    Point3 point_;
    Vector3 normal_;
    double t_;
    bool front_face_;
    std::shared_ptr<Material> material_;

    // pre-condition: outward_normal is assumed to have unit length
    void SetFaceNormal(const Ray& ray, const Vector3& outward_normal) {
        this->front_face_ = Dot(ray.direction(), outward_normal) < 0;
        this->normal_ = this->front_face_ ? outward_normal : -outward_normal;
    }

    void set_material(std::shared_ptr<Material> material) { material_ = std::move(material); }
};

class Hittable {
   public:
    virtual bool Hit(const Ray& ray, Interval ray_t, HitRecord& hitRecord) const = 0;

    virtual ~Hittable() = default;
};

#endif  // RAYTRACINGINONEWEEKEND_HITTABLE_H
