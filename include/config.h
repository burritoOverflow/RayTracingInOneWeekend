#ifndef CONFIG_H
#define CONFIG_H

#include <chrono>
#include <string_view>
#include "color.h"

// see rationale for these constants in section 4.2

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

}  // namespace config

#endif  // CONFIG_H