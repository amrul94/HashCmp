#include "images_tests.h"
#include "hashes.h"

namespace tests {
    namespace out {
        OutputJson GetImagesTestJson(const TestParameters& tp, ReportsRoot& reports_root) {
            const std::string test_name = "Images tests";
            const std::filesystem::path report_test_dir = test_name;
            const auto report_test_path = reports_root.root_path / report_test_dir;
            std::filesystem::create_directories(report_test_path);

            const std::filesystem::path report_name = std::to_string(tp.hash_bits) + " bits.json";
            const std::filesystem::path out_path = report_test_path / report_name;
            std::ofstream out(out_path);
            assert(out);

            boost::json::object obj;
            obj["Test name"] = test_name;
            obj["Bits"] = tp.hash_bits;
            return OutputJson{std::move(obj), std::move(out)};
        }
    }

    #define RUN_IMAGES_TEST_IMPL(BITS, NUM_THREADS, ROOT) \
        const auto hashes##BITS = hfl::Build##BITS##bitsHashes();   \
        const TestParameters tp##BITS{BITS, NUM_THREADS};     \
        TestWithImages(hashes##BITS, tp##BITS, ROOT)

    void RunImagesTests(ReportsRoot& reports_root) {
        const uint16_t num_threads = GetNumThreads();
        RUN_IMAGES_TEST_IMPL(16, num_threads, reports_root);
        RUN_IMAGES_TEST_IMPL(24, num_threads, reports_root);
        RUN_IMAGES_TEST_IMPL(32, num_threads, reports_root);
        RUN_IMAGES_TEST_IMPL(48, num_threads, reports_root);
        RUN_IMAGES_TEST_IMPL(64, num_threads, reports_root);
    }

}

