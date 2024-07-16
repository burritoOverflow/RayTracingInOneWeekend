#include "quad.h"
#include <cmath>
#include "interval.h"
#include "vec3.h"

// see section 6.3
bool Quad::Hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const {
    const auto denominator = Dot(this->normal_, ray.direction());

    // ray parallel to the plane; no hit
    if (std::fabs(denominator) < 1e-8) {
        return false;
    }

    // return false if the hit point parameter t is outside of the Ray interval
    const auto t = (D_ - Dot(this->normal_, ray.origin())) / denominator;
    if (!ray_t.Contains(t)) {
        return false;
    }

    const auto intersection = ray.at(t);
    const Vector3 planar_hitpt_vector = intersection - Q_;

    // see Derivation for these values in section 6.5
    const auto alpha = Dot(w_, Cross(planar_hitpt_vector, v_));
    const auto beta = Dot(w_, Cross(u_, planar_hitpt_vector));

    if (!this->IsInterior(alpha, beta, hit_record)) {
        return false;
    }

    // Otherwise, Ray hits the 2D shape; set the HitRecord
    hit_record.t_ = t;
    hit_record.point_ = intersection;
    hit_record.material_ = material_;
    hit_record.SetFaceNormal(ray, this->normal_);

    return true;
}

// Given hit point in Plane coordinates, return False if outside the primitive, else set the UV coordinates
// + return True
bool Quad::IsInterior(const double a, const double b, HitRecord& hit_record) const {
    Interval unit_interval{0, 1};

    if (!unit_interval.Contains(a) || !unit_interval.Contains(b)) {
        return false;
    }

    hit_record.u_ = a;
    hit_record.v_ = b;
    return true;
}