#ifndef THESIS_WORK_AVALANCHE_TESTS_H
#define THESIS_WORK_AVALANCHE_TESTS_H

#include <bitset>
#include <concepts>
#include <compare>
#include <future>
#include <vector>
#include <thread>

#include <boost/assert.hpp>
#include <boost/format.hpp>

#include <pcg_random.hpp>

#include "concurrency.h"
#include "generators.h"
#include "hashes.h"
#include "my_assert.h"
#include "output.h"
#include "test_parameters.h"
#include "timers.h"

namespace tests {
    class AvalancheInfo;

    namespace out {
        // Формирует json-файл, в который будет сохранена информация с теста хеш функции на лавинный эффект
        OutputJson GetAvalancheTestJson(const AvalancheTestParameters& parameters, out::Logger& loger);
        // Сохраняет информацию с теста хеш функции на лавинный эффект в json
        boost::json::object AvalancheInfoToJson(const AvalancheInfo& avalanche_info);
    }

    // Структура, которая хранит расстояние значения Хемминга (value) и его частоту (frequency)
    struct DistanceAndFrequency {
        uint16_t value = 0;     // Значение расстояния Хемминга
        uint64_t frequency = 0; // Частота данного значения Хемминга

        DistanceAndFrequency() = default;

        // Конструктор, которые инициализирует расстояние Хемминга целым числом
        // и устанавливает частоту равную 1
        explicit DistanceAndFrequency(std::integral auto distance)
            : value(distance)
            , frequency(1) {
        }

        // Оператор присваивания. Присваивает other_distance к value
        // и устанавливает значение частоты равную 1
        DistanceAndFrequency operator=(std::integral auto other_distance) {
            value = other_distance;
            frequency = 1;
            return *this;
        }

        // Операторы сравнения
        auto operator<=>(const DistanceAndFrequency& other) const { return value <=> other.value; };
        auto operator<=>(std::integral auto other_distance) const { return value <=> other_distance; };

        bool operator==(const DistanceAndFrequency& other) const { return value == other.value; };
        bool operator==(std::integral auto other_distance) const { return value == other_distance; };
    };

    // Оператор вывода структуры DistanceAndFrequency
    std::ostream& operator<<(std::ostream &os, const DistanceAndFrequency& distance_and_frequency);

    // Структура, которая хранит информацию о минимальном, максимальном
    // и среднем значении расстояния Хемминга
    struct HammingDistance {
        DistanceAndFrequency min {std::numeric_limits<uint8_t>::max()};     // худший случай
        DistanceAndFrequency max {0};                                       // лучший случай
        long double avg = 0;                                                // среднее значение
    };

    // Оператор вывода структуры HammingDistance
    std::ostream& operator<<(std::ostream &os, const HammingDistance& distance);

    // Структура, которая хранит пару число и его хеш
    struct NumberAndHash {
        uint64_t number = 0;
        uint64_t hash = 0;
    };

    // Оператор вывода структуры NumberAndHash
    std::ostream& operator<<(std::ostream &os, const NumberAndHash& number_and_hash);

    // Структура, которая хранит информацию о лавинном эффекте
    struct AvalancheInfo {
        HammingDistance hamming_distance;
        NumberAndHash original_pair;    // Пара оригинальные число и его хеш для худшего случая
        NumberAndHash modified_pair;    // Пара измененные число и его хеш для худшего случая
        std::vector<uint64_t> all_distances = std::vector<uint64_t>(65, 0); // частота всех расстояний хемминга
    };

    // Оператор вывода структуры AvalancheInfo
    std::ostream& operator<<(std::ostream &os, const AvalancheInfo& avalanche_info);

    // Сравнивает минимальное расстояние Хемминга с текущим
    void CompareAndChangeMinHammingDistance(AvalancheInfo& avalanche_info, const DistanceAndFrequency& dist_and_freq,
                                         const NumberAndHash& original, const NumberAndHash& modified);

    // Сравнивает максимальное расстояние Хемминга с текущим. Если значение больше максимального, то:
    // - присваивает текущее значение к максимальному;
    // - устанавливает значение частоты равное 1
    void CompareAndChangeMaxHammingDistance(AvalancheInfo& avalanche_info, const DistanceAndFrequency& dist_and_freq);

    // Рекуррентно вычисляет среднее расстояние Хемминга
    static inline long double CalculateArithmeticMean(long double prev_avg, long double current_value, long double step) {
        long double multiplier1 = 1l / (step + 1l);
        long double multiplier2 = step * prev_avg + current_value;
        return multiplier1 * multiplier2;
    }

    // Вычисляет расстояние хемминга. Реализация описана ниже
    template<hfl::UnsignedIntegral UintT>
    void CalculateHammingDistance(AvalancheInfo& avalanche_info, const hfl::Hash<UintT>& hash,
                                  const AvalancheTestParameters& tp, uint64_t original_number, uint64_t iteration_step);

    // Тестирование лавинного эффекта для одной хеш функции. Реализация описана ниже
    template<hfl::UnsignedIntegral UintT>
    AvalancheInfo HashAvalancheTest(const hfl::Hash<UintT>& hash, const AvalancheTestParameters& tp,
                                    out::Logger& logger);

    // Тестирование лавинного эффекта всех функций. Реализация описана ниже
    template<hfl::UnsignedIntegral UintT>
    void AvalancheTest(const std::vector<hfl::Hash<UintT>>& hashes, const AvalancheTestParameters& tp,
                       out::Logger& logger);

