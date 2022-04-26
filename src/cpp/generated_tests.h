#ifndef THESIS_WORK_WORDS_TESTS_H
#define THESIS_WORK_WORDS_TESTS_H

#include <barrier>
#include <deque>
#include <execution>
#include <fstream>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include <boost/format.hpp>
#include <boost/json.hpp>

#include <pcg_random.hpp>

#include "concurrency.h"
#include "generators.h"
#include "timers.h"
#include "output.h"
#include "test_parameters.h"
#include "hashes.h"

constexpr int KILOBYTE = 1024;
constexpr int FOUR_KILOBYTES = KILOBYTE * 4;

namespace tests {
    namespace out {
        OutputJson GetGenTestJson(const GenBlocksParameters& gbp, out::Logger& logger);
    }

    template<hfl::UnsignedIntegral UintT>
    auto HashTestWithGenBlocks(std::vector<pcg64>& generators, const hfl::Hash<UintT>& hs, const GenBlocksParameters& gbp,
                               out::Logger& logger);

    template<hfl::UnsignedIntegral UintT>
    void TestWithGeneratedBlocks(std::vector<pcg64>& generators, const std::vector<hfl::Hash<UintT>>& hashes,
                                 const GenBlocksParameters& gbp, out::Logger& logger);


    void RunCollTestNormal(uint16_t words_length, uint16_t num_threads, out::Logger& logger);

    void RunCollTestWithMask(uint16_t words_length, uint16_t num_threads, out::Logger& logger);

    void RunTestWithGeneratedBlocks(uint16_t words_length, out::Logger& logger);


    // ===============================================================================


    // Эту функцию думаю в будущем разбить на части
    template<hfl::UnsignedIntegral UintT>
    auto HashTestWithGenBlocks(const hfl::Hash<UintT>& hasher, const GenBlocksParameters& gbp, out::Logger& logger) {
        out::LogDuration log_duration("\t\ttime", logger);
        logger << boost::format("\n\t%1%: \n") % hasher.GetName();

        std::vector<pcg64> generators = GetGenerators(gbp.num_threads, (gbp.num_keys * gbp.words_length) / 8);   \
        // Выделить coll_flags, collisions, num_collisions и мьютекс (в будущем) в отдельный класс (например, Counters).
        // При этом подсчет коллизий вынести в метод класса AddHash
        const auto num_hashes = static_cast<uint64_t>(std::pow(2, gbp.mask_bits));
        std::deque<std::atomic_bool> coll_flags(num_hashes);
        boost::json::object collisions;
        std::atomic_uint64_t num_collisions = 0;

        size_t num_words = (1 << (gbp.mask_bits >> 1));
        size_t step = (gbp.mask_bits != 24) ? 1 : 2;

        bool loop_conditional = (num_words <= gbp.num_keys);
        auto completion_lambda =
                [&logger, &num_words, &num_collisions, &collisions, &loop_conditional, &gbp, step] () {
            logger << boost::format("\t\t%1% words:\t%2% collisions\n") % num_words % num_collisions;
            std::string index = std::to_string(num_words);
            collisions[index] = num_collisions;
            num_words <<= step;
            loop_conditional = (num_words <= gbp.num_keys);
        };

        std::atomic_size_t gen_index = 0;
        std::barrier sync_point(gbp.num_threads, completion_lambda);

        auto thread_task = [&loop_conditional, &num_words, &gbp, &generators, &gen_index, &hasher, &coll_flags,
                            &num_collisions, &sync_point] () {
            pcg64& rng = generators[gen_index++];
            for (size_t i = 0; loop_conditional; ) {
                size_t thread_num_words = num_words / gbp.num_threads;
                for (; i < thread_num_words; ++i) {
                    std::string str = GenerateRandomDataBlock(rng, gbp.words_length);
                    const auto hash = static_cast<uint64_t>(hasher(str));
                    const uint64_t modified = ModifyHash(gbp, hash);
                    num_collisions += coll_flags[modified].exchange(true);
                }
                sync_point.arrive_and_wait();
            }
        };

        std::vector<std::thread> threads(gbp.num_threads - 1);
        for (auto& t : threads) {
            t = std::thread(thread_task);
        }
        thread_task();
        for (auto& t : threads) {
            t.join();
        }

        auto hash_name = static_cast<boost::json::string>(hasher.GetName());
        if (gbp.mode == TestFlag::MASK) {
            hash_name += " (mask " + std::to_string(gbp.mask_bits) + " bits)";
        }
        boost::json::object result;
        result[hash_name] = collisions;
        return std::pair{std::move(hash_name), std::move(collisions)};
    }

    // Возможно стоит поделить эту функции на части, так как она очень большая
    template<hfl::UnsignedIntegral UintT>
    void TestWithGeneratedBlocks(const std::vector<hfl::Hash<UintT>>& hashes, const GenBlocksParameters& gbp, out::Logger& logger) {
        logger << "--- START " << gbp.hash_bits << " BITS TEST ---" << std::endl;

        auto out_json = out::GetGenTestJson(gbp, logger);

        boost::json::object collisions;
        std::mutex local_mutex;

        for (const auto& hasher : hashes) {
            auto [hash_name, counters] = HashTestWithGenBlocks(hasher, gbp, logger);
            collisions[hash_name] = std::move(counters);
        }

        out_json.obj["Collisions"] = collisions;
        out_json.out << out_json.obj;

        logger << "\n--- END " << gbp.hash_bits << " BITS TEST ---" << std::endl << std::endl;
    }
}

#endif //THESIS_WORK_WORDS_TESTS_H