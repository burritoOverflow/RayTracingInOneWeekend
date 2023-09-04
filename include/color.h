#ifndef RAYTRACINGINONEWEEKEND_COLOR_H
#define RAYTRACINGINONEWEEKEND_COLOR_H

#include <fstream>
#include "vec3.h"

namespace color {
using Color = Vector3;

static inline double LinearToGamma(const double linear_component) {
    return sqrt(linear_component);
}

void WriteColor(std::ofstream& outstream, const Color& pixel_color, int samples_per_pixel);
}  // namespace color

#endif  // RAYTRACINGINONEWEEKEND_COLOR_H
