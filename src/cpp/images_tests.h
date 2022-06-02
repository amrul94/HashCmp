#ifndef THESIS_WORK_IMAGES_TESTS_H
#define THESIS_WORK_IMAGES_TESTS_H

//#include <deque>
#include <filesystem>

#include <boost/format.hpp>

#include "concurrency.h"
#include "count_collisions.h"
#include "hashes.h"
#include "output.h"
#include "test_parameters.h"
#include "timers.h"

namespace tests {
    namespace out {
        /*
         *  Формирует json-файл, в который будет сохранена информация с теста хеш функции
         *  на устойчивости к коллизиям
         *  Входные параметры:
         *      1. parameters - параметры тестирования:
         *          - битность хеша (16, 32 или 64)
         *          - число потоков (зависит от системы)
         *      2. logger - записывает лог в файл и выводит его на консоль
         */
        OutputJson GetImagesTestJson(const TestParameters& parameters, out::Logger& logger);
    }

    /*
     *  Тестирование устойчивости к коллизиям одной хеш функции. Реализация описана ниже
     *  Параметр шаблона: целое беззнаковое число - тип хеш-значения
     *  Входные параметры:
     *      1. hash - хеш-функция
     *      2. parameters - параметры тестирования:
     *          - битность хеша (16, 24, 32, 48 или 64)
     *          - число потоков (зависит от системы)
     *      3. logger - записывает лог в файл и выводит его на консоль
     *  Выходное значение: число коллизий
     */
    template <hfl::UnsignedIntegral UintT>
    uint64_t HashTestWithImages(const hfl::Hash<UintT>& hash, const TestParameters& parameters, out::Logger& logger);

    /*
     *  Тестирование устойчивости к коллизиям хеш функций. Реализация описана ниже
     *  Параметр шаблона: целое беззнаковое число - тип хеш-значения
     *  Входные параметры:
     *      1. hashes - массив со всеми хеш-функциями одной битности
     *      3. parameters - параметры тестирования:
     *          - битность хеша (16, 24, 32, 48 или 64)
     *          - число потоков (зависит от системы)
     *      4. logger - записывает лог в файл и выводит его на консоль
     */
    template <hfl::UnsignedIntegral UintT>
    void TestWithImages(const std::vector<hfl::Hash<UintT>>& hashes, const TestParameters& parameters,
                        out::Logger& logger);

// =======================================================================================

    // Тестирование устойчивости к коллизиям одной хеш функции
    template <hfl::UnsignedIntegral UintT>
    uint64_t HashTestWithImages(const hfl::Hash<UintT>& hash, const TestParameters& parameters, out::Logger& logger) {
        namespace fs = std::filesystem;
        using HashMap = std::map<uint64_t, uint64_t>;

        out::LogDuration log_duration("\t\ttime", logger);
        logger << boost::format("\n\t%1%: \n") % hash.GetName();

        fs::path general_images_dir = std::filesystem::current_path() / "data/images";
        std::atomic_uint8_t dir_number;

        // Функция, запускаемая в отдельном потоке
        auto thread_task = [&hash, &parameters, &dir_number, general_images_dir](uint64_t, uint64_t) {
            fs::path current_images_dir = general_images_dir / std::to_string(dir_number++);
            HashMap hash_values;
            std::vector<fs::path> removed_path;
            // Вычисляет хеши изображений и заполняет ими ассоциативный массив (hash_values)
            for (const auto& dir_entry: fs::recursive_directory_iterator(current_images_dir)) {
                const auto& path = dir_entry.path();
                const auto status = fs::status(path);
                if (!fs::is_directory(status)) {
                    std::ifstream image_file = std::ifstream(path, std::ios_base::binary);
                    const uint64_t hash_value = hash(image_file);
                    ++hash_values[hash_value];
                }
            }
            return hash_values;
        };

        // Функция, которая объединяет результаты выполнения thread_task,
        // запущенных в разных потоках
        auto merge_results = [](HashMap& all_hashes, const HashMap& part_hashes) {
            for (const auto& [hash_value, count] : part_hashes) {
                all_hashes[hash_value] += count;
            }
        };

        // Запуск теста в разных потоках
        ThreadTasks<HashMap> tasks(thread_task, merge_results, parameters.num_threads, parameters.num_threads);
        const auto hashes = tasks.GetResult();

        // Вычисление числа коллизий
        const auto num_collisions = CountCollisions(hashes);

        logger << boost::format("\t\tcollisions: %1%\n") % num_collisions;
        return num_collisions;
    }

    // Тестирование устойчивости к коллизиям хеш функций
    template <hfl::UnsignedIntegral UintT>
    void TestWithImages(const std::vector<hfl::Hash<UintT>>& hashes, const TestParameters& parameters,
                        out::Logger& logger) {
        out::StartAndEndLogBitsTest printer(logger, parameters.hash_bits);

        auto out_json = out::GetImagesTestJson(parameters, logger);
        boost::json::object collisions;
        // В цикле запускает тест для каждой хеш функции
        for (const auto& hash : hashes) {
            collisions[hash.GetName()] = HashTestWithImages(hash, parameters, logger);
        }
        out_json.obj["Collisions"] = collisions;
        out_json.out << out_json.obj;
    }

    // Запуск тестирования устойчивости к коллизиям хеш функций.
    void RunImagesTests(out::Logger& logger);
}
#endif //THESIS_WORK_IMAGES_TESTS_H
