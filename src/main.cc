#include <chrono>
#include <filesystem>
#include <iostream>
#include "color.h"
#include "config.h"
#include "hittable.h"
#include "hittable_list.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

using namespace std::chrono;

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

void LogDuration(steady_clock::time_point start_time, steady_clock::time_point end_time) {
    const auto total_time = end_time - start_time;
    const auto elapsed_ms = duration_cast<milliseconds>(total_time).count();
    const auto elapsed_s = duration_cast<seconds>(total_time).count();

    std::clog << "\rDuration: " << elapsed_s << "s " << elapsed_ms << " ms "
              << "\n";
}

Color RayColor(Ray& ray, const Hittable &world) {
    HitRecord hit_record;
    if (world.Hit(ray, 0, config::infinity, hit_record)) {
        return 0.5 * (hit_record.normal_ + Color(1,1,1));
    }

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
    const auto image_out_filename = GetCurrentDateStr(date_fmt_str) + "image.ppm";
    auto image_path = std::filesystem::path(config::DIRNAME) / image_out_filename;

    std::ofstream out_stream(image_path.string().c_str());
    out_stream << "P3\n" << config::IMAGE_WIDTH << ' ' << config::IMAGE_HEIGHT << "\n255\n";

    const auto start_time = steady_clock::now();

    // add a "world"
    HittableList world;
    world.AddObject(std::make_shared<Sphere>(Point3(0,0,-1), 0.5));
    world.AddObject(std::make_shared<Sphere>(Point3(0,-100.5,-1), 100));

    for (int j = 0; j < config::IMAGE_HEIGHT; ++j) {
        std::clog << "\r" << GetLogPreamble()
                  << "  Scanlines remaining: " << (config::IMAGE_HEIGHT - j) << ' ' << std::flush;

        for (int i = 0; i < config::IMAGE_WIDTH; ++i) {
            const auto pixel_center =
                config::PIXEL00_LOC + (i * config::PIXEL_DELTA_U) + (j * config::PIXEL_DELTA_V);
            const auto ray_direction = pixel_center - config::CAMERA_CENTER;

            Ray ray{config::CAMERA_CENTER, ray_direction};
            const Color pixel_color = RayColor(ray, world);

            WriteColor(out_stream, pixel_color);
        }
    }

    // ugh, avoid ugly formatting
    std::clog << "\rDone.                                           \n";

    const auto end_time = steady_clock::now();
    LogDuration(start_time, end_time);

    out_stream.close();
}
