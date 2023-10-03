#ifndef RAYTRACINGINONEWEEKEND_CONFIG_H
#define RAYTRACINGINONEWEEKEND_CONFIG_H

#include <chrono>
#include <random>
#include <string_view>

namespace config {

constexpr std::string_view DIRNAME = "images";

const double infinity = std::numeric_limits<double>::infinity();

const double PI = 3.1415926535897932385;

inline double DegreesToRadians(const double degrees) {
    return degrees * PI / 180.0;
}

std::string GetCurrentDateStr(const std::string& time_fmt_str);

std::string GetLogPreamble();

void CreateImageOutdir();

void LogDuration(std::chrono::steady_clock::time_point start_time,
                 std::chrono::steady_clock::time_point end_time);

static inline double GetRandomDouble(const double lower_bound = 0.0, const double upper_bound = 1.0) {
    static std::uniform_real_distribution<double> distribution{lower_bound, upper_bound};
    static std::random_device dev;
    static std::mt19937 generator(dev());
    return distribution(generator);
}
}  // namespace config

#endif  // RAYTRACINGINONEWEEKEND_CONFIG_H
