#include "bvhnode.h"

bool BoundingVolumeHierarchyNode::Hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const {
    if (!this->bbox_.Hit(ray, ray_t)) {
        return false;
    }

    const bool hit_left = this->left_->Hit(ray, ray_t, hit_record);
    const auto interval = Interval{ray_t.Min(), hit_left ? hit_record.t_ : ray_t.Max()};
    const bool hit_right = this->right_->Hit(ray, interval, hit_record);

    return hit_left || hit_right;
};

bool BoundingVolumeHierarchyNode::BoxCompare(const std::shared_ptr<Hittable>& a,
                                             const std::shared_ptr<Hittable>& b,
                                             const int axis_index) {
    const auto a_axis_interval = a->BoundingBox().AxisInterval(axis_index);
    const auto b_axis_interval = b->BoundingBox().AxisInterval(axis_index);

    return a_axis_interval.Min() < b_axis_interval.Min();
}

bool BoundingVolumeHierarchyNode::BoxXCompare(const std::shared_ptr<Hittable>& a,
                                              const std::shared_ptr<Hittable>& b) {
    return BoundingVolumeHierarchyNode::BoxCompare(a, b, 0);
}

bool BoundingVolumeHierarchyNode::BoxYCompare(const std::shared_ptr<Hittable>& a,
                                              const std::shared_ptr<Hittable>& b) {
    return BoundingVolumeHierarchyNode::BoxCompare(a, b, 1);
}

bool BoundingVolumeHierarchyNode::BoxZCompare(const std::shared_ptr<Hittable>& a,
                                              const std::shared_ptr<Hittable>& b) {
    return BoundingVolumeHierarchyNode::BoxCompare(a, b, 2);
}
