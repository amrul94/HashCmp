#ifndef THESIS_WORK_CHECK_DISTRIBUTION_H
#define THESIS_WORK_CHECK_DISTRIBUTION_H

#include <atomic>
#include <concepts>
#include <iostream>
#include <string>
#include <thread>
#include <vector>


#include <boost/format.hpp>

#include "concurrency.h"
#include "hashes.h"
#include "output.h"
#include "test_parameters.h"
#include "timers.h"

namespace tests {
    using Bucket = std::uint16_t;

    namespace out {
        /*
         * Сохраняет результаты тестов в json файл
         *  Входные параметры:
         *      1. buckets - массив счетчиков хеш-значений
         *      2. parameters - параметры тестирования:
         *          - битность хеша (16, 32 или 64)
         *          - число потоков (зависит от системы)
         *          - число ключей (целое положительное число)
         *          - размер массива buckets
         *          - флаг тестирования: NORMAL или BINS
         *      3. hash_name - название хеш функции
         *      4. logger - записывает лог в файл и выводит его на консоль
         */
        void SaveReport(const std::vector<std::atomic<Bucket>>& buckets, const DistTestParameters& parameters,
                        const std::string& hash_name, out::Logger& logger);
    }

    // Класс, в котором хранятся хеш-значения
    class DistributionHashes {
    public:
        static constexpr Bucket max_bucket = std::numeric_limits<Bucket>::max();

        explicit DistributionHashes(size_t num_buckets);

        // Добавляет хеш-значение
        void AddHash(uint64_t hash_value);

        // Возвращает вектор хеш-значений
        [[nodiscard]] const std::vector<std::atomic<Bucket>>& GetBuckets() const;

    private:
        std::vector<std::atomic<Bucket>> buckets_; // Массив счетчиков хеш-значений
    };


    /*
     * Тестирование распределительных свойств одной хеш функции. Реализация описана ниже
     *      1. hash - хеш-функция
     *      2. parameters - параметры тестирования:
     *          - битность хеша (16, 32 или 64)
     *          - число ключей (целое положительное число)
     *          - число потоков (зависит от системы)
     *          - размер массива счетчиков хеш-значений
     *          - флаг тестирования: NORMAL или BINS
     *      3. logger - записывает лог в файл и выводит его на консоль
     */
    template<hfl::UnsignedIntegral UintT>
    void HashDistributionTest(const hfl::Hash<UintT>& hash, const DistTestParameters& parameters, out::Logger& logger);

    /*
     * Тестирование распределительных свойств хеш функций. Реализация описана ниже
     *  Параметр шаблона: целое беззнаковое число - тип хеш-значения
     *  Входные параметры:
     *      1. hashes - массив со всеми хеш-функциями одной битности
     *      2. parameters - параметры тестирования:
     *          - битность хеша (16, 32 или 64)
     *          - число потоков (зависит от системы)
     *          - число ключей (целое положительное число)
     *          - размер массива счетчиков хеш-значений
     *          - флаг тестирования: NORMAL или BINS
     *      3. logger - записывает лог в файл и выводит его на консоль
     */
    template<hfl::UnsignedIntegral UintT>
    void DistributionTest(const std::vector<hfl::Hash<UintT>>& hashes, const DistTestParameters& parameters,
                          out::Logger& logger);

    //  Запускает тестирование распределительных свойств всех хеш функций
    //  Входной параметр: logger - записывает лог в файл и выводит его на консоль
    void RunDistributionTests(out::Logger& logger);

// ==================================================

    // Тестирование распределительных свойств одной хеш функции
    template<hfl::UnsignedIntegral UintT>
    void HashDistributionTest(const hfl::Hash<UintT>& hash, const DistTestParameters& parameters, out::Logger& logger) {
        out::LogDuration log_duration("\t\ttime", logger);
        logger << boost::format("\t%1%: \n") % hash.GetName();

        DistributionHashes distribution_hashes{parameters.num_buckets};

        // Функция, которая непосредственно тестирует хеш функцию.
        // Запускается в отдельном потоке
        auto lambda = [&hash, &parameters, &distribution_hashes](uint64_t start, uint64_t end) {
            for (uint64_t number = start; number < end; ++number) {
                const uint64_t hash_value = hash(number);
                const uint64_t modified_value = ModifyHash(parameters, hash_value);
                distribution_hashes.AddHash(modified_value);
            }
        };

        // Запуск теста в разных потоках
        ThreadTasks<void> thread_tasks(lambda, parameters.num_threads, parameters.num_keys);
        // Сохранение результатов тестирования
        out::SaveReport(distribution_hashes.GetBuckets(), parameters, hash.GetName(), logger);
    }

    // Тестирование распределительных свойств хеш функций
    template<hfl::UnsignedIntegral UintT>
    void DistributionTest(const std::vector<hfl::Hash<UintT>>& hashes, const DistTestParameters& parameters,
                          out::Logger& logger) {
        // В цикле запускает тесты для всех хеш-функций
        out::StartAndEndLogBitsTest log(logger, parameters.hash_bits);
        for (const hfl::Hash<UintT>& hash : hashes) {
            HashDistributionTest(hash, parameters, logger);
        }
    }
}

#endif //THESIS_WORK_CHECK_DISTRIBUTION_H
