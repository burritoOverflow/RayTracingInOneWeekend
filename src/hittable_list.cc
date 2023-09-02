#include "hittable_list.h"

void HittableList::AddObject(const std::shared_ptr<Hittable>& object) {
    objects.push_back(object);
}

bool HittableList::Hit(const Ray& ray,
                       double ray_tmin,
                       double ray_tmax,
                       HitRecord& hit_record) const {
    HitRecord temp_record;
    bool hit_anything{false};
    double closest_so_far{ray_tmax};

    for (const auto& object : objects) {
        if (object->Hit(ray, ray_tmin, closest_so_far, temp_record)) {
            hit_anything = true;
            closest_so_far = temp_record.t_;
            hit_record = temp_record;
        }
    }

    return hit_anything;
}