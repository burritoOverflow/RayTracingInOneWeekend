#include "color.h"
#include "interval.h"

void color::WriteColor(std::ofstream& outstream,
                       const Color& pixel_color,
                       const int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // divide the color by the number of samples
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // write the translated[0, 255] for each color component (see 8.2)
    static const Interval intensity{0.000, 0.999};

    outstream << static_cast<int>(255.999 * intensity.clamp(r)) << ' '
              << static_cast<int>(255.999 * intensity.clamp(g)) << ' '
              << static_cast<int>(255.999 * intensity.clamp(b)) << '\n';
}
