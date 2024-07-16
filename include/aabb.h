#ifndef RAYTRACINGINONEWEEKEND_AABB_H
#define RAYTRACINGINONEWEEKEND_AABB_H

#include <type_traits>
#include "interval.h"
#include "ray.h"
#include "vec3.h"

class AxisAlignedBoundingBox {
   public:
    AxisAlignedBoundingBox(){};  // default AABB is empy since intervals are empty by default

    AxisAlignedBoundingBox(const Interval& x, const Interval& y, const Interval& z) : x_(x), y_(y), z_(z) {
        this->PadToMinimum();
    }

    AxisAlignedBoundingBox(const Point3& a, const Point3& b) {
        // Treat the two points a and b as extrema for the bounding box, so we don't require a
        // particular minimum/maximum coordinate order.
        x_ = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
        y_ = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
        z_ = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);

        this->PadToMinimum();
    }

    AxisAlignedBoundingBox(const AxisAlignedBoundingBox& bbox1, const AxisAlignedBoundingBox& bbox2) {
        this->x_ = Interval(bbox1.x_, bbox2.x_);
        this->y_ = Interval(bbox1.y_, bbox2.y_);
        this->z_ = Interval(bbox1.z_, bbox2.z_);
    }

    static const AxisAlignedBoundingBox EMPTY, UNIVERSE;

    inline const Interval AxisInterval(const int n) const {
        if (n == 1) {
            return y_;
        }
        if (n == 2) {
            return z_;
        }
        return x_;
    }

    bool Hit(const Ray& ray, Interval& ray_t) const;

    inline int GetLongestAxis() const {
        if (this->x_.GetSize() > this->y_.GetSize()) {
            return this->x_.GetSize() > this->z_.GetSize() ? 0 : 2;
        } else {
            return this->y_.GetSize() > this->z_.GetSize() ? 1 : 2;
        }
    }

   private:
    Interval x_, y_, z_;

    // adjust the AABB so that no side is narrower than some delta, add padding if necessary.
    void PadToMinimum() {
        const double DELTA{0.0001};
        if (x_.GetSize() < DELTA) {
            x_ = x_.Expand(DELTA);
        }
        if (y_.GetSize() < DELTA) {
            y_ = y_.Expand(DELTA);
        }
        if (z_.GetSize() < DELTA) {
            z_ = z_.Expand(DELTA);
        }
    }
};

#endif  // RAYTRACINGINONEWEEKEND_AABB_H