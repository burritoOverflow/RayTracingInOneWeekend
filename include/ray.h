//
// Created by rob on 8/20/23.
//

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
 */
class Ray {
   public:
    Ray() = default;
    Ray(const Point3& origin, const Vector3& direction) : origin_(origin), direction_(direction) {}

    Point3 origin() { return origin_; }
    Vector3 direction() { return direction_; }

    Point3 at(double d) { return origin_ + d * direction_; }

   private:
    Point3 origin_;
    Vector3 direction_;
};
#endif  // RAYTRACINGINONEWEEKEND_RAY_H
