#include "distribution_tests.h"

#include <span>

#include <boost/format.hpp>
#include <boost/json.hpp>

namespace tests {
    namespace {
        out::OutputJson GetDistTestJson(const DistTestParameters& dtp, const std::string& hash_name,
                                        ReportsRoot& reports_root) {
            using namespace std::literals;
            const std::filesystem::path check_dist_dir = "Distribution tests";
            const std::filesystem::path hash_bits_dir = std::to_string(dtp.hash_bits);
            const std::filesystem::path report_name = hash_name + " with "s + TestFlagToString(dtp.mode) + " mode.json";
            const auto hash_bits_path = reports_root.root_path / check_dist_dir / hash_bits_dir;
            std::filesystem::create_directories(hash_bits_path);

            const std::filesystem::path hash_out_path = hash_bits_path / report_name;
            std::ofstream out(hash_out_path);
            return out::OutputJson{boost::json::object{}, std::move(out)};
        }

        // потом переделаю
        uint64_t Median(uint64_t first, uint64_t last, uint64_t bin_size) {
            // Требует переделки (оставил на 8 семестр)
            uint64_t med_index = (last + first) / 2;
            return med_index * bin_size;
        }

        boost::json::object ProcessingStatistics(const std::vector<std::atomic<Bucket>>& buckets,
                                                 const DistTestParameters& dtp, const std::string& hash_name) {
            // Разгрузить эту функцию разделив ее на части
            // Возможно стоит сделать структуру для хранения json данных

            boost::json::object result;
            result["Test name"] = "Test Check Distribution";
            result["Mode"] = TestFlagToString(dtp.mode);
            result["Bits"] = dtp.hash_bits;
            result["Hash name"] = hash_name;

            uint16_t bar_count = 16;
            boost::json::array x_bars(bar_count);
            std::iota(x_bars.begin(), x_bars.end(), 1);

            boost::json::array x_ranges(bar_count);
            boost::json::array y_mean(bar_count);
            boost::json::array y_err_min(bar_count);
            boost::json::array y_err_max(bar_count);
            boost::json::array y_min(bar_count);
            boost::json::array y_max(bar_count);

            // возможно стоит вынести в отдельную функцию
            auto lambda = [&](uint64_t start_bar, uint64_t end_bar) {
                uint64_t step = buckets.size() / bar_count;
                for (uint32_t bar = start_bar; bar < end_bar; ++bar) {
                    const uint64_t begin = bar * step;
                    const uint64_t end = (bar + 1) * step;

                    const uint64_t median = Median(begin, end, dtp.divisor);
                    x_ranges[bar] = median;

                    // Указатели стоит заменить на итераторы
                    const std::span y_block(buckets.data() + begin, buckets.data() + end);

                    // думаю стоит сделать long double вместо double
                    const double sum_value = std::accumulate(y_block.begin(), y_block.end(), static_cast<double>(0));
                    const double avg_value = sum_value / static_cast<double>(step);
                    y_mean[bar] = avg_value;
                    const auto min_value = std::ranges::min_element(y_block);
                    const auto max_value = std::ranges::max_element(y_block);
                    const auto min_diff = std::abs(avg_value - *min_value);
                    const auto max_diff = std::abs(avg_value - *max_value);
                    y_err_min[bar] = min_diff;
                    y_err_max[bar] = max_diff;
                    y_min[bar] = *min_value;
                    y_max[bar] = *max_value;
                }
            };

            uint64_t start_bar = 0;
            uint64_t step_bar = bar_count / dtp.num_threads;
            std::vector<std::thread> threads(dtp.num_threads - 1);

            for (auto& t : threads) {
                t = std::thread{lambda, start_bar, start_bar + step_bar};
                start_bar += step_bar;
            }
            lambda(start_bar, bar_count);

            for (auto& t : threads) {
                t.join();
            }

            result["Bar count"] = bar_count;
            result["Bin size"] = dtp.divisor;
            result["X ranges"] = x_ranges;
            result["Y mean"] = y_mean;
            result["Y err min"] = y_err_min;
            result["Y err max"] = y_err_max;
            result["Y min"] = y_min;
            result["Y max"] = y_max;
            return result;
        }
    }

    void PrintReports(const std::vector<std::atomic<Bucket>>& buckets, const DistTestParameters& dtp, const std::string& hash_name,
                      ReportsRoot& reports_root) {

        auto out_json = GetDistTestJson(dtp, hash_name, reports_root);
        out_json.obj = ProcessingStatistics(buckets, dtp, hash_name);
        out_json.out << out_json.obj;

    }

    #define RUN_DIST_TEST_NORMAL_IMPL(BITS, NUM_THREADS, ROOT)                  \
        const auto hashes##BITS = hfl::Build##BITS##bitsHashes();               \
        const DistTestParameters cp##BITS{BITS, NUM_THREADS, TestFlag::NORMAL}; \
        DistributionTest(hashes##BITS, cp##BITS, ROOT)

    void RunDistTestNormal(size_t num_threads, ReportsRoot& reports_root) {
        RUN_DIST_TEST_NORMAL_IMPL(16, num_threads, reports_root);
        RUN_DIST_TEST_NORMAL_IMPL(32, num_threads, reports_root);
    }

    #define RUN_DIST_TEST_WITH_BINS_IMPL(BITS, NUM_THREADS, ROOT)                     \
        const auto hashes##BITS = hfl::Build##BITS##bitsHashes();               \
        const DistTestParameters cp##BITS{BITS, NUM_THREADS, TestFlag::BINS};   \
        DistributionTest(hashes##BITS, cp##BITS, ROOT)


    void RunDistTestWithBins(size_t num_threads, ReportsRoot& reports_root) {
        RUN_DIST_TEST_WITH_BINS_IMPL(64, num_threads, reports_root);
    }

    void RunDistributionTests(ReportsRoot& reports_root) {
        const size_t hardware_threads = std::thread::hardware_concurrency();
        const size_t num_threads = hardware_threads != 0 ? hardware_threads : 1;
        reports_root.logger << boost::format("\tnum_threads = %1%\n\n") % num_threads;

        RunDistTestNormal(num_threads, reports_root);
        RunDistTestWithBins(num_threads, reports_root);
    }
}
