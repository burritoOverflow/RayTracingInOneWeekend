#include "hittable.h"
#include <cmath>
#include "aabb.h"
#include "config.h"
#include "ray.h"
#include "vec3.h"

RotateY::RotateY(const std::shared_ptr<Hittable>& object, const double angle) : object_(object) {
    const auto radians = config::DegreesToRadians(angle);
    this->sin_theta_ = std::sin(radians);
    this->cos_theta_ = std::cos(radians);

    this->bbox_ = object->BoundingBox();

    Point3 min(config::infinity, config::infinity, config::infinity);
    Point3 max(-config::infinity, -config::infinity, -config::infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                const auto x = i * this->bbox_.x().Max() + (1 - i) * bbox_.x().Min();
                const auto y = i * this->bbox_.y().Max() + (1 - j) * bbox_.y().Min();
                const auto z = i * this->bbox_.z().Max() + (1 - k) * bbox_.z().Min();

                const auto new_x = this->cos_theta_ * x + sin_theta_ * z;
                const auto new_z = -this->sin_theta_ * x + this->cos_theta_ * z;

                const Vector3 tester{new_x, y, new_z};

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }
    this->bbox_ = AxisAlignedBoundingBox{min, max};
}

// see implementation in section 8.2 - "Instance Rotation"
bool RotateY::Hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const {
    // change the Ray from World to Object space (rotate by -theta)
    // x' = cos(theta) * x -sin(theta) * z
    // z' = sin(theta) * x + cos(theta) * z
    auto origin = ray.origin();
    auto direction = ray.direction();

    origin[0] = this->cos_theta_ * ray.origin()[0] - this->sin_theta_ * ray.origin()[2];
    origin[2] = this->sin_theta_ * ray.origin()[0] + this->cos_theta_ * ray.origin()[2];

    direction[0] = this->cos_theta_ * ray.direction()[0] - this->sin_theta_ * ray.direction()[2];
    direction[2] = this->sin_theta_ * ray.direction()[0] + this->cos_theta_ * ray.direction()[2];

    Ray rotated_r{origin, direction, ray.time()};

    // determine if and where an intersection exists in Object Space
    if (!object_->Hit(rotated_r, ray_t, hit_record)) {
        return false;
    }

    // change the intersection point from Object space to World space
    auto p = hit_record.point_;
    p[0] = this->cos_theta_ * hit_record.point_[0] + this->sin_theta_ * hit_record.point_[2];
    p[2] = -this->sin_theta_ * hit_record.point_[0] + this->cos_theta_ * hit_record.point_[2];

    // convert normal from Object space to World space
    auto normal = hit_record.normal_;
    normal[0] = this->cos_theta_ * hit_record.normal_[0] + this->sin_theta_ * hit_record.normal_[2];
    normal[2] = -this->sin_theta_ * hit_record.normal_[0] + this->cos_theta_ * hit_record.normal_[2];

    hit_record.point_ = p;
    hit_record.normal_ = normal;

    return true;
}