    // Запускает тестирование лавинного эффекта всех функций
    void RunAvalancheTests(out::Logger& loger);


// ==================================================

    // Вычисляет расстояние хемминга.
    template<hfl::UnsignedIntegral UintT>
    void CalculateHammingDistance(AvalancheInfo& avalanche_info, const hfl::Hash<UintT>& hash,
                                  const AvalancheTestParameters& tp, uint64_t original_number,
                                  uint64_t iteration_step) {
        // число бит хэшируемого числа
        constexpr uint8_t number_size = 64;
        // Число бит хеш-значения
        constexpr uint8_t hash_size = std::numeric_limits<UintT>::digits;
        // Пара исходного числа и его хеша
        const NumberAndHash original {original_number, hash(original_number)};

        // Цикл, в котором меняется i-ый бит исходного числа (i = 1, 2, ... 64)
        // и вычисляется расстояние Хемминга
        for (uint8_t bit_index = 0; bit_index < number_size; ++bit_index) {
            std::bitset<number_size> modified_number_bits = original_number;
            modified_number_bits.flip(bit_index);
            const uint64_t modified_number = modified_number_bits.to_ullong();
            const NumberAndHash modified {modified_number, hash(modified_number)};
            std::bitset<hash_size> xor_hashes = original.hash ^ modified.hash;
            DistanceAndFrequency hamming_distance{xor_hashes.count()};
            ++avalanche_info.all_distances[hamming_distance.value];
            CompareAndChangeMinHammingDistance(avalanche_info, hamming_distance, original, modified);
            CompareAndChangeMaxHammingDistance(avalanche_info, hamming_distance);
            avalanche_info.hamming_distance.avg = CalculateArithmeticMean(avalanche_info.hamming_distance.avg,
                                                                          hamming_distance.value, iteration_step);
        }
    }

    // Тестирование лавинного эффекта для одной хеш функции
    template<hfl::UnsignedIntegral UintT>
    AvalancheInfo HashAvalancheTest(const hfl::Hash<UintT>& hash, const AvalancheTestParameters& tp,
                                    out::Logger& logger) {
        logger << boost::format("\t%1%: \n") % hash.GetName();

        // Выдает генераторы, число которых равно числу запускаемых потоков
        auto generators = GetGenerators(tp.num_threads, tp.num_keys);

        std::atomic_uint16_t gen_index = 0;
        // Функция, запускаемая в отдельном потоке.
        // Вычисляет расстояния хемминга для last - first чисел
        auto thread_task = [&hash, &tp, &generators, &gen_index] (uint64_t first, uint64_t last) {
            pcg64 rng = generators[gen_index++];
            AvalancheInfo avalanche_info;
            while (first < last) {
                uint64_t number = rng();
                CalculateHammingDistance(avalanche_info, hash, tp, number, ++first);
            }
            return avalanche_info;
        };

        uint16_t step = 0;
        // Функция, в которой объединяется информация о лавинном эффекте вычисленном в разных потоках
        auto merge_results = [&step](AvalancheInfo& best_result, const AvalancheInfo& current_result)  {
            CompareAndChangeMinHammingDistance(best_result, current_result.hamming_distance.min,
                                               current_result.original_pair, current_result.modified_pair);
            CompareAndChangeMaxHammingDistance(best_result, current_result.hamming_distance.max);
            best_result.hamming_distance.avg = CalculateArithmeticMean(best_result.hamming_distance.avg,
                                                                       current_result.hamming_distance.avg, ++step);
            for (size_t distance; distance < best_result.all_distances.size(); ++distance) {
                best_result.all_distances[distance] += current_result.all_distances[distance];
            }
        };

        // Запускает вычисление лавинного эффекта в нескольких потоках
        ThreadTasks<AvalancheInfo> tasks(thread_task, merge_results, tp.num_threads, tp.num_keys);
        const auto result = tasks.GetResult();

        // Логирование информации о лавинном эффекте
        logger << result << std::endl;
        for (size_t i = 0; i < result.all_distances.size(); ++i) {
            std::cout << "\t\tdistance =" << i << " , frequency = " << result.all_distances[i] << '\n';
        }
        return result;
    }

    // Тестирование лавинного эффекта всех функций
    template<hfl::UnsignedIntegral UintT>
    void AvalancheTest(const std::vector<hfl::Hash<UintT>>& hashes, const AvalancheTestParameters& tp,
                       out::Logger& logger) {
        out::StartAndEndLogBitsTest log(logger, tp.hash_bits);

        auto out_json = out::GetAvalancheTestJson(tp, logger);
        boost::json::object avalanche_statistics;

        // В цикле запускаются тесты (HashAvalancheTest) для каждой хеш функции
        for (const auto& hash : hashes) {
            out::LogDuration log_duration("\t\ttime", logger);
            AvalancheInfo avalanche_info = HashAvalancheTest(hash, tp, logger);
            auto hash_avalanche_statistics = out::AvalancheInfoToJson(avalanche_info);
            logger << "\t\tmedian hamming distance: " << hash_avalanche_statistics["Median case"] << std::endl;
            avalanche_statistics[hash.GetName()] = std::move(out::AvalancheInfoToJson(avalanche_info));
        }

        // Сохранение информации из тестов в json
        out_json.obj["Avalanche effect"] = avalanche_statistics;
        out_json.out << out_json.obj;
    }
}

#endif //THESIS_WORK_AVALANCHE_TESTS_H
