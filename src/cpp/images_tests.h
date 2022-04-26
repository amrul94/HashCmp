#ifndef THESISWORK_IMAGES_TESTS_H
#define THESISWORK_IMAGES_TESTS_H

#include <deque>
#include <filesystem>

#include <boost/format.hpp>

#include "english_tests.h"
#include "concurrency.h"
#include "timers.h"
#include "output.h"
#include "test_parameters.h"
#include "hashes.h"

namespace tests {
    namespace out {
        OutputJson GetImagesTestJson(const TestParameters& tp, out::Logger& logger);
    }

    template <hfl::UnsignedIntegral UintT>
    uint64_t HashTestWithImages(const hfl::Hash<UintT>& hash, const TestParameters& tp, out::Logger& logger) {
        namespace fs = std::filesystem;
        using HashMap = std::map<uint64_t, uint64_t>;

        out::LogDuration log_duration("\t\ttime", logger);
        logger << boost::format("\n\t%1%: \n") % hash.GetName();

        fs::path general_images_dir = std::filesystem::current_path() / "data/images";
        std::atomic_uint8_t dir_number;

        auto thread_task = [&hash, &tp, &dir_number, general_images_dir](uint64_t, uint64_t) {
            fs::path current_images_dir = general_images_dir / std::to_string(dir_number++);
            HashMap hashes;

            for (const auto& dir_entry: fs::recursive_directory_iterator(current_images_dir)) {
                const auto& path = dir_entry.path();
                const auto status = fs::status(path);
                if (!fs::is_directory(status)) {
                    std::ifstream image_file = std::ifstream(path, std::ios_base::binary);
                    const uint64_t hash_value = hash(image_file);
                    ++hashes[hash_value];
                }
            }

            return std::move(hashes);
        };

        auto merge_results = [](HashMap& all_hashes, const HashMap& part_hashes) {
            for (const auto& [hash_value, count] : part_hashes) {
                all_hashes[hash_value] += count;
            }
        };

        ThreadTasks<HashMap> tasks(thread_task, merge_results, tp.num_threads, tp.num_threads);
        const auto hashes = tasks.GetResult();
        const auto num_collisions = CountCollisions(hashes);

        logger << boost::format("\t\tcollisions: %1%\n") % num_collisions;
        return num_collisions;
    }

    template <hfl::UnsignedIntegral UintT>
    void TestWithImages(const std::vector<hfl::Hash<UintT>>& hashes, const TestParameters& parameters, out::Logger& logger) {
        logger << boost::format("--- START TEST WITH %1% BITS HASHES ---\n") % parameters.hash_bits;

        auto out_json = out::GetImagesTestJson(parameters, logger);
        boost::json::object collisions;
        for (const auto& hash : hashes) {
            collisions[hash.GetName()] = HashTestWithImages(hash, parameters, logger);
        }
        out_json.obj["Collisions"] = collisions;
        out_json.out << out_json.obj;

        logger << boost::format("--- END TEST WITH %1% BITS HASHES ---\n\n") % parameters.hash_bits;
    }

    void RunImagesTests(out::Logger& logger);
}
#endif //THESISWORK_IMAGES_TESTS_H
