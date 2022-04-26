#include <cmath>

#include "timers.h"
#include "cpu_and_wall_time.h"

namespace tests {
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

    namespace out {

        LogDuration::LogDuration(std::string_view id, Logger& logger)
                : id_(id)
                , out_(logger) {
        }

        LogDuration::~LogDuration() {
            using namespace std::chrono;
            using namespace std::literals;

            const auto end_time = Clock::now();
            const auto dur = end_time - start_time_;

            out_ << id_ << ": "s;

            const int64_t time_in_milliseconds = duration_cast<milliseconds>(dur).count();

            if (time_in_milliseconds < milliseconds_per_second) {
                PrintMilliseconds(time_in_milliseconds);
            } else if (duration_cast<seconds>(dur).count() < seconds_per_minute) {
                PrintSeconds(time_in_milliseconds);
            } else if (duration_cast<minutes>(dur).count() < minutes_per_hour) {
                PrintMinutes(time_in_milliseconds);
            } else if (duration_cast<hours>(dur).count() < hours_per_day) {
                PrintHours(time_in_milliseconds);
            } else {
                PrintDays(time_in_milliseconds);
            }
        }

        void LogDuration::PrintMilliseconds(int64_t milliseconds) const {
            out_ << milliseconds % milliseconds_per_second << " ms\n\n";
        }

        void LogDuration::PrintSeconds(int64_t milliseconds) const {
            out_ << milliseconds / milliseconds_per_second << " sec ";
            PrintMilliseconds(milliseconds % milliseconds_per_second);
        }

        void LogDuration::PrintMinutes(int64_t milliseconds) const {
            out_ << milliseconds / milliseconds_per_minute << " min ";
            PrintSeconds(milliseconds % milliseconds_per_minute);
        }

        void LogDuration::PrintHours(int64_t milliseconds) const {
            out_ << milliseconds / milliseconds_per_hour << " h ";
            PrintMinutes(milliseconds % milliseconds_per_hour);
        }

        void LogDuration::PrintDays(int64_t milliseconds) const {
            out_ << milliseconds / milliseconds_per_day << " d ";
            PrintHours(milliseconds % milliseconds_per_day);
        }
    }
}