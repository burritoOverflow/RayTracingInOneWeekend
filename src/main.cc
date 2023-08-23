#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "color.h"
#include "config.h"
#include "ray.h"
#include "vec3.h"

using time_point = std::chrono::steady_clock::time_point;

void CreateImageOutdir() {
    if (!std::filesystem::exists(config::DIRNAME)) {
        std::filesystem::create_directory(config::DIRNAME);
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

    // a from 0 to 1
    const auto a = 0.5 * (unit_direction.y() + 1.0);

    // lerp of the form: blendedValue = (1 - a) * startValue + a * endValue
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main() {
    CreateImageOutdir();

    const std::string date_fmt_str = "%m-%d-%Y_%H-%M-%S";
    const auto image_out_filename = GetCurrentDateStr(std::move(date_fmt_str)) + "image.ppm";
    auto image_path = std::filesystem::path(config::DIRNAME) / image_out_filename;

    std::ofstream out_stream(image_path.string().c_str());
    out_stream << "P3\n" << config::IMAGE_WIDTH << ' ' << config::IMAGE_HEIGHT << "\n255\n";

    const auto start_time = std::chrono::steady_clock::now();

    for (int j = 0; j < config::IMAGE_HEIGHT; ++j) {
        std::clog << "\r" << GetLogPreamble()
                  << "  Scanlines remaining: " << (config::IMAGE_HEIGHT - j) << ' ' << std::flush;

        for (int i = 0; i < config::IMAGE_WIDTH; ++i) {
            const auto pixel_center =
                config::PIXEL00_LOC + (i * config::PIXEL_DELTA_U) + (j * config::PIXEL_DELTA_V);
            const auto ray_direction = pixel_center - config::CAMERA_CENTER;

            Ray ray{config::CAMERA_CENTER, ray_direction};
            const Color pixel_color = RayColor(ray);

            WriteColor(out_stream, pixel_color);
        }
    }

    // ugh, avoid ugly formatting
    std::clog << "\rDone.                                           \n";

    const auto end_time = std::chrono::steady_clock::now();
    LogDuration(start_time, end_time);

    out_stream.close();
}
