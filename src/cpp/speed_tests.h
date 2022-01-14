#ifndef THESISWORK_SPEED_TESTS_H
#define THESISWORK_SPEED_TESTS_H

#include "boost/format.hpp"
#include "boost/json.hpp"

#include "blocks_generator.h"
#include "log_duration.h"
#include "test_parameters.h"

namespace tests {
    namespace detail {
        OutputJson GetSpeedTestJson(uint16_t hash_bits, size_t num_words, ReportsRoot& reports_root);
    }

    struct HashSpeed {
        std::string name{};
        double sec_time{};
    };

    template<typename HashStruct>
    double InnerLoopSpeedTest(const HashStruct& hs, const std::vector<std::string>& words, ReportsRoot& reports_root);

    template<typename HashStruct>
    double OuterLoopSpeedTest(const HashStruct& hs, const std::vector<std::string>& words, ReportsRoot& reports_root);

    template<typename HashStruct>
    HashSpeed HashSpeedTest(const HashStruct& hs, const std::vector<std::string>& words, ReportsRoot& reports_root);

    template<typename HashStructs>
    void SpeedTests([[maybe_unused]] const HashStructs& hashes, const std::vector<std::string>& words, ReportsRoot& reports_root);

    void RunSpeedTests(uint64_t num_blocks, uint32_t block_length, ReportsRoot& reports_root);

// ====================================

    template<typename HashStruct>
    double InnerLoopSpeedTest(const HashStruct& hs, const std::vector<std::string>& words, ReportsRoot& reports_root) {
        const auto sc_clk_tck = static_cast<long double>(sysconf(_SC_CLK_TCK));

        Timer timer;
        auto sum_hashes = static_cast<uint64_t>(hs.hasher("initial hash"));

        for (const std::string& word : words) {
            timer.Start();
            sum_hashes += static_cast<uint64_t>(hs.hasher(word));
            timer.End();
        }

        std::cout << boost::format("\tsum hashes: %1%\n") % sum_hashes;
        reports_root.logger << boost::format("\tfirst timer: %1%\n") % timer;

        return timer.GetTotalTime();
    }

    template<typename HashStruct>
    double OuterLoopSpeedTest(const HashStruct& hs, const std::vector<std::string>& words, ReportsRoot& reports_root) {
        auto sum_hashes = static_cast<uint64_t>(hs.hasher("initial hash"));

        Timer timer1;
        timer1.Start();
        for (const auto & word : words) {
            sum_hashes += static_cast<uint64_t>(hs.hasher(word));
        }
        timer1.End();

        std::cout << boost::format("\tsum hashes: %1%\n") % sum_hashes;
        uint64_t sum_k = 0;

        Timer timer2;
        timer2.Start();
        for (const auto & word : words) {
            sum_k += (++sum_k);
        }
        timer2.End();


        const double total_time = timer1.GetTotalTime() - timer2.GetTotalTime();
        std::cout << boost::format("\tsum k: %1%\n") % sum_k;
        reports_root.logger << boost::format("\tsecond timer: %1% sec\n") % total_time;

        return total_time;
    }


    template<typename HashStruct>
    HashSpeed HashSpeedTest(const HashStruct& hs, const std::vector<std::string>& words, ReportsRoot& reports_root) {
        LOG_DURATION_STREAM("\tlog duration all time", reports_root.logger);
        reports_root.logger << boost::format("\n%1%:") % hs.name << std::endl;

        const double total_time_1 = InnerLoopSpeedTest(hs, words, reports_root);
        const double total_time_2 = OuterLoopSpeedTest(hs, words, reports_root);
        const double best_total_time = total_time_1 < total_time_2 ? total_time_1 : total_time_2;

        reports_root.logger << boost::format("\tbest timer: %1% sec\n") % best_total_time;
        return HashSpeed{hs.name, best_total_time};
    }

    template<typename HashStructs>
    void SpeedTests(const HashStructs& hashes, const std::vector<std::string>& words, ReportsRoot& reports_root) {
        [[maybe_unused]] const auto hash_bits = hashes.front().bits;

        reports_root.logger << boost::format("start %1% bits") % hash_bits;

        auto out_json = detail::GetSpeedTestJson(hash_bits, words.size(), reports_root);
        boost::json::object speed;
        for (const auto& current_hash : hashes) {
            HashSpeed hs = HashSpeedTest(current_hash, words, reports_root);
            speed[hs.name] = std::to_string(hs.sec_time) + " sec";
        }

        out_json.obj["Speed"] = speed;
        out_json.out << out_json.obj;

        reports_root.logger << boost::format("end %1% bits\n\n") % hash_bits;
    }
}

#endif //THESISWORK_SPEED_TESTS_H
