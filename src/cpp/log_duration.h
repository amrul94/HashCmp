#ifndef THESIS_WORK_LOG_DURATION_H
#define THESIS_WORK_LOG_DURATION_H

#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>


#include <sys/times.h>

#include "logger.h"

class Timer {
public:
    void Start();
    void End();
    [[nodiscard]] double GetTotalTime() const;

private:
    static double GetTime();

    tms tms_time_{};
    double start_time_ = 0;
    double stop_time_ = 0;
    double total_time = 0;
};

std::ostream& operator<< (std::ostream& out, const Timer& point);

#define PROFILE_CONCAT_INTERNAL(X, Y) X##Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, __LINE__)
#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x)
#define LOG_DURATION_STREAM(x, y) LogDuration UNIQUE_VAR_NAME_PROFILE(x, y)

class LogDuration {
public:
    using Clock = std::chrono::steady_clock;

    explicit LogDuration(std::string_view id, Logger& out)
            : id_(id), out_(out) {
    }

    ~LogDuration();

private:
    const std::string id_;
    const Clock::time_point start_time_ = Clock::now();
    Logger& out_;
};

inline std::string CurrentTime() {
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::string ts = std::ctime(&t);
    ts.resize(ts.size() - 1);
    ts.replace(ts.find(':'), 1, ".");
    ts.replace(ts.find(':'), 1, ".");
    return ts;
}

#endif // THESIS_WORK_LOG_DURATION_H