#ifndef THESIS_WORK_SPEED_TESTS_H
#define THESIS_WORK_SPEED_TESTS_H

#include "boost/format.hpp"

#include "generators.h"
#include "timers.h"
#include "output.h"
#include "test_parameters.h"

namespace tests {
    namespace out {
        // Формирует json-файл, в который будет сохранена информация с теста
        // скорости хеширования хеш функций
        OutputJson GetSpeedTestJson(uint16_t hash_bits, size_t num_words, out::Logger& logger);
    }

    // Структура, которая хранит имя хеш-функции
    // и время хеширования
    struct HashSpeed {
        std::string name{};
        double sec_time{};
    };

    // Второй метод подсчета скорости
    template<typename HashFunc>
    double CalculateHashingTime(HashFunc func, std::string_view hash_name, const std::vector<std::string>& words,
                                out::Logger& logger);

    // Тестирования скорости хеширования хеш-функций одной битности
    template<typename HashFunc>
    HashSpeed HashSpeedTest(HashFunc func, std::string_view hash_name, const std::vector<std::string>& words,
                            out::Logger& logger);

    // Функции тестирования скорости хеширования хеш-функций заданной битности
    [[maybe_unused]] boost::json::object SpeedTests16(const std::vector<std::string>& words, out::Logger& logger);
    [[maybe_unused]] boost::json::object SpeedTests24(const std::vector<std::string>& words, out::Logger& logger);
    [[maybe_unused]] boost::json::object SpeedTests32(const std::vector<std::string>& words, out::Logger& logger);
    [[maybe_unused]] boost::json::object SpeedTests48(const std::vector<std::string>& words, out::Logger& logger);
    [[maybe_unused]] boost::json::object SpeedTests64(const std::vector<std::string>& words, out::Logger& logger);

    // Запускает тестирование скорости хеширования хеш-функций
    void RunSpeedTests(uint64_t num_blocks, uint32_t block_length, out::Logger& logger);

    // ====================================

    // Подсчитывает время хеширования
    template<typename Hash>
    double CalculateHashingTime(Hash func, std::string_view hash_name, const std::vector<std::string>& words,
                                out::Logger& logger) {
        auto sum_hashes = static_cast<uint64_t>(func("initial hash"));

        Timer timer1;
        timer1.Start();
        for (const auto & word : words) {
            sum_hashes += static_cast<uint64_t>(func(word));
        }
        timer1.End();

        std::cout << boost::format("\t\tsum hashes: %1%\n") % sum_hashes;
        uint64_t sum_k = 0;

        Timer timer2;
        timer2.Start();
        for (const auto & word : words) {
            sum_k += (++sum_k);
        }
        timer2.End();


        const double total_time = timer1.GetTotalTime() - timer2.GetTotalTime();
        std::cout << boost::format("\t\tsum k: %1%\n") % sum_k;
        logger << boost::format("\t\tsecond timer: %1% sec\n") % total_time;

        return total_time;
    }


    // Тестирования скорости хеширования хеш-функций одной битности
    template<typename HashFunc>
    HashSpeed HashSpeedTest(HashFunc func, std::string_view hash_name, const std::vector<std::string>& words,
                            out::Logger& logger) {
        out::LogDuration log_duration("\t\tlog duration all time", logger);
        logger << boost::format("\n\t%1%:\n") % hash_name;

        // Подсчитывает время хеширования
        const double hashing_time = CalculateHashingTime(func, hash_name, words, logger);

        // Выводит лог
        logger << boost::format("\t\tbest timer: %1% sec\n") % hashing_time;
        // Возвращает результаты теста
        return HashSpeed{std::string{hash_name}, hashing_time};
    }
}

#endif //THESIS_WORK_SPEED_TESTS_H
