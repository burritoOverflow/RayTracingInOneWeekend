#include "sphere.h"
#include "interval.h"

bool Sphere::Hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const {
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

    if (!ray_t.surrounds(root)) {
        root = (-half_b + sqrtd) / a;

        if (!ray_t.surrounds(root))
            return false;
    }

    hit_record.t_ = root;
    hit_record.point_ = ray.at(hit_record.t_);

    // 6.4 front vs back faces
    const Vector3 outward_normal = (hit_record.point_ - center_) / radius_;
    hit_record.SetFaceNormal(ray, outward_normal);

    return true;
}
