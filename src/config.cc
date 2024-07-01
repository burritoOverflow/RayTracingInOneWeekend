#include "config.h"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
#if defined(__APPLE__)
#include <sstream>
#endif

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

// format a string for logging of the elapsed duration hh:mm:ss
std::string config::GetElapsedTime() {
    const auto fmt_value = [](long value) {
        return value < 10 ? "0" + std::to_string(value) : std::to_string(value);
    };

    const std::string hours = fmt_value(config::measured_duration.hours_);
    const std::string minutes = fmt_value(config::measured_duration.minutes_);
    const std::string seconds = fmt_value(config::measured_duration.seconds_);

    return "Elapsed time: " + hours + ":" + minutes + ":" + seconds;
}

void config::CreateImageOutdir() {
    if (!std::filesystem::exists(config::DIRNAME)) {
        std::filesystem::create_directory(config::DIRNAME);
    }
}

// update elapsed duration once per second
void config::IncrementElapsedTime() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        config::measured_duration.seconds_++;
        const auto MAX = 60;
        if (config::measured_duration.seconds_ == MAX) {
            config::measured_duration.seconds_ = 0;
            config::measured_duration.minutes_++;
        }

        if (config::measured_duration.minutes_ == MAX) {
            config::measured_duration.minutes_ = 0;
            config::measured_duration.hours_++;
        }
    }
}

void config::LogDuration(const std::chrono::steady_clock::time_point start_time,
                         const std::chrono::steady_clock::time_point end_time) {
    const auto total_time = end_time - start_time;

    const auto elapsed_min = std::chrono::duration_cast<std::chrono::minutes>(total_time).count();
    const auto elapsed_s = std::chrono::duration_cast<std::chrono::seconds>(total_time).count();
    const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count();

    long output_ms;
    long output_s = elapsed_s;

    if (elapsed_min > 0) {
        output_s -= elapsed_min * 60;
    }

    if (elapsed_s > 1l) {
        output_ms = elapsed_ms % (elapsed_s * static_cast<long>(pow(10, 3)));
    } else {
        output_ms = elapsed_ms;
    }

    std::clog << "\rDuration: " << elapsed_min << "m " << output_s << "s " << output_ms << " ms "
              << "\n";
}
