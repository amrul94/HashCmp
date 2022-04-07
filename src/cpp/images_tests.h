#ifndef THESISWORK_IMAGES_TESTS_H
#define THESISWORK_IMAGES_TESTS_H

#include <deque>
#include <filesystem>

#include <boost/format.hpp>

#include "concurrency.h"
#include "jpeg_image.h"
#include "log_duration.h"
#include "test_parameters.h"
#include "english_tests.h"

namespace tests {
    namespace out {
        OutputJson GetImagesTestJson(const TestParameters& tp, ReportsRoot& reports_root);
    }

    template <typename HashStruct>
    uint64_t HashTestWithImages(const HashStruct& hs, const TestParameters& tp, ReportsRoot& reports_root) {
        namespace fs = std::filesystem;
        using HashMap = std::map<uint64_t, uint64_t>;

        LOG_DURATION_STREAM("\t\ttime", reports_root.logger);
        reports_root.logger << boost::format("\n\t%1%: \n") % hs.name;

        fs::path general_images_dir = std::filesystem::current_path() / "data/images";
        std::atomic_uint8_t dir_number;

        auto thread_task = [&hs, &tp, &dir_number, general_images_dir](uint64_t, uint64_t) {
            fs::path current_images_dir = general_images_dir / std::to_string(dir_number++);
            HashMap hashes;

            for (const auto& dir_entry: fs::recursive_directory_iterator(current_images_dir)) {
                const auto& path = dir_entry.path();
                const auto status = fs::status(path);
                if (!fs::is_directory(status)) {
                    std::ifstream image_file = std::ifstream(path, std::ios_base::binary);
                    const auto hash = static_cast<uint64_t>(hs.hasher(image_file));
                    const uint64_t modify = ModifyHash(tp, hash);
                    ++hashes[modify];
                }
            }

            return std::move(hashes);
        };

        auto merge_results = [](HashMap& all_hashes, const HashMap& part_hashes) {
            for (const auto& [hash, count] : part_hashes) {
                all_hashes[hash] += count;
            }
        };

        ThreadTasks<HashMap> tasks(thread_task, merge_results, tp.num_threads, tp.num_threads);
        const auto hashes = tasks.GetResult();
        const auto num_collisions = CountCollisions(hashes);

        reports_root.logger << boost::format("\t\tcollisions: %1%\n") % num_collisions;
        return num_collisions;
    }

    template <typename HashStruct>
    void TestWithImages(const std::vector<HashStruct>& hashes, const TestParameters& tp, ReportsRoot& reports_root) {
        reports_root.logger << boost::format("--- START TEST WITH %1% BITS HASHES ---\n") % tp.hash_bits;

        auto out_json = out::GetImagesTestJson(tp, reports_root);
        boost::json::object collisions;
        for (const auto& hs : hashes) {
            collisions[hs.name] = HashTestWithImages(hs, tp,reports_root);
        }
        out_json.obj["Collisions"] = collisions;
        out_json.out << out_json.obj;

        reports_root.logger << boost::format("--- END TEST WITH %1% BITS HASHES ---\n\n") % tp.hash_bits;
    }

    void RunImagesTests(ReportsRoot& reports_root);
}
#endif //THESISWORK_IMAGES_TESTS_H
