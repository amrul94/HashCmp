#include <cmath>

#include "timers.h"
#include "cpu_and_wall_time.h"

namespace tests {
    // Начало подсчета времени
    void Timer::Start() {
        start_time_ = GetTime();
    }

    // окончание подсчета времени
    void Timer::End() {
        stop_time_ = GetTime();
        total_time_ += (stop_time_ - start_time_);
    }

    // Возвращает затраченное время (total_time_)
    double Timer::GetTotalTime() const {
        return total_time_;
    }

    // Возвращает текущее процессорное время
    double Timer::GetTime() {
        return get_cpu_time();
    }

    // Оператор вывода класса Timer. Выводит время в минутах и секундах
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
        // Конструктор класса LogDuration
        LogDuration::LogDuration(std::string_view id, Logger& logger)
                : id_(id)
                , out_(logger) {
        }

        // Деструктор класса LogDuration.
        // Подсчитывает затраченное время и выводит его в лог
        LogDuration::~LogDuration() {
            using namespace std::chrono;
            using namespace std::literals;

            const auto end_time = Clock::now();
            const auto dur = end_time - start_time_;

            out_ << id_ << ": "s;

            // Затраченное время в секундах
            const int64_t time_in_milliseconds = duration_cast<milliseconds>(dur).count();

            // Вывод времени в лог
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

        // Вывод времени в миллисекундах
        void LogDuration::PrintMilliseconds(int64_t milliseconds) const {
            out_ << milliseconds % milliseconds_per_second << " ms\n\n";
        }

        // Вывод времени в секундах
        void LogDuration::PrintSeconds(int64_t milliseconds) const {
            out_ << milliseconds / milliseconds_per_second << " sec ";
            PrintMilliseconds(milliseconds % milliseconds_per_second);
        }

        // Вывод времени в минутах
        void LogDuration::PrintMinutes(int64_t milliseconds) const {
            out_ << milliseconds / milliseconds_per_minute << " min ";
            PrintSeconds(milliseconds % milliseconds_per_minute);
        }

        // Вывод времени в часах
        void LogDuration::PrintHours(int64_t milliseconds) const {
            out_ << milliseconds / milliseconds_per_hour << " h ";
            PrintMinutes(milliseconds % milliseconds_per_hour);
        }

        // Вывод времени в днях
        void LogDuration::PrintDays(int64_t milliseconds) const {
            out_ << milliseconds / milliseconds_per_day << " d ";
            PrintHours(milliseconds % milliseconds_per_day);
        }
    }
}