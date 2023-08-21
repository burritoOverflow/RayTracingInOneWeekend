#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "color.h"
#include "vec3.h"

// see rationale in section 4.2
const auto ASPECT_RATIO = 16.0 / 9.0;
const int IMAGE_WIDTH = 800;
const int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO);
const auto VIEWPORT_HEIGHT = 2.0;
const auto VIEWPORT_WIDTH = VIEWPORT_HEIGHT * (static_cast<double>(IMAGE_WIDTH) / IMAGE_HEIGHT);

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

int main() {
    CreateImageOutdir();

    const std::string date_fmt_str = "%m-%d-%Y_%H-%M-%S";
    const auto image_out_filename = GetCurrentDateStr(date_fmt_str) + "image.ppm";
    auto image_path = std::filesystem::path(DIRNAME) / image_out_filename;

    std::ofstream out_stream(image_path.string().c_str());
    out_stream << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

    const auto start_time = std::chrono::steady_clock::now();

    for (int j = 0; j < IMAGE_HEIGHT; ++j) {
        std::clog << "\r" << GetLogPreamble() << "  Scanlines remaining: " << (IMAGE_HEIGHT - j)
                  << ' ' << std::flush;

        for (int i = 0; i < IMAGE_WIDTH; ++i) {
            auto pixel_color =
                Color(double(i) / (IMAGE_WIDTH - 1), double(j) / (IMAGE_HEIGHT - 1), 0);
            WriteColor(out_stream, pixel_color);
        }
    }

    // ugh, avoid ugly formatting
    std::clog << "\rDone.                                           \n";

    const auto end_time = std::chrono::steady_clock::now();
    LogDuration(start_time, end_time);

    out_stream.close();
}
