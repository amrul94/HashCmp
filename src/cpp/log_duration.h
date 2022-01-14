#ifndef THESIS_WORK_LOG_DURATION_H
#define THESIS_WORK_LOG_DURATION_H

#include <atomic>
#include <chrono>
#include <iostream>
#include <utility>


#include <sys/times.h>

class Timer {
public:
    void Start();
    void End();
    [[nodiscard]] double GetTotalTime() const;

private:
    clock_t GetTime();

    tms tms_time_{};
    clock_t start_time_ = 0;
    clock_t stop_time_ = 0;
    clock_t total_time = 0;
    const long double sc_clk_tck_ = static_cast<long double>(sysconf(_SC_CLK_TCK));
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

    explicit LogDuration(std::string_view id, std::ostream& out = std::cerr);

    ~LogDuration();

private:
    const std::string id_;
    Timer timer_;
    std::ostream& out_;
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