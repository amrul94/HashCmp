#ifndef THESIS_WORK_TIMERS_H
#define THESIS_WORK_TIMERS_H

#include <chrono>
//#include <fstream>
//#include <iostream>


//#include <sys/times.h>

#include "output.h"

namespace tests {
    // Класс таймера, который вычисляет процессорное время
    class Timer {
    public:
        // Начало подсчета времени
        void Start();
        // окончание подсчета времени
        void End();
        // Возвращает затраченное время
        [[nodiscard]] double GetTotalTime() const;

    private:
        // Возвращает текущее процессорное время
        static double GetTime();

        double start_time_ = 0;
        double stop_time_ = 0;
        double total_time_ = 0;
    };

    // Оператор вывода класса Timer. Выводит время в минутах и секундах
    std::ostream& operator<< (std::ostream& out, const Timer& point);

    namespace out {
        // Класс таймера, который вычисляет фактическое время и выводит его в лог
        class LogDuration {
        public:
            using Clock = std::chrono::steady_clock;

            // Конструкторы класса. Конструктор по умолчанию удален
            LogDuration() = delete;
            LogDuration(std::string_view id, Logger& logger);
            LogDuration(const LogDuration&) = default;
            LogDuration(LogDuration&&) = default;

            // Операторы присваивания удалены
            LogDuration& operator=(const LogDuration&) = delete;
            LogDuration& operator=(LogDuration&&) = delete;

            ~LogDuration();

        private:
            // Методы вывода времени
            void PrintMilliseconds(int64_t milliseconds) const;
            void PrintSeconds(int64_t milliseconds) const;
            void PrintMinutes(int64_t milliseconds) const;
            void PrintHours(int64_t milliseconds) const;
            void PrintDays(int64_t milliseconds) const;

            // Константы времени
            static constexpr int64_t milliseconds_per_second = 1000;
            static constexpr int64_t seconds_per_minute = 60;
            static constexpr int64_t minutes_per_hour = 60;
            static constexpr int64_t hours_per_day = 60;

            static constexpr int64_t milliseconds_per_minute = milliseconds_per_second * seconds_per_minute;
            static constexpr int64_t milliseconds_per_hour = milliseconds_per_minute * minutes_per_hour;
            static constexpr int64_t milliseconds_per_day = milliseconds_per_hour * hours_per_day;

            const std::string id_;  // выводимая в лог строка
            const Clock::time_point start_time_ = Clock::now(); // точка отсчета таймера
            Logger& out_;           // Лог, в который осуществляется вывод
        };
    }
}

#endif // THESIS_WORK_TIMERS_H