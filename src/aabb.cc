#include "aabb.h"
#include "interval.h"
#include "vec3.h"

// see section 3.4 in `TheNextWeek`
bool AxisAlignedBoundingBox::Hit(const Ray& ray, Interval& ray_t) {
    const Point3& ray_origin = ray.origin();
    const Vector3& ray_direction = ray.direction();

    for (int axis = 0; axis < 3; ++axis) {
        const Interval& ax = AxisInterval(axis);
        const double adinv = 1.0 / ray_direction[axis];

        const auto t0 = (ax.Min() - ray_origin[axis]) * adinv;
        const auto t1 = (ax.Max() - ray_origin[axis]) * adinv;

        if (t0 < t1) {
            if (t0 > ray_t.Min()) {
                ray_t.Min(t0);
            }
            if (t1 < ray_t.Max()) {
                ray_t.Max(t1);
            }
        } else {
            if (t1 > ray_t.Min()) {
                ray_t.Min(t1);
            }
            if (t0 < ray_t.Max()) {
                ray_t.Max(t0);
            }
        }

        if (ray_t.Max() <= ray_t.Min()) {
            return false;
        }
    }
    return true;
}