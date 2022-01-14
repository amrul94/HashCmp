#include <cmath>

#include "log_duration.h"

void Timer::Start() {
    start_time_ = GetTime();
}

void Timer::End() {
    stop_time_ = GetTime();
    total_time += (stop_time_ - start_time_);
}

double Timer::GetTotalTime() const {
    return static_cast<double>(total_time / sc_clk_tck_);
}

clock_t Timer::GetTime() {
    return times(&tms_time_);
}

std::ostream& operator<< (std::ostream& out, const Timer& timer) {
    static double sec_per_min = 60;
    double time = timer.GetTotalTime();

    if (time < sec_per_min) {
        out << time << " sec";
    } else {
        out << std::round(time) / sec_per_min << " min";
    }

    return out;
}

LogDuration::LogDuration(std::string_view id, std::ostream &out)
    : id_(id), out_(out) {
    timer_.Start();
}

LogDuration::~LogDuration() {
    timer_.End();
    out_ << id_ << ": " << timer_ << std::endl;
}
