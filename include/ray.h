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
 *  where e is the ray's Origin and (s - e) is the ray's Direction
 */
class Ray {
   public:
    Ray() = default;
    Ray(const Point3& origin, const Vector3& direction) : origin_(origin), direction_(direction) {}

    Point3 Origin() const { return origin_; }
    Vector3 Direction() const { return direction_; }

    // Implementation of the function that represents the ray: P(t) = A +tb
    Point3 at(double t) const { return origin_ + t * direction_; }

   private:
    Point3 origin_;
    Vector3 direction_;
};
#endif  // RAYTRACINGINONEWEEKEND_RAY_H
