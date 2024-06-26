#include "color.h"
#include "interval.h"

void color::WriteColor(std::ofstream& out_stream, const Color& pixel_color, const int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // divide the color by the number of samples
    const auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // apply the linear to gamma transformation (see 9.5)
    r = LinearToGamma(r);
    g = LinearToGamma(g);
    b = LinearToGamma(b);

    // write the translated[0, 255] for each color component (see 8.2)
    static const Interval intensity{0.000, 0.999};

    out_stream << static_cast<int>(255.999 * intensity.Clamp(r)) << ' '
               << static_cast<int>(255.999 * intensity.Clamp(g)) << ' '
               << static_cast<int>(255.999 * intensity.Clamp(b)) << '\n';
}
