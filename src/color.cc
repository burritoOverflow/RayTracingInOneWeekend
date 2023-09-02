#include "color.h"

void color::WriteColor(std::ofstream& outstream, const Color& pixel_color) {
    outstream << static_cast<int>(255.999 * pixel_color.x()) << ' '
              << static_cast<int>(255.999 * pixel_color.y()) << ' '
              << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}
