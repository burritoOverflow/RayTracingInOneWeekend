#include "quad.h"
#include <math.h>
#include <cmath>
#include <memory>
#include "hittable_list.h"
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

// returns the 3D (6 sided) box that contains the two opposite vertices a and b.
std::shared_ptr<HittableList> Box(const Point3& a,
                                  const Point3& b,
                                  const std::shared_ptr<Material>& material) {
    auto sides = std::make_shared<HittableList>();

    // construct two opposite vertices with min and max coordinates
    const auto min = Point3{fmin(a.x(), b.x()), fmin(a.y(), b.y()), fmin(a.z(), b.z())};
    const auto max = Point3{fmax(a.x(), b.x()), fmax(a.y(), b.y()), fmax(a.z(), b.z())};

    const auto dx = Vector3{max.x() - min.x(), 0, 0};
    const auto dy = Vector3{0, max.y() - min.y(), 0};
    const auto dz = Vector3{0, 0, max.z() - min.z()};

    sides->AddObject(std::make_shared<Quad>(Point3(min.x(), min.y(), max.z()), dx, dy, material));   // front
    sides->AddObject(std::make_shared<Quad>(Point3(max.x(), min.y(), max.z()), -dz, dy, material));  // right
    sides->AddObject(std::make_shared<Quad>(Point3(max.x(), min.y(), min.z()), -dx, dy, material));  // back
    sides->AddObject(std::make_shared<Quad>(Point3(min.x(), min.y(), min.z()), dz, dy, material));   // left
    sides->AddObject(std::make_shared<Quad>(Point3(min.x(), max.y(), max.z()), dx, -dz, material));  // top
    sides->AddObject(std::make_shared<Quad>(Point3(min.x(), min.y(), min.z()), dx, dz, material));   // bottom

    return sides;
}