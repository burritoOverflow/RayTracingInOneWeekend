#ifndef CONFIG_H
#define CONFIG_H

#include "color.h"

// see rationale for these constants in section 4.2

namespace config {
// Image

const auto ASPECT_RATIO = 16.0 / 9.0;
const int IMAGE_WIDTH = 800;
const int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO);
const auto VIEWPORT_HEIGHT = 2.0;
const auto VIEWPORT_WIDTH = VIEWPORT_HEIGHT * (static_cast<double>(IMAGE_WIDTH) / IMAGE_HEIGHT);

// Camera

const auto FOCAL_LENGTH = 1.0;
const auto CAMERA_CENTER = Point3(0, 0, 0);

// Viewport vectors (see section 4.2)

// v_u - vector from the left to the right edge
const auto VIEWPORT_U = Vector3(VIEWPORT_WIDTH, 0, 0);

// v_v - vector from the upper edge to the lower edge
// y increases down the image
const auto VIEWPORT_V = Vector3(0, -VIEWPORT_HEIGHT, 0);

// Calculate the horizontal and vertical delta vectors from pixel to pixel.

// delta_u - distance between pixels on the x-axis
const auto PIXEL_DELTA_U = VIEWPORT_U / IMAGE_WIDTH;

// delta_v - distance between pixels on the y-axis
const auto PIXEL_DELTA_V = VIEWPORT_V / IMAGE_HEIGHT;

// location of the upper left pixel - x = 0, y = 0
const auto VIEWPORT_UPPER_LEFT =
    CAMERA_CENTER - Vector3(0, 0, FOCAL_LENGTH) - VIEWPORT_U / 2 - VIEWPORT_V / 2;

const auto PIXEL00_LOC = VIEWPORT_UPPER_LEFT + 0.5 * (PIXEL_DELTA_U + PIXEL_DELTA_V);

const std::string DIRNAME = "images";

}  // namespace config

#endif  // CONFIG_H