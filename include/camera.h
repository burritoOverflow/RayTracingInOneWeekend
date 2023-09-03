#ifndef RAYTRACINGINONEWEEKEND_CAMERA_H
#define RAYTRACINGINONEWEEKEND_CAMERA_H

#include "color.h"
#include "hittable.h"

class Camera final {
   public:
    void Render(const Hittable& world);

    void SetMaxRecursionDepth(int max_recursion_depth) {
        this->max_recursion_depth_ = max_recursion_depth;
    }

   private:
    double aspect_ratio_ = 16.0 / 9.0;  // ratio of image width over height
    int image_width_ = 1000;            // rendered image width in pixel count
    int image_height_;                  // rendered image height
    Point3 camera_center_;
    Point3 pixel00_loc_;            // location of pixel 0,0
    Vector3 pixel_delta_u_;         // delta_u - distance between pixels on the x-axis
    Vector3 pixel_delta_v_;         // delta_v - distance between pixels on the y-axis
    int samples_per_pixel_ = 100;   // count of random samples per pixel
    int max_recursion_depth_ = 10;  // limit the number of ray bounces in the scene

    void Init();

    static color::Color RayColor(const Ray& ray, const Hittable& world, int depth);

    Ray GetRay(int i, int j) const;

    Vector3 PixelSampleSquare() const;
};

#endif  // RAYTRACINGINONEWEEKEND_CAMERA_H
