#ifndef RAYTRACINGINONEWEEKEND_SPHERE_H
#define RAYTRACINGINONEWEEKEND_SPHERE_H

#include <cmath>
#include <memory>
#include "aabb.h"
#include "hittable.h"
#include "material.h"
#include "vec3.h"

class Sphere final : public Hittable {
   public:
    // default ctor is a stationary Sphere
    Sphere(const Point3& center, const double radius, const std::shared_ptr<Material>& material)
        : center_(std::move(center)), radius_(std::fmax(0, radius)), material_(material), is_moving_(false) {
        const auto rvec = Vector3(radius, radius, radius);
        this->bbox_ = AxisAlignedBoundingBox(center - rvec, center + rvec);
    };

    // overloaded ctor for a moving Sphere
    Sphere(const Point3& center1,
           const Point3& center2,
           double radius,
           const std::shared_ptr<Material>& material)
        : center_(std::move(center1)), radius_(std::fmax(0, radius)), material_(material), is_moving_(true) {
        const auto rvec = Vector3(radius, radius, radius);
        const AxisAlignedBoundingBox bbox1 = AxisAlignedBoundingBox(center1 - rvec, center1 + rvec);
        const AxisAlignedBoundingBox bbox2 = AxisAlignedBoundingBox(center2 - rvec, center2 + rvec);

        this->bbox_ = AxisAlignedBoundingBox(bbox1, bbox2);

        this->center_vec_ = center2 - center_;
    };

    bool Hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const override;

    AxisAlignedBoundingBox BoundingBox() const override { return this->bbox_; }

   private:
    Point3 center_;
    double radius_;
    std::shared_ptr<Material> material_;
    bool is_moving_;
    Vector3 center_vec_;
    AxisAlignedBoundingBox bbox_;

    // Section 2.4:
    // Linearly interpolate from `center1` to `center2` according to time, where t=0 yields
    // center1, and t=1 yields center2.
    inline Point3 GetSphereCenter(const double time) const { return this->center_ + time * this->center_vec_; }
};

#endif  // RAYTRACINGINONEWEEKEND_SPHERE_H
