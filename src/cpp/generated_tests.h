#ifndef THESIS_WORK_WORDS_TESTS_H
#define THESIS_WORK_WORDS_TESTS_H

#include <barrier>
#include <deque>
#include <fstream>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

#include <boost/format.hpp>

#include <pcg_random.hpp>

#include "concurrency.h"
#include "generators.h"
#include "hashes.h"
#include "output.h"
#include "timers.h"

#include "test_parameters.h"

namespace tests {
    namespace out {
        /*
         *  Формирует json-файл, в который будет сохранена информация с теста хеш функции
         *  на устойчивости к коллизиям
         *  Входные параметры:
         *      1. parameters - параметры тестирования:
         *          - битность хеша (16, 24, 32, 48 или 64)
         *          - битность маски (16, 24 или 32)
         *          - число потоков (зависит от системы)
         *          - число хешируемых блоков (целое положительное число)
         *          - размер блока
         *          - флаг тестирования (NORMAL или MASK)
         *      2. logger - записывает лог в файл и выводит его на консоль
         */
        OutputJson GetGenTestJson(const GenBlocksParameters& parameters, out::Logger& logger);
    }

    /*
     *  Тестирование устойчивости к коллизиям одной хеш функции. Реализация описана ниже
     *  Параметр шаблона: целое беззнаковое число - тип хеш-значения
     *  Входные параметры:
     *      1. hash - хеш-функция
     *      2. parameters - параметры тестирования:
     *          - битность хеша (16, 24, 32, 48 или 64)
     *          - битность маски (16, 24 или 32)
     *          - число потоков (зависит от системы)
     *          - число хешируемых блоков (целое положительное число)
     *          - размер блока
     *          - флаг тестирования (NORMAL или MASK)
     *      3. logger - записывает лог в файл и выводит его на консоль
     *  Выходное значение: пара название хеш функции и число коллизий
     */
    template<hfl::UnsignedIntegral UintT>
    auto HashTestWithGenBlocks(const hfl::Hash<UintT>& hash,
                               const GenBlocksParameters& parameters, out::Logger& logger);

    /*
     *  Тестирование устойчивости к коллизиям хеш функций. Реализация описана ниже
     *  Параметр шаблона: целое беззнаковое число - тип хеш-значения
     *  Входные параметры:
     *      1. hashes - массив со всеми хеш-функциями одной битности
     *      2. words - массив хешируемых слов
     *      3. parameters - параметры тестирования:
     *          - битность хеша (16, 24, 32, 48 или 64)
     *          - битность маски (16, 24 или 32)
     *          - число потоков (зависит от системы)
     *          - число хешируемых блоков (целое положительное число)
     *          - размер блока
     *          - флаг тестирования (NORMAL или MASK)
     *      4. logger - записывает лог в файл и выводит его на консоль
     */
    template<hfl::UnsignedIntegral UintT>
    void TestWithGeneratedBlocks(const std::vector<hfl::Hash<UintT>>& hashes,
                                 const GenBlocksParameters& parameters, out::Logger& logger);

    // Запуск тестирования устойчивости к коллизиям хеш функций
    void RunTestWithGeneratedBlocks(uint16_t words_length, out::Logger& logger);

    // ===============================================================================

    // Тестирование устойчивости к коллизиям одной хеш функции
    template<hfl::UnsignedIntegral UintT>
    auto HashTestWithGenBlocks(const hfl::Hash<UintT>& hash, const GenBlocksParameters& parameters,
                               out::Logger& logger) {
        out::LogDuration log_duration("\t\ttime", logger);
        logger << boost::format("\n\t%1%: \n") % hash.GetName();

        // Выдает генераторы, число которых равно числу запускаемых потоков
        std::uint64_t num_generated_numbers = (parameters.num_keys * parameters.words_length) / 8;
        std::vector<pcg64> generators = GetGenerators(parameters.num_threads, num_generated_numbers);   \

        boost::json::object collisions;
        std::atomic_uint64_t num_collisions = 0;

        size_t num_words = (1 << (parameters.mask_bits >> 1));
        size_t step = (parameters.mask_bits != 24) ? 1 : 2;

        // Условие завершения внешнего цикла в thread_task (описан ниже)
        bool loop_conditional = (num_words <= parameters.num_keys);
        // Функция, запускаемая барьером sync_point (описан ниже) после выполнения
        // внутреннего цикла в thread_task
        auto loop_completion_task =
                [&logger, &num_words, &num_collisions, &collisions, &loop_conditional, 
                 &parameters, step] () noexcept{
            logger << boost::format("\t\t%1% words:\t%2% collisions\n") % num_words % num_collisions;
            std::string index = std::to_string(num_words);
            collisions[index] = num_collisions;
            num_words <<= step;
            loop_conditional = (num_words <= parameters.num_keys);
        };

        const uint64_t num_hashes = 1ull << parameters.mask_bits;
        std::deque<std::atomic_bool> coll_flags(num_hashes);

        std::atomic_size_t gen_index = 0;
        std::barrier sync_point(parameters.num_threads, loop_completion_task);

        // Функция, запускаемая в отдельном потоке. Подсчитывает число коллизий
        auto thread_task = [&loop_conditional, &num_words, &parameters, &generators, &gen_index, &hash, &coll_flags,
                            &num_collisions, &sync_point] () {
            pcg64& rng = generators[gen_index++];
            for (size_t i = 0; loop_conditional; ) {
                size_t thread_num_words = num_words / parameters.num_threads;
                for (; i < thread_num_words; ++i) {
                    std::string str = GenerateRandomDataBlock(rng, parameters.words_length);
                    const uint64_t hash_value = hash(str);
                    const uint64_t modified_hash_value = ModifyHash(parameters, hash_value);
                    num_collisions += coll_flags[modified_hash_value].exchange(true);
                }
                sync_point.arrive_and_wait();
            }
        };

        {   // Запуск thread_task в нескольких потоках
            std::vector<std::jthread> threads(parameters.num_threads - 1);
            for (auto& t : threads) {
                t = std::jthread(thread_task);
            }
            thread_task();
        }

        // Возвращение результатов теста
        auto hash_name = static_cast<boost::json::string>(hash.GetName());
        if (parameters.mode == TestFlag::MASK) {
            hash_name += " (mask " + std::to_string(parameters.mask_bits) + " bits)";
        }
        boost::json::object result;
        result[hash_name] = collisions;
        return std::pair{std::move(hash_name), std::move(collisions)};
    }


    // Тестирование устойчивости к коллизиям хеш функций
    template<hfl::UnsignedIntegral UintT>
    void TestWithGeneratedBlocks(const std::vector<hfl::Hash<UintT>>& hashes, const GenBlocksParameters& parameters,
                                 out::Logger& logger) {
        out::StartAndEndLogBitsTest log(logger, parameters.hash_bits);

        auto out_json = out::GetGenTestJson(parameters, logger);
        boost::json::object collisions;

        // В цикле запускаются тесты (HashTestWithGenBlocks) для каждой хеш функции
        for (const auto& hash : hashes) {
            auto [hash_name, counters] = HashTestWithGenBlocks(hash, parameters, logger);
            collisions[hash_name] = std::move(counters);
        }

        // Сохранение результатов тестирования
        out_json.obj["Collisions"] = collisions;
        out_json.out << out_json.obj;
    }
}

#endif //THESIS_WORK_WORDS_TESTS_H