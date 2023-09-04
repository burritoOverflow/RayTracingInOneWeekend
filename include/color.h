#ifndef RAYTRACINGINONEWEEKEND_COLOR_H
#define RAYTRACINGINONEWEEKEND_COLOR_H

#include <fstream>
#include "vec3.h"

namespace color {
using Color = Vector3;

// see 9.5 for gamma correction rationale, and a detailed discussion in section 3.2.2 of
// "Fundamentals of Computer Graphics"
static inline double LinearToGamma(const double linear_component) {
    // use "gamma 2" as this is the power when going to grom gamma space to linear space
    // use sqrt to invert this
    return sqrt(linear_component);
}

void WriteColor(std::ofstream& out_stream, const Color& pixel_color, int samples_per_pixel);
}  // namespace color

#endif  // RAYTRACINGINONEWEEKEND_COLOR_H
