#ifndef THESISWORK_SPEED_TESTS_H
#define THESISWORK_SPEED_TESTS_H

#include <sys/times.h>

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
        clock_t tck_time{};
        double sec_time{};
    };

    template<typename HashStruct>
    clock_t InnerLoopSpeedTest(const HashStruct& hs, const std::vector<std::string>& words, ReportsRoot& reports_root);

    template<typename HashStruct>
    clock_t OuterLoopSpeedTest(const HashStruct& hs, const std::vector<std::string>& words, ReportsRoot& reports_root);

    template<typename HashStruct>
    HashSpeed HashSpeedTest(const HashStruct& hs, const std::vector<std::string>& words, ReportsRoot& reports_root);

    template<typename HashStructs>
    void SpeedTests(const HashStructs& hashes, const std::vector<std::string>& words, ReportsRoot& reports_root);

    void RunSpeedTests(uint64_t num_blocks, uint32_t block_length, ReportsRoot& reports_root);


// ====================================

    template<typename HashStruct>
    clock_t InnerLoopSpeedTest(const HashStruct& hs, const std::vector<std::string>& words, ReportsRoot& reports_root) {
        const auto sc_clk_tck = static_cast<long double>(sysconf(_SC_CLK_TCK));

        tms tms_time{};
        clock_t delta_t = 0;
        auto sum_hashes = static_cast<uint64_t>(hs.hasher("initial hash"));

        for (const std::string& word : words) {
            clock_t t1 = times(&tms_time);
            sum_hashes += static_cast<uint64_t>(hs.hasher(word));
            clock_t t2 = times(&tms_time);
            delta_t += t2 - t1;
        }

        std::cout << boost::format("\tsum hashes: %1%\n") % sum_hashes;
        reports_root.logger << boost::format("\tfirst timer: %1% tck (%2% s)\n") % delta_t % (delta_t / sc_clk_tck);

        return delta_t;
    }

    template<typename HashStruct>
    clock_t OuterLoopSpeedTest(const HashStruct& hs, const std::vector<std::string>& words, ReportsRoot& reports_root) {
        const auto sc_clk_tck = static_cast<long double>(sysconf(_SC_CLK_TCK));


        tms tms_time{};
        clock_t delta_t = 0;
        auto sum_hashes = static_cast<uint64_t>(hs.hasher("initial hash"));

        clock_t t1 = times(&tms_time);
        for (const auto & word : words) {
            sum_hashes += static_cast<uint64_t>(hs.hasher(word));
        }
        clock_t t2 = times(&tms_time);

        std::cout << boost::format("\tsum hashes: %1%\n") % sum_hashes;
        uint64_t sum_k = 0;

        clock_t t3 = times(&tms_time);
        for (const auto & word : words) {
            sum_k += (++sum_k);
        }
        clock_t t4 = times(&tms_time);

        delta_t = (t2 - t1) - (t4 - t3);
        std::cout << boost::format("\tsum k: %1%\n") % sum_k;
        reports_root.logger << boost::format("\tsecond timer: %1% tck (%2% s)\n") % delta_t % (delta_t / sc_clk_tck);

        return delta_t;
    }


    template<typename HashStruct>
    HashSpeed HashSpeedTest(const HashStruct& hs, const std::vector<std::string>& words, ReportsRoot& reports_root) {
        LOG_DURATION_STREAM("\tlog duration all time", reports_root.logger);
        reports_root.logger << boost::format("\n%1%:") % hs.name << std::endl;

        const auto sc_clk_tck = static_cast<long double>(sysconf(_SC_CLK_TCK));
        const clock_t delta_t1 = InnerLoopSpeedTest(hs, words, reports_root);
        const clock_t delta_t2 = OuterLoopSpeedTest(hs, words, reports_root);
        const clock_t tck_best_timer = delta_t1 < delta_t2 ? delta_t1 : delta_t2;

        long double sec_best_timer = (tck_best_timer / sc_clk_tck);
        reports_root.logger << boost::format("\tbest timer: %1% tck (%2% s)\n") % tck_best_timer % sec_best_timer;
        return HashSpeed{hs.name, tck_best_timer, static_cast<double>(sec_best_timer)};
    }

    template<typename HashStructs>
    void SpeedTests(const HashStructs& hashes, const std::vector<std::string>& words, ReportsRoot& reports_root) {
        const auto hash_bits = hashes.front().bits;

        reports_root.logger << boost::format("start %1% bits") % hash_bits;

        auto out_json = detail::GetSpeedTestJson(hash_bits, words.size(), reports_root);
        boost::json::object speed;
        for (const auto& current_hash : hashes) {
            HashSpeed hs = HashSpeedTest(current_hash, words, reports_root);
            boost::json::object jhs;
            jhs["Tck time"] = hs.tck_time;
            jhs["Sec time"] = hs.sec_time;
            speed[hs.name] = jhs;
        }

        out_json.obj["Speed"] = speed;
        out_json.out << out_json.obj;

        reports_root.logger << boost::format("end %1% bits\n\n") % hash_bits;
    }
}

#endif //THESISWORK_SPEED_TESTS_H
