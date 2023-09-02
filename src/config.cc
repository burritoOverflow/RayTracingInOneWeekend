#include "config.h"
#include <filesystem>
#include <string>

std::string config::GetCurrentDateStr(const std::string& time_fmt_str) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, time_fmt_str.c_str());
    return oss.str();
}

std::string config::GetLogPreamble() {
    const std::string date_fmt_str = "%m/%d/%Y - %T";
    return GetCurrentDateStr(date_fmt_str);
}

 void config::CreateImageOutdir() {
    if (!std::filesystem::exists(config::DIRNAME)) {
        std::filesystem::create_directory(config::DIRNAME);
    }
}

void config::LogDuration(std::chrono::steady_clock::time_point start_time,
                 std::chrono::steady_clock::time_point end_time) {
    const auto total_time = end_time - start_time;
    const auto elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count();
    const auto elapsed_s = std::chrono::duration_cast<std::chrono::seconds>(total_time).count();

    std::clog << "\rDuration: " << elapsed_s << "s " << elapsed_ms << " ms "
              << "\n";
}
