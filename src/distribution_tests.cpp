#include "distribution_tests.h"

#include <span>

#include <boost/accumulators/statistics/mean.hpp>
#include <boost/json.hpp>

namespace {
    using json_obj = boost::json::object;
    // потом переделаю

    uint64_t Median(uint64_t first, uint64_t last, uint64_t bin_size) {
        uint64_t med_index = (last + first) / 2;
        uint64_t m1 = (med_index - 1) * bin_size;
        uint64_t m2 = med_index * bin_size;
        return (m1 + m2) / 2;
    }

    json_obj ProcessingStatistics(const std::vector<uint32_t>& buckets, const CheckParameters& cp, const std::string& hash_name) {
        boost::json::object result;
        result["Test name"] = "Test Check Distribution";
        result["Mode"] = TestFlagToString(cp.mode);
        result["Bits"] = cp.hash_bits;
        result["Hash name"] = hash_name;
        uint32_t bar_count = 16;

        boost::json::array x_bars(16, 0);
        std::iota(x_bars.begin(),  x_bars.end(), 1);

        boost::json::array x_ranges{};
        boost::json::array y_mean{};
        boost::json::array y_err_min{};
        boost::json::array y_err_max{};
        boost::json::array y_min{};
        boost::json::array y_max{};

        uint32_t step = buckets.size() / bar_count;
        for (uint32_t i = 0; i < bar_count; ++i) {
            const uint32_t begin = i * step;
            const uint32_t end = (i + 1) * step;

            const uint64_t median = Median(begin, end, cp.divisor);
            x_ranges.push_back(median);
            const std::span y_block(buckets.begin() + begin, buckets.begin() + end);
            const double sum_value = std::accumulate(y_block.begin(), y_block.end(), static_cast<double>(0));
            const double avg_value = sum_value / static_cast<double>(step);
            y_mean.push_back(avg_value);
            const auto min_value = std::ranges::min_element(y_block);
            const auto max_value = std::ranges::max_element(y_block);
            const auto min_diff = std::abs(avg_value - *min_value);
            const auto max_diff = std::abs(avg_value - *max_value);
            y_err_min.push_back(min_diff);
            y_err_max.push_back(max_diff);
            y_min.push_back(*min_value);
            y_max.push_back(*max_value);
        }

        result["Bar count"] = bar_count;
        result["Bin size"] = cp.divisor;
        result["X ranges"] = x_ranges;
        result["Y mean"] = y_mean;
        result["Y err min"] = y_err_min;
        result["Y err max"] = y_err_max;
        result["Y min"] = y_min;
        result["Y max"] = y_max;
        return result;
    }
}

void PrintReports(const std::vector<uint32_t>& buckets, const CheckParameters& cp, const std::string& hash_name) {
    using namespace std::literals;
    std::string file_name = "reports/check_dist/"s + std::to_string(cp.hash_bits) + "/"s +
                            hash_name + " with "s + TestFlagToString(cp.mode) + " mode " + CurrentTime() + ".json"s;

    std::ofstream hash_out(file_name);
    assert(hash_out);
    boost::json::object obj = ProcessingStatistics(buckets, cp, hash_name);
    hash_out << obj;

}

void RunDistTestNormal(std::ostream& log) {
    const auto hashes16 = hfl::Build16bitsHashes();
    const CheckParameters cp16{16, 16, TestFlag::NORMAL};
    DistributionTest(hashes16, cp16, log);

    const auto hashes24 = hfl::Build24bitsHashes();
    const CheckParameters cp24{24, 24, TestFlag::NORMAL};
    DistributionTest(hashes24, cp24, log);
}

void RunDistTestWithBins(std::ostream& log) {
    const auto hashes32 = hfl::Build32bitsHashes();
    const CheckParameters cp32{32, 32, TestFlag::BINS};
    DistributionTest(hashes32, cp32, log);

    const auto hashes64 = hfl::Build64bitsHashes();
    const CheckParameters cp64{64, 64, TestFlag::BINS};
    DistributionTest(hashes64, cp64, log);
}

void RunDistTestWithMask(std::ostream& log) {
    const auto hashes32 = hfl::Build32bitsHashes(hfl::BuildFlag::MASK);
    const CheckParameters cp32{32, 24, TestFlag::MASK};
    DistributionTest(hashes32, cp32, log);

    const auto hashes64 = hfl::Build64bitsHashes(hfl::BuildFlag::MASK);
    const CheckParameters cp64{64, 24, TestFlag::MASK};
    DistributionTest(hashes64, cp64, log);
}

void RunDistributionTests() {
    std::ofstream log("reports/Log Check Dist " + CurrentTime() + ".txt");
    RunDistTestNormal();
    RunDistTestWithBins();
    RunDistTestWithMask();
}