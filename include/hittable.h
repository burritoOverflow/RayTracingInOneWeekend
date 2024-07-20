#ifndef RAYTRACINGINONEWEEKEND_HITTABLE_H
#define RAYTRACINGINONEWEEKEND_HITTABLE_H

#include <memory>
#include "aabb.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

class Material;

struct HitRecord {
    Point3 point_;
    Vector3 normal_;
    double t_;
    // texture coordinates
    double u_;
    double v_;
    bool has_front_face_;
    std::shared_ptr<Material> material_;

    // pre-condition: outward_normal is assumed to have unit length
    void SetFaceNormal(const Ray& ray, const Vector3& outward_normal) {
        this->has_front_face_ = Dot(ray.direction(), outward_normal) < 0;
        this->normal_ = this->has_front_face_ ? outward_normal : -outward_normal;
    }

    void SetMaterial(std::shared_ptr<Material> material) { material_ = std::move(material); }
};

class Hittable {
   public:
    virtual bool Hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const = 0;

    virtual AxisAlignedBoundingBox BoundingBox() const = 0;

    virtual ~Hittable() = default;
};

class Translate : public Hittable {
   public:
    Translate(const std::shared_ptr<Hittable>& object, const Vector3& offset)
        : object_(object), offset_(offset) {
        this->bbox_ = object->BoundingBox() + offset;
    }

    inline AxisAlignedBoundingBox BoundingBox() const override { return this->bbox_; }

    inline bool Hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const override {
        // move Ray backwards by the offset
        Ray offset_r{ray.origin() - offset_, ray.direction(), ray.time()};

        // Determine whether (and where) an Intersection exists along the offset Ray
        if (!object_->Hit(offset_r, ray_t, hit_record)) {
            return false;
        }

        // adjust intersection Point forwards by the offset
        hit_record.point_ += offset_;
        return true;
    }

   private:
    std::shared_ptr<Hittable> object_;
    Vector3 offset_;
    AxisAlignedBoundingBox bbox_;
};

class RotateY : public Hittable {
   public:
    RotateY(const std::shared_ptr<Hittable>& object, const double angle);

    inline AxisAlignedBoundingBox BoundingBox() const override { return this->bbox_; }

    bool Hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const override;

   private:
    std::shared_ptr<Hittable> object_;
    double sin_theta_;
    double cos_theta_;
    AxisAlignedBoundingBox bbox_;
};

#endif  // RAYTRACINGINONEWEEKEND_HITTABLE_H
