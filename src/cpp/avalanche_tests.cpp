#include "avalanche_tests.h"

namespace tests {
    static constexpr uint8_t NUM_HASH_BITS = 64;
    namespace out {
        // Формирует json-файл, в который будет сохранена информация с теста хеш функции на лавинный эффект
        OutputJson GetAvalancheTestJson(const AvalancheTestParameters& parameters, out::Logger& logger) {
            // Создает папку, в которую будет сохранен json
            const std::string test_name = "Avalanche effect tests";
            const std::filesystem::path report_test_dir = test_name;
            const auto report_test_path = logger.GetLogDirPath() / report_test_dir;
            std::filesystem::create_directories(report_test_path);

            // Создает файл, в который будет сохранен json
            const std::filesystem::path report_name = std::to_string(parameters.hash_bits) + " bits.json";
            const std::filesystem::path out_path = report_test_path / report_name;
            std::ofstream out(out_path);
            BOOST_ASSERT_MSG(out, "Failed to create json file");

            // Формирует json
            boost::json::object obj;
            obj["Test name"] = test_name;
            obj["Bits"] = parameters.hash_bits;
            return OutputJson{std::move(obj), std::move(out)};
        }

        // Вычисляет медианное расстояние Хемминга
        static uint8_t Median(const std::vector<uint64_t>& hamming_distances) {
            const uint64_t num_distances = std::accumulate(hamming_distances.begin(),
                                                           hamming_distances.end(), 0ull);
            const uint64_t median_position = num_distances / 2;
            uint64_t current_position = 0;
            size_t index = 0;
            for (; index < hamming_distances.size() && current_position < median_position; ++index) {
                current_position += hamming_distances[index];
            }
            return index - 1;
        }

        // Сохраняет информацию с теста хеш функции на лавинный эффект в json
        boost::json::object AvalancheInfoToJson(const AvalancheInfo& avalanche_info) {
            boost::json::object avalanche_statistics;

            boost::json::object worst_case;
            worst_case["Distance"] = avalanche_info.hamming_distance.min.value;
            worst_case["Frequency"] = avalanche_info.hamming_distance.min.frequency;
            avalanche_statistics["Worst case"] = worst_case;

            boost::json::object best_case;
            best_case["Distance"] = avalanche_info.hamming_distance.max.value;
            best_case["Frequency"] = avalanche_info.hamming_distance.max.frequency;
            avalanche_statistics["Best case"] = best_case;

            avalanche_statistics["Average case"] = static_cast<double>(avalanche_info.hamming_distance.avg);
            avalanche_statistics["Median case"] = Median(avalanche_info.all_distances);

            boost::json::object original_pair;
            original_pair["Number"] = avalanche_info.original_pair.number;
            original_pair["Hash"] = std::bitset<NUM_HASH_BITS>(avalanche_info.original_pair.hash).to_string();
            avalanche_statistics["Original pair"] = original_pair;

            boost::json::object modified_pair;
            modified_pair["Number"] = avalanche_info.modified_pair.number;
            modified_pair["Hash"] = std::bitset<NUM_HASH_BITS>(avalanche_info.modified_pair.hash).to_string();
            avalanche_statistics["Modified pair"] = modified_pair;

            return avalanche_statistics;
        }
    }

    // Оператор вывода структуры DistanceAndFrequency
    std::ostream& operator<<(std::ostream &os, const DistanceAndFrequency& distance_and_frequency) {
        return os  << "value = " << distance_and_frequency.value
                   << ", frequency = " << distance_and_frequency.frequency;
    }

    // Оператор вывода структуры HammingDistance
    std::ostream& operator<<(std::ostream &os, const HammingDistance& distance) {
        return os  << "\t\t\tminimum: " << distance.min
                   << "\n\t\t\tmaximum: " << distance.max
                   << "\n\t\t\taverage: " << distance.avg;
    }

    // Оператор вывода структуры NumberAndHash
    std::ostream& operator<<(std::ostream &os, const NumberAndHash& number_and_hash) {
        return os  << "number = " << number_and_hash.number << ", hash = "
                   << std::bitset<NUM_HASH_BITS>(number_and_hash.hash);
    }

    // Оператор вывода структуры AvalancheInfo
    std::ostream& operator<<(std::ostream &os, const AvalancheInfo& avalanche_info) {
        return os  << "\t\thamming distance:\n" << avalanche_info.hamming_distance
                   << "\n\t\toriginal: " << avalanche_info.original_pair
                   << "\n\t\tmodified: " << avalanche_info.modified_pair;
    }

    // Сравнивает минимальное расстояние Хемминга с текущим.
    void CompareAndChangeMinHammingDistance(AvalancheInfo& avalanche_info, const DistanceAndFrequency& dist_and_freq,
                                            const NumberAndHash& original, const NumberAndHash& modified) {
        if (avalanche_info.hamming_distance.min == dist_and_freq) {
            // Если текущее значение равно минимальному, то увеличивает частоту минимального расстояния
            avalanche_info.hamming_distance.min.frequency += dist_and_freq.frequency;
        } else if (avalanche_info.hamming_distance.min > dist_and_freq) {
            // Если текущее значение меньше минимального, то:
            // - присваивает текущее значение к минимальному;
            // - устанавливает значение частоты равное 1;
            // - сохраняет пары NumberAndHash
            avalanche_info.hamming_distance.min = dist_and_freq;
            avalanche_info.original_pair = original;
            avalanche_info.modified_pair = modified;
        }
    }

    void CompareAndChangeMaxHammingDistance(AvalancheInfo& avalanche_info, const DistanceAndFrequency& dist_and_freq) {
        if (avalanche_info.hamming_distance.max == dist_and_freq) {
            // Если текущее значение равно максимальному, то увеличивает частоту максимального расстояния
            avalanche_info.hamming_distance.max.frequency += dist_and_freq.frequency;
        } else if (avalanche_info.hamming_distance.max < dist_and_freq) {
            // Если значение больше максимального, то:
            // - присваивает текущее значение к максимальному;
            // - устанавливает значение частоты равное 1
            avalanche_info.hamming_distance.max = dist_and_freq;
        }
    }

    // Запускает тестирование лавинного эффекта всех функций одной битности
    template<hfl::UnsignedIntegral UintT>
    static void RunAvalancheTestImpl(uint16_t num_threads, out::Logger& logger) {
        constexpr uint16_t bits = std::numeric_limits<UintT>::digits;
        constexpr uint64_t num_keys = 1ull << 32;
        const auto hashes = hfl::BuildHashes<UintT>();
        const AvalancheTestParameters parameters{bits, num_threads, num_keys};
        AvalancheTest(hashes, parameters, logger);
    }

    // Запускает тестирование лавинного эффекта всех функций всех битностей
    void RunAvalancheTests(out::Logger& logger) {
        out::StartAndEndLogTest start_and_end_log(logger, "AVALANCHE");
        const uint16_t num_threads = GetNumThreads();

        RunAvalancheTestImpl<uint16_t>(num_threads, logger);
        RunAvalancheTestImpl<uint32_t>(num_threads, logger);
        RunAvalancheTestImpl<uint64_t>(num_threads, logger);
    }
}