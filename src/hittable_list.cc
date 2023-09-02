#include "hittable_list.h"

void HittableList::AddObject(const std::shared_ptr<Hittable>& object) {
    objects.push_back(object);
}

bool HittableList::Hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const {
    HitRecord temp_record;
    bool hit_anything{false};
    double closest_so_far{ray_t.max()};

    for (const auto& object : objects) {
        Interval interval{ray_t.min(), closest_so_far};

        if (object->Hit(ray, interval, temp_record)) {
            hit_anything = true;
            closest_so_far = temp_record.t_;
            hit_record = temp_record;
        }
    }

    return hit_anything;
}
