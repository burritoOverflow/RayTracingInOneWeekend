#ifndef RAYTRACINGINONEWEEKEND_COLOR_H
#define RAYTRACINGINONEWEEKEND_COLOR_H

#include <fstream>
#include "vec3.h"

namespace color {
using Color = Vector3;

void WriteColor(std::ofstream& outstream, const Color& pixel_color);
}  // namespace color

#endif  // RAYTRACINGINONEWEEKEND_COLOR_H
