#include "camera.h"
#include <chrono>
#include <filesystem>
#include "config.h"

color::Color Camera::RayColor(Ray& ray, const Hittable& world) {
    HitRecord hit_record;
    if (world.Hit(ray, Interval(0, config::infinity), hit_record)) {
        return 0.5 * (hit_record.normal_ + color::Color(1, 1, 1));
    }

    // linearly blend white and blue, depending on height of the y-coord
    const Vector3 unit_direction = UnitVector(ray.direction());

    // a from 0 to 1
    const auto a = 0.5 * (unit_direction.y() + 1.0);

    // lerp of the form: blendedValue = (1 - a) * startValue + a * endValue
    return (1.0 - a) * color::Color(1.0, 1.0, 1.0) + a * color::Color(0.5, 0.7, 1.0);
}

void Camera::Render(const Hittable& world) {
    Init();
    config::CreateImageOutdir();

    const std::string date_fmt_str = "%m-%d-%Y_%H-%M-%S";
    const auto image_out_filename = config::GetCurrentDateStr(date_fmt_str) + "image.ppm";
    auto image_path = std::filesystem::path(config::DIRNAME) / image_out_filename;

    std::ofstream out_stream(image_path.string().c_str());
    out_stream << "P3\n" << image_width_ << ' ' << image_height_ << "\n255\n";
    const auto start_time = std::chrono::steady_clock::now();

    for (int j = 0; j < image_height_; ++j) {
        std::clog << "\r" << config::GetLogPreamble()
                  << "  Scanlines remaining: " << (image_height_ - j) << ' ' << std::flush;

        for (int i = 0; i < image_width_; ++i) {
            const auto pixel_center = pixel00_loc_ + (i * pixel_delta_u_) + (j * pixel_delta_v_);
            const auto ray_direction = pixel_center - camera_center_;

            Ray ray{camera_center_, ray_direction};
            const color::Color pixel_color = RayColor(ray, world);
            color::WriteColor(out_stream, pixel_color);
        }
    }

    // ugh, avoid ugly formatting
    std::clog << "\rDone.                                           \n";

    const auto end_time = std::chrono::steady_clock::now();
    config::LogDuration(start_time, end_time);
    out_stream.close();
}

void Camera::Init() {
    image_height_ = static_cast<int>(image_width_ / aspect_ratio_);

    // viewport dimension determination
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width_) / image_height_);
    auto focal_length = 1.0;

    // v_u - vector from the left to the right edge
    const auto viewport_u = Vector3(viewport_width, 0, 0);

    // v_v - vector from the upper edge to the lower edge
    // y increases down the image
    const auto viewport_v = Vector3(0, -viewport_height, 0);

    // delta_u - distance between pixels on the x-axis
    pixel_delta_u_ = viewport_u / image_width_;

    // delta_v - distance between pixels on the y-axis
    pixel_delta_v_ = viewport_v / image_height_;

    // location of the upper left pixel - x = 0, y = 0
    const auto viewport_upper_left =
        camera_center_ - Vector3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;

    pixel00_loc_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);
}
