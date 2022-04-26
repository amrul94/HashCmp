#ifndef THESIS_WORK_LOG_DURATION_H
#define THESIS_WORK_LOG_DURATION_H

#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>


#include <sys/times.h>

#include "output.h"

namespace tests {
    class Timer {
    public:
        void Start();
        void End();
        [[nodiscard]] double GetTotalTime() const;

    private:
        static double GetTime();

        double start_time_ = 0;
        double stop_time_ = 0;
        double total_time = 0;
    };

    std::ostream& operator<< (std::ostream& out, const Timer& point);

    namespace out {
        class LogDuration {
        public:
            using Clock = std::chrono::steady_clock;

            LogDuration() = delete;
            LogDuration(std::string_view id, Logger& logger);
            LogDuration(const LogDuration&) = default;
            LogDuration(LogDuration&&) = default;

            LogDuration& operator=(const LogDuration&) = delete;
            LogDuration& operator=(LogDuration&&) = delete;

            ~LogDuration();

        private:
            void PrintMilliseconds(int64_t milliseconds) const;
            void PrintSeconds(int64_t milliseconds) const;
            void PrintMinutes(int64_t milliseconds) const;
            void PrintHours(int64_t milliseconds) const;
            void PrintDays(int64_t milliseconds) const;

            static constexpr int64_t milliseconds_per_second = 1000;
            static constexpr int64_t seconds_per_minute = 60;
            static constexpr int64_t minutes_per_hour = 60;
            static constexpr int64_t hours_per_day = 60;

            static constexpr int64_t milliseconds_per_minute = milliseconds_per_second * seconds_per_minute;
            static constexpr int64_t milliseconds_per_hour = milliseconds_per_minute * minutes_per_hour;
            static constexpr int64_t milliseconds_per_day = milliseconds_per_hour * hours_per_day;

            const std::string id_;
            const Clock::time_point start_time_ = Clock::now();
            Logger& out_;
        };
    }
}


#endif // THESIS_WORK_LOG_DURATION_H