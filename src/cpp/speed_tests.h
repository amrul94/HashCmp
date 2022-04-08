#ifndef THESISWORK_SPEED_TESTS_H
#define THESISWORK_SPEED_TESTS_H

#include "boost/format.hpp"
#include "boost/json.hpp"

#include "generator.h"
#include "log_duration.h"
#include "test_parameters.h"

namespace tests {
    namespace out {
        OutputJson GetSpeedTestJson(uint16_t hash_bits, size_t num_words, ReportsRoot& reports_root);
    }

    struct HashSpeed {
        std::string name{};
        double sec_time{};
    };

    template<typename HashFunc>
    double InnerLoopSpeedTest(HashFunc func, std::string_view hash_name, const std::vector<std::string>& words,
                              ReportsRoot& reports_root);

    template<typename HashFunc>
    double OuterLoopSpeedTest(HashFunc func, std::string_view hash_name, const std::vector<std::string>& words,
                              ReportsRoot& reports_root);

    template<typename HashFunc>
    HashSpeed HashSpeedTest(HashFunc func, std::string_view hash_name, const std::vector<std::string>& words,
                            ReportsRoot& reports_root);

    [[maybe_unused]] boost::json::object SpeedTests16(const std::vector<std::string>& words, ReportsRoot& reports_root);

    [[maybe_unused]] boost::json::object SpeedTests24(const std::vector<std::string>& words, ReportsRoot& reports_root);

    [[maybe_unused]] boost::json::object SpeedTests32(const std::vector<std::string>& words, ReportsRoot& reports_root);

    [[maybe_unused]] boost::json::object SpeedTests48(const std::vector<std::string>& words, ReportsRoot& reports_root);

    [[maybe_unused]] boost::json::object SpeedTests64(const std::vector<std::string>& words, ReportsRoot& reports_root);

    template<typename HashStructs>
    void SpeedTests(const HashStructs& hashes, const std::vector<std::string>& words, ReportsRoot& reports_root);

    void RunSpeedTests(uint64_t num_blocks, uint32_t block_length, ReportsRoot& reports_root);

// ====================================

    template<typename HashFunc>
    double InnerLoopSpeedTest(HashFunc func, std::string_view hash_name, const std::vector<std::string>& words,
                              ReportsRoot& reports_root) {
        Timer timer;
        auto sum_hashes = static_cast<uint64_t>(func("initial hash"));

        for (const std::string& word : words) {
            timer.Start();
            sum_hashes += static_cast<uint64_t>(func(word));
            timer.End();
        }

        std::cout << boost::format("\t\tsum hashes: %1%\n") % sum_hashes;
        reports_root.logger << boost::format("\t\tfirst timer: %1%\n") % timer;

        return timer.GetTotalTime();
    }

    template<typename HashFunc>
    double OuterLoopSpeedTest(HashFunc func, std::string_view hash_name, const std::vector<std::string>& words,
                              ReportsRoot& reports_root) {
        auto sum_hashes = static_cast<uint64_t>(func("initial hash"));

        Timer timer1;
        timer1.Start();
        for (const auto & word : words) {
            sum_hashes += static_cast<uint64_t>(func(word));
        }
        timer1.End();

        std::cout << boost::format("\t\tsum hashes: %1%\n") % sum_hashes;
        uint64_t sum_k = 0;

        Timer timer2;
        timer2.Start();
        for (const auto & word : words) {
            sum_k += (++sum_k);
        }
        timer2.End();


        const double total_time = timer1.GetTotalTime() - timer2.GetTotalTime();
        std::cout << boost::format("\t\tsum k: %1%\n") % sum_k;
        reports_root.logger << boost::format("\t\tsecond timer: %1% sec\n") % total_time;

        return total_time;
    }


    template<typename HashFunc>
    HashSpeed HashSpeedTest(HashFunc func, std::string_view hash_name, const std::vector<std::string>& words,
                            ReportsRoot& reports_root) {
        LOG_DURATION_STREAM("\t\tlog duration all time", reports_root.logger);
        reports_root.logger << boost::format("\n\t%1%:\n") % hash_name;

        const double total_time_1 = InnerLoopSpeedTest(func, hash_name, words, reports_root);
        const double total_time_2 = OuterLoopSpeedTest(func, hash_name, words, reports_root);
        const double best_total_time = total_time_1 < total_time_2 ? total_time_1 : total_time_2;

        reports_root.logger << boost::format("\t\tbest timer: %1% sec\n") % best_total_time;
        return HashSpeed{std::string{hash_name}, best_total_time};
    }
}

#endif //THESISWORK_SPEED_TESTS_H
