#ifndef THESIS_WORK_SPEED_TESTS_H
#define THESIS_WORK_SPEED_TESTS_H

#include "boost/format.hpp"

#include "generators.h"
#include "timers.h"
#include "output.h"
#include "test_parameters.h"

namespace tests {
    namespace out {
        /*
         *  Формирует json-файл, в который будет сохранена информация с теста
         *  скорости хеширования хеш функций
         *  Входные параметры:
         *      1. hash_bits - число битов хеш-значения
         *      2. num_words - число хешируемых блоков (слов)
         *      3. logger - записывает лог в файл и выводит его на консоль
         */
        OutputJson GetSpeedTestJson(uint16_t hash_bits, size_t num_words, out::Logger& logger);
    }

    // Структура, которая хранит имя хеш-функции
    // и время хеширования
    struct HashSpeed {
        std::string name{};
        double sec_time{};
    };

    /*
     *  Подсчитывает время хеширования
     *  Параметр шаблона: хеш функция
     *  Входные параметры:
     *      1. hash - хеш-функция
     *      2. hash_name - название хеш функции
     *      3. words - массив хешируемых слов
     *      4. logger - записывает лог в файл и выводит его на консоль
     *  Возвращаемое значение: время хеширования
    */
    template<typename Hash>
    double CalculateHashingTime(Hash hash, std::string_view hash_name, const std::vector<std::string>& words,
                                out::Logger& logger);

    /*
     *  Тестирования скорости хеширования хеш-функций одной битности
     *  Параметр шаблона: хеш функция
     *  Входные параметры:
     *      1. hash - хеш-функция
     *      2. hash_name - название хеш функции
     *      3. words - массив хешируемых слов
     *      4. logger - записывает лог в файл и выводит его на консоль
     *  Возвращаемое значение: название хеш функции и время хеширования
     */
    template<typename Hash>
    HashSpeed HashSpeedTest(Hash hash, std::string_view hash_name, const std::vector<std::string>& words,
                            out::Logger& logger);

    /*
     *  Функции тестирования скорости хеширования хеш-функций заданной битности
     *  Входные параметры:
     *      1. words - массив хешируемых слов
     *      2. logger - записывает лог в файл и выводит его на консоль
     *  Возвращаемое значение: json-структура, в которую записываются результаты теста
     */
    [[maybe_unused]] boost::json::object SpeedTests16(const std::vector<std::string>& words, out::Logger& logger);
    [[maybe_unused]] boost::json::object SpeedTests24(const std::vector<std::string>& words, out::Logger& logger);
    [[maybe_unused]] boost::json::object SpeedTests32(const std::vector<std::string>& words, out::Logger& logger);
    [[maybe_unused]] boost::json::object SpeedTests48(const std::vector<std::string>& words, out::Logger& logger);
    [[maybe_unused]] boost::json::object SpeedTests64(const std::vector<std::string>& words, out::Logger& logger);

    /*
     *  Запускает тестирование скорости хеширования хеш-функций
     *  Входные параметры:
     *      1. num_blocks - число хешируемых блоков данных
     *      2. block_length - размер блока данных
     *      3. logger - записывает лог в файл и выводит его на консоль
     */
    void RunSpeedTests(uint64_t num_blocks, uint32_t block_length, out::Logger& logger);

// ====================================================================================

    // Подсчитывает время хеширования
    template<typename Hash>
    double CalculateHashingTime(Hash hash, std::string_view hash_name, const std::vector<std::string>& words,
                                out::Logger& logger) {
        auto sum_hashes = static_cast<uint64_t>(hash("initial hash"));

        Timer timer1;
        timer1.Start();
        for (const auto & word : words) {
            sum_hashes += static_cast<uint64_t>(hash(word));
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
    template<typename Hash>
    HashSpeed HashSpeedTest(Hash hash, std::string_view hash_name, const std::vector<std::string>& words,
                            out::Logger& logger) {
        out::LogDuration log_duration("\t\tlog duration all time", logger);
        logger << boost::format("\n\t%1%:\n") % hash_name;

        // Подсчитывает время хеширования
        const double hashing_time = CalculateHashingTime(hash, hash_name, words, logger);

        // Выводит лог
        logger << boost::format("\t\tbest timer: %1% sec\n") % hashing_time;
        // Возвращает результаты теста
        return HashSpeed{std::string{hash_name}, hashing_time};
    }
}

#endif //THESIS_WORK_SPEED_TESTS_H
