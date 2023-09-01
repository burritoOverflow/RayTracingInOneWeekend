#include "sphere.h"

bool Sphere::Hit(Ray& ray, double ray_tmin, double ray_tmax, HitRecord& hit_record) const {
    // see walkthrough in section 5.1 (and the subsequent changes in section 6.2)
    const Vector3 oc = ray.origin() - center_;
    const auto a = ray.direction().LengthSquared();
    const auto half_b = Dot(oc, ray.direction());
    const auto c = oc.LengthSquared() - radius_ * radius_;
    const auto discriminant = half_b * half_b - a * c;

    if (discriminant < 0) {
        return false;
    }

    const auto sqrtd = sqrt(discriminant);

    // find the nearest root in the range (see 6.3)
    auto root = (-half_b - sqrtd) / a;

    if (root <= ray_tmin || ray_tmax <= root) {
        root = (-half_b + sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root)
            return false;
    }

    hit_record.t_ = root;
    hit_record.point_ = ray.at(hit_record.t_);
    hit_record.normal_ = (hit_record.point_ - center_) / radius_;

    return true;
}
