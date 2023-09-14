#include "camera.h"
#include <chrono>
#include <filesystem>
#include "config.h"
#include "material.h"

color::Color Camera::RayColor(const Ray& ray, const Hittable& world, int depth) {
    HitRecord hit_record;

    // if the ray bounce limit is exceeded, no more light is gathered
    if (depth <= 0) {
        return {0, 0, 0};
    }

    // see section 9.3 for this interval value
    if (world.Hit(ray, Interval(0.001, config::infinity), hit_record)) {
        Ray scattered{};
        color::Color attenuation{};

        if (hit_record.material_->Scatter(ray, hit_record, attenuation, scattered)) {
            return attenuation * RayColor(scattered, world, depth - 1);
        }

        return color::Color{0, 0, 0};
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
        std::clog << "\r" << config::GetLogPreamble() << "  Scanlines remaining: " << (image_height_ - j)
                  << ' ' << std::flush;

        for (int i = 0; i < image_width_; ++i) {
            color::Color pixel_color{0, 0, 0};

            for (int sample = 0; sample < samples_per_pixel_; ++sample) {
                const Ray ray = GetRay(i, j);
                pixel_color += RayColor(ray, world, max_recursion_depth_);
            }

            color::WriteColor(out_stream, pixel_color, samples_per_pixel_);
        }
    }

    // ugh, avoid ugly formatting
    std::clog << "\rDone.                                           \n";

    const auto end_time = std::chrono::steady_clock::now();
    config::LogDuration(start_time, end_time);
    out_stream.close();
}

void Camera::Init() {
    this->camera_center_ = this->look_from_;
    this->image_height_ = static_cast<int>(image_width_ / aspect_ratio_);

    const auto theta = config::DegreesToRadians(this->vertical_field_of_view_);
    const auto h = tan(theta / 2);

    const auto viewport_height = 2 * h * focus_distance_;
    const auto viewport_width =
        viewport_height * (static_cast<double>(this->image_width_) / this->image_height_);

    this->w_ = UnitVector(this->look_from_ - this->look_at_);
    this->u_ = UnitVector(Cross(this->v_up_, this->w_));
    this->v_ = Cross(this->w_, this->u_);

    // calc vectors across the horizontal and down the vertical viewport edges

    // vector across viewport horizontal edge
    const auto viewport_u = viewport_width * this->u_;

    // vector across viewport vertical edge
    const auto viewport_v = viewport_height * -this->v_;

    // delta_u - distance between pixels on the x-axis
    pixel_delta_u_ = viewport_u / this->image_width_;

    // delta_v - distance between pixels on the y-axis
    pixel_delta_v_ = viewport_v / this->image_height_;

    // location of the upper left pixel - x = 0, y = 0
    const auto viewport_upper_left =
        this->camera_center_ - (this->focus_distance_ * this->w_) - viewport_u / 2 - viewport_v / 2;

    pixel00_loc_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);

    const auto defocus_radius =
        this->focus_distance_ * tan(config::DegreesToRadians(this->defocus_angle_ / 2));

    this->defocus_disk_u_ = defocus_radius * this->u_;
    this->defocus_disk_v_ = defocus_radius * this->v_;
}

Ray Camera::GetRay(const int i, const int j) const {
    // get randomly sampled camera rayfor the pixel at location i,j
    const auto pixel_center = pixel00_loc_ + (i * pixel_delta_u_) + (j * pixel_delta_v_);
    const auto pixel_sample = pixel_center + PixelSampleSquare();

    const auto ray_origin = (this->defocus_angle_ <= 0) ? this->camera_center_ : DefocusDiskSample();
    const auto ray_direction = pixel_sample - ray_origin;

    return {ray_origin, ray_direction};
}

// get a random point in the camera defocus disk
Point3 Camera::DefocusDiskSample() const {
    const auto p = RandomInUnitDisk();
    return this->camera_center_ + (p[0] * this->defocus_disk_u_) + (p[1] * this->defocus_disk_v_);
}

// Returns a random point in the square surrounding a pixel at the origin
Vector3 Camera::PixelSampleSquare() const {
    const auto px = -0.5 + config::GetRandomDouble();
    const auto py = -0.5 + config::GetRandomDouble();
    return (px * pixel_delta_u_) + (py * pixel_delta_v_);
}
