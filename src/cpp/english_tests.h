#ifndef THESIS_WORK_ENGLISH_TESTS_H
#define THESIS_WORK_ENGLISH_TESTS_H

#include <map>
#include <vector>

#include <boost/format.hpp>
#include <boost/json.hpp>

#include "count_collisions.h"
#include "output.h"
#include "timers.h"
#include "test_parameters.h"
#include "hashes.h"

namespace tests {
    namespace out {
        // Формирует json-файл, в который будет сохранена информация с теста хеш функции
        // на устойчивости к коллизиям
        OutputJson GetEnglishTestJson(const TestParameters& parameters, size_t num_words, out::Logger& logger);
    }

    // Парсинг слов из файла
    std::vector<std::string> ParseWords(const std::filesystem::path& file_name);

    // Тестирование устойчивости к коллизиям одной хеш функции. Реализация описана ниже
    template <hfl::UnsignedIntegral UintT>
    uint64_t HashTestWithEngWords(const hfl::Hash<UintT>& hash, const std::vector<std::string>& words,
                                  const TestParameters& parameters, out::Logger& logger);

    // Тестирование устойчивости к коллизиям хеш функций. Реализация описана ниже
    template <hfl::UnsignedIntegral UintT>
    void TestWithEnglishWords(const std::vector<hfl::Hash<UintT>>& hashes, const std::vector<std::string>& words,
                              const TestParameters& parameters, out::Logger& logger);

    // Запуск тестирования устойчивости к коллизиям хеш функций.
    void RunTestWithEnglishWords(out::Logger& logger);

// =====================================================

    // Тестирование устойчивости к коллизиям одной хеш функции
    template <hfl::UnsignedIntegral UintT>
    uint64_t HashTestWithEngWords(const hfl::Hash<UintT>& hash, const std::vector<std::string>& words,
                                  const TestParameters& parameters, out::Logger& logger) {
        out::LogDuration log_duration("\t\ttime", logger);
        logger << '\t' + hash.GetName() + ':' << std::endl;

        // Вычисляет хеши слов и заполняет ими ассоциативный массив (hash_values)
        std::map<uint64_t, uint64_t> hash_values;
        for (const std::string& word : words) {
            const auto hash_value = static_cast<uint64_t>(hash(word));
            ++hash_values[hash_value];
        }

        // Вычисляет число коллизий
        const uint32_t num_collisions = CountCollisions(hash_values);
        // Выводит лог
        logger << "\t\tcollisions: " << num_collisions << std::endl;

        return num_collisions;
    }

    // Тестирование устойчивости к коллизиям хеш функций
    template <hfl::UnsignedIntegral UintT>
    void TestWithEnglishWords(const std::vector<hfl::Hash<UintT>>& hashes, const std::vector<std::string>& words,
                              const TestParameters& parameters, out::Logger& logger) {
        out::StartAndEndLogBitsTest printer(logger, parameters.hash_bits);

        auto out_json = out::GetEnglishTestJson(parameters, words.size(), logger);
        boost::json::object collisions;
        // В цикле запускает тест для каждой хеш функции
        for (const hfl::Hash<UintT>& hash : hashes) {
            collisions[hash.GetName()] = HashTestWithEngWords(hash, words, parameters, logger);
        }
        out_json.obj["Collisions"] = collisions;
        out_json.out << out_json.obj;
    }

}

#endif //THESIS_WORK_ENGLISH_TESTS_H
