#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "color.h"
#include "ray.h"
#include "vec3.h"

// see rationale in section 4.2

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

using time_point = std::chrono::steady_clock::time_point;

void CreateImageOutdir() {
    if (!std::filesystem::exists(DIRNAME)) {
        std::filesystem::create_directory(DIRNAME);
    }
}

std::string GetCurrentDateStr(const std::string& time_fmt_str) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, time_fmt_str.c_str());
    return oss.str();
}

std::string GetLogPreamble() {
    const std::string date_fmt_str = "%m/%d/%Y - %T";
    return GetCurrentDateStr(date_fmt_str);
}

void LogDuration(time_point start_time, time_point end_time) {
    const auto total_time = end_time - start_time;
    const auto elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count();
    const auto elapsed_s = std::chrono::duration_cast<std::chrono::seconds>(total_time).count();
    std::clog << "\rDuration: " << elapsed_s << "s " << elapsed_ms << " ms "
              << "\n";
}

Color RayColor(Ray& ray) {
    // linearly blend white and blue, depending on height of the y-coord
    const Vector3 unit_direction = UnitVector(ray.direction());

    // a from 0 - 1
    const auto a = 0.5 * (unit_direction.y() + 1.0);

    // lerp of the form: blendedValue = (1 - a) * startValue + a * endValue
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main() {
    CreateImageOutdir();

    const std::string date_fmt_str = "%m-%d-%Y_%H-%M-%S";
    const auto image_out_filename = GetCurrentDateStr(std::move(date_fmt_str)) + "image.ppm";
    auto image_path = std::filesystem::path(DIRNAME) / image_out_filename;

    std::ofstream out_stream(image_path.string().c_str());
    out_stream << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

    const auto start_time = std::chrono::steady_clock::now();

    for (int j = 0; j < IMAGE_HEIGHT; ++j) {
        std::clog << "\r" << GetLogPreamble() << "  Scanlines remaining: " << (IMAGE_HEIGHT - j)
                  << ' ' << std::flush;

        for (int i = 0; i < IMAGE_WIDTH; ++i) {
            const auto pixel_center = PIXEL00_LOC + (i * PIXEL_DELTA_U) + (j * PIXEL_DELTA_V);
            const auto ray_direction = pixel_center - CAMERA_CENTER;

            Ray ray{CAMERA_CENTER, ray_direction};
            Color pixel_color = RayColor(ray);

            WriteColor(out_stream, pixel_color);
        }
    }

    // ugh, avoid ugly formatting
    std::clog << "\rDone.                                           \n";

    const auto end_time = std::chrono::steady_clock::now();
    LogDuration(start_time, end_time);

    out_stream.close();
}
