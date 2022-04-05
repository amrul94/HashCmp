#ifndef THESISWORK_IMAGES_TESTS_H
#define THESISWORK_IMAGES_TESTS_H

#include <deque>
#include <filesystem>

#include <boost/format.hpp>

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
        LOG_DURATION_STREAM("\t\ttime", reports_root.logger);
        reports_root.logger << boost::format("\n\t%1%: \n") % hs.name;
        namespace fs = std::filesystem;
        fs::path images_dir = std::filesystem::current_path() / "data/images (new)/Original/Checked (Part 2)";

        // Этот код частично повторяется с HashTestWithEngWords()
        std::map<uint64_t, uint64_t> hashes;
        for (const auto& dir_entry: fs::recursive_directory_iterator(images_dir)) {
            const auto& path = dir_entry.path();
            const auto status = fs::status(path);
            if (!fs::is_directory(status)) {
                img::Image image = img::LoadJPEG(path);
                const auto hash = static_cast<uint64_t>(hs.hasher(image));
                const uint64_t modify = ModifyHash(tp, hash);
                ++hashes[modify];
            }
        }
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
