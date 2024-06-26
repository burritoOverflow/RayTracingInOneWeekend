#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>
#include "aabb.h"
#include "hittable.h"

class HittableList final : public Hittable {
   public:
    HittableList() = default;

    explicit HittableList(const std::shared_ptr<Hittable>& object) { AddObject(object); }

    void AddObject(const std::shared_ptr<Hittable>& object);

    bool Hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const override;

    AxisAlignedBoundingBox BoundingBox() const override { return this->bbox_; }

   private:
    std::vector<std::shared_ptr<Hittable>> objects_;
    AxisAlignedBoundingBox bbox_;
};

#endif  // HITTABLE_LIST_H
