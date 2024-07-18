#ifndef QUAD_H
#define QUAD_H

#include <memory>
#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "vec3.h"

class Quad : public Hittable {
   public:
    Quad(const Point3& Q, const Vector3& u, const Vector3& v, const std::shared_ptr<Material>& material)
        : Q_(Q), u_(u), v_(v), material_(material) {
        const auto n = Cross(u, v);
        this->normal_ = UnitVector(n);
        this->D_ = Dot(this->normal_, Q);
        this->w_ = n / Dot(n, n);

        this->SetAxisAlignedBoundingBox();
    }

    virtual void SetAxisAlignedBoundingBox() {
        // see section 6.1 for Quadrilateral
        const auto bbox_diagonal1 = AxisAlignedBoundingBox{Q_, Q_ + u_ + v_};
        const auto bbox_diagonal2 = AxisAlignedBoundingBox{Q_ + u_, Q_ + v_};
        this->bbox_ = AxisAlignedBoundingBox{bbox_diagonal1, bbox_diagonal2};
    }

    virtual bool IsInterior(const double a, const double b, HitRecord& hit_record) const;

    bool Hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const override;

    AxisAlignedBoundingBox BoundingBox() const override { return this->bbox_; }

   private:
    Point3 Q_;       // statrting corner
    Vector3 u_, v_;  // u - first side, v - second side; Q+u gives one of the corners adjacent to Q, Q+v gives
    // the other corner adjacent to Q
    Vector3 w_;  // P = Q + au + Bv  introduced in section 6.4
    Vector3 normal_;
    double D_;  // Ax + By + Cz = D (See section 6.2)
    std::shared_ptr<Material> material_;
    AxisAlignedBoundingBox bbox_;
};

std::shared_ptr<HittableList> Box(const Point3& a, const Point3& b, const std::shared_ptr<Material>& material);

#endif  // QUAD_H