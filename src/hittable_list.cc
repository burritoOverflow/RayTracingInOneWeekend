#include "hittable_list.h"

void HittableList::AddObject(const std::shared_ptr<Hittable>& object) {
    this->objects_.push_back(object);
}

bool HittableList::Hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const {
    HitRecord temp_record;
    bool did_hit_anything{false};
    double closest_so_far{ray_t.Max()};

    for (const auto& object : objects_) {
        Interval interval{ray_t.Min(), closest_so_far};

        if (object->Hit(ray, interval, temp_record)) {
            did_hit_anything = true;
            closest_so_far = temp_record.t_;
            hit_record = temp_record;
        }
    }

    return did_hit_anything;
}
