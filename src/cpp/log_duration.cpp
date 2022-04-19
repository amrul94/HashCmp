#include <cmath>

#include "log_duration.h"
#include "cpu_and_wall_time.h"

void Timer::Start() {
    start_time_ = GetTime();
}

void Timer::End() {
    stop_time_ = GetTime();
    total_time += (stop_time_ - start_time_);
}

double Timer::GetTotalTime() const {
    return total_time;
}

double Timer::GetTime() {
    return get_cpu_time();
}

std::ostream& operator<< (std::ostream& out, const Timer& timer) {
    const double time = timer.GetTotalTime();
    const long round_time = static_cast<long>(std::round(time));

    static const long sec_per_min = 60;
    if (round_time < sec_per_min) {
        out << time << " sec";
    } else {
        out << round_time / sec_per_min << " min ";
        out << round_time % sec_per_min << " sec";
        out << " (" << time << " sec)";
    }

    return out;
}

LogDuration::~LogDuration() {
    using namespace std::chrono;
    using namespace std::literals;

    const auto end_time = Clock::now();
    const auto dur = end_time - start_time_;
    if (duration_cast<milliseconds>(dur).count() < 1000) {
        out_ << id_ << ": "s << duration_cast<milliseconds>(dur).count() << " ms"s << std::endl;
    } else if (duration_cast<seconds>(dur).count() < 60) {
        out_ << id_ << ": "s << duration_cast<seconds>(dur).count() << " sec"s << std::endl;
    } else if (duration_cast<minutes>(dur).count() < 60) {
        out_ << id_ << ": "s << duration_cast<minutes>(dur).count() << " min"s << std::endl;
    } else {
        out_ << id_ << ": "s << duration_cast<hours>(dur).count() << " min"s << std::endl;
    }
}
