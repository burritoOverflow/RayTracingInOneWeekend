#ifndef BOUNDING_VOLUME_HIERARCHY_NODE_H
#define BOUNDING_VOLUME_HIERARCHY_NODE_H

#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>
#include "aabb.h"
#include "config.h"
#include "hittable.h"
#include "hittable_list.h"
#include "interval.h"

class BoundingVolumeHierarchyNode : public Hittable {
   public:
    // see comments on the rationale here:
    // https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies/thebvhnodeclass
    BoundingVolumeHierarchyNode(HittableList hittable_list)
        : BoundingVolumeHierarchyNode(hittable_list.GetObjects(), 0, hittable_list.GetObjects().size()){};

    BoundingVolumeHierarchyNode(std::vector<std::shared_ptr<Hittable>>& objects,
                                const size_t start,
                                const size_t end) {
        const int axis = config::GetRandomInt(0, 2);
        const auto comparator = (axis == 0)   ? BoundingVolumeHierarchyNode::BoxXCompare
                                : (axis == 1) ? BoundingVolumeHierarchyNode::BoxYCompare
                                              : BoundingVolumeHierarchyNode::BoxZCompare;

        const size_t object_span = end - start;

        if (object_span == 1) {
            this->left_ = right_ = objects.at(start);
        } else if (object_span == 2) {
            // two elements; one in each substree
            this->left_ = objects.at(start);
            this->right_ = objects.at(start + 1);
        } else {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            const auto mid = start + object_span / 2;
            this->left_ = std::make_shared<BoundingVolumeHierarchyNode>(objects, start, mid);
            this->right_ = std::make_shared<BoundingVolumeHierarchyNode>(objects, mid, end);
        }

        this->bbox_ = AxisAlignedBoundingBox(this->left_->BoundingBox(), this->right_->BoundingBox());
    };

    bool Hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const override;

    inline AxisAlignedBoundingBox BoundingBox() const override { return this->bbox_; }

   private:
    std::shared_ptr<Hittable> left_;
    std::shared_ptr<Hittable> right_;
    AxisAlignedBoundingBox bbox_;

    static bool BoxCompare(const std::shared_ptr<Hittable>& a,
                           const std::shared_ptr<Hittable>& b,
                           const int axis_index);

    static bool BoxXCompare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b);
    static bool BoxYCompare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b);
    static bool BoxZCompare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b);
};

#endif  // BOUNDING_VOLUME_HIERARCHY_NODE_H