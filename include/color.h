#ifndef RAYTRACINGINONEWEEKEND_COLOR_H
#define RAYTRACINGINONEWEEKEND_COLOR_H

#include <fstream>
#include "vec3.h"

using Color = Vector3;

void WriteColor(std::ofstream& outstream, Color pixel_color) {
    outstream << static_cast<int>(255.999 * pixel_color.x()) << ' '
              << static_cast<int>(255.999 * pixel_color.y()) << ' '
              << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif  // RAYTRACINGINONEWEEKEND_COLOR_H
