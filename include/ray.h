#ifndef RAYTRACINGINONEWEEKEND_RAY_H
#define RAYTRACINGINONEWEEKEND_RAY_H

#include "vec3.h"

/*
 * Ray as a function: P(t) = A + tb
 * where P is a 3D position along a line in 3D
 * A is the ray origin
 * and b is the ray direction
 *
 * changing t and P(t) moves the point along the ray
 *
 * see discussion in Section 4.1
 *
 * Alternatively, see pg 73 in "Fundamentals of Computer Graphics, 4th"
 *
 * p(t) = e + t(s - e)
 * where - 'advance from e along the vector (s - e) a fractional distance t to find the
 * point p
 *
 *  where e is the ray's origin and (s - e) is the ray's direction
 */
class Ray {
   public:
    Ray() = default;
    Ray(const Point3& origin, const Vector3& direction) : origin_(origin), direction_(direction), time_(0.0) {}

    // for motion blur simuulation (see 2.3 in `TheNextWeek`)
    Ray(const Point3& origin, const Vector3& direction, double time)
        : origin_(origin), direction_(direction), time_(time) {}

    Point3 origin() const { return origin_; }
    Vector3 direction() const { return direction_; }
    double time() const { return time_; }

    // Implementation of the function that represents the ray: P(t) = A +tb
    // where P is a 3D position along the line in 3D, A is the Ray's origin, and b is the Ray's direction
    // P(t) "moves along the Ray" for different values of t (see 4.1)
    Point3 at(double t) const { return origin_ + t * direction_; }

   private:
    Point3 origin_;
    Vector3 direction_;
    double time_;
};
#endif  // RAYTRACINGINONEWEEKEND_RAY_H
