#include "collision_tests.h"

#include <random>

#include <pcg_random.hpp>

#include "hashes.h"

uint64_t CountCollisions(const std::vector<std::uint32_t>& hashes) {
    uint64_t collisions = 0;
    for (const auto& count : hashes) {
        if (count > 1) {
            collisions += (count - 1);
        }
    }
    return collisions;
}

uint64_t CountCollisions(const std::map<uint64_t, uint64_t>& hashes) {
    uint64_t collisions = 0;
    for (const auto& [hash, count] : hashes) {
        if (count > 1) {
            collisions += (count - 1);
        }
    }
    return collisions;
}

std::vector<std::string> ParseWords(const std::filesystem::path& file_name) {
    std::ifstream input(file_name);
    assert(input);

    std::vector<std::string> words;
    while (input) {
        std::string str;
        input >> str;
        words.push_back(std::move(str));
    }
    return words;
}

#define RUN_TEST_WITH_ENG_WORDS_IMPL(WORDS, BITS, ROOT)                         \
    const auto hashes##BITS = hfl::Build##BITS##bitsHashes();                   \
    const TestParameters tp##BITS{BITS, static_cast<uint64_t>((WORDS).size())}; \
    TestWithEnglishWords(hashes##BITS, WORDS, tp##BITS, ROOT)                   \


void RunTestWithEnglishWords(ReportsRoot& reports_root) {
    const std::filesystem::path& file_name = "data/english_words.txt";
    const std::vector<std::string> words = ParseWords(file_name);

    RUN_TEST_WITH_ENG_WORDS_IMPL(words, 16, reports_root);
    RUN_TEST_WITH_ENG_WORDS_IMPL(words, 24, reports_root);
    RUN_TEST_WITH_ENG_WORDS_IMPL(words, 32, reports_root);
    RUN_TEST_WITH_ENG_WORDS_IMPL(words, 48, reports_root);
    RUN_TEST_WITH_ENG_WORDS_IMPL(words, 64, reports_root);
}

std::string UintToString(uint64_t src, uint64_t size) {
    std::string dest;
    dest.resize(size);
    std::memcpy(dest.data(), &src, size);
    return dest;
}

std::string GenerateRandomDataBlockSeq(pcg64& generator, uint32_t length) {
    std::string word;
    word.reserve(length);

    uint64_t chunks = length / sizeof(uint64_t);
    for (uint64_t i = 0; i < chunks; ++i) {
        word += UintToString(generator(), sizeof(uint64_t));
    }

    uint64_t diff = length - chunks * sizeof(uint64_t);
    word += UintToString(generator(), diff);

    assert(word.size() == length);
    return word;
}

namespace {
    using GenerateTask = std::packaged_task<std::string(pcg64&, uint32_t)>;

    std::vector<GenerateTask> CreateTasks(size_t num_threads) {
        std::vector<GenerateTask> tasks(num_threads - 1);
        for (auto& task : tasks) {
            task = GenerateTask(GenerateRandomDataBlockSeq);
        }
        return tasks;
    }

    std::vector<std::future<std::string>> GetFutures(std::vector<GenerateTask>& tasks) {
        std::vector<std::future<std::string>> results(tasks.size());
        for (int i = 0; i < tasks.size(); ++i) {
            results[i] = tasks[i].get_future();
        }
        return results;
    }

    std::vector<std::thread> StartThreads(std::vector<pcg64>& generators, std::vector<GenerateTask>& tasks,
                                          uint32_t chunk_length) {
        std::vector<std::thread> threads(tasks.size());
        for (int i = 0; i < tasks.size(); ++i) {
            threads[i] = std::thread(std::move(tasks[i]), std::ref(generators[i]), chunk_length);
        }
        return threads;
    }

    void JoinThreads(std::vector<std::thread>& threads) {
        for (auto & thread : threads) {
            thread.join();
        }
    }

    std::string GetFirstResults(std::vector<std::future<std::string>>& results) {
        std::string result;
        for (auto & i : results) {
            result += i.get();
        }
        return result;
    }
}

std::string GenerateRandomDataBlockPar(std::vector<pcg64>& generators, uint32_t length) {
    const size_t num_threads = generators.size();
    const uint32_t chunk_length = length / num_threads;

    auto tasks = CreateTasks(num_threads);
    auto results = GetFutures(tasks);
    auto threads = StartThreads(generators, tasks, chunk_length);

    const uint32_t last_chunk_length = length - chunk_length * threads.size();
    std::string last_result = GenerateRandomDataBlockSeq(generators.back(), last_chunk_length);

    JoinThreads(threads);
    std::string result = GetFirstResults(results) + std::move(last_result);

    assert(result.size() == length);
    return result;
}

#define RUN_COLL_TEST_NORMAL_IMPL(GENS, BITS, POW_COUNTS, LENGTH, NUM_THREADS, MODE, ROOT)  \
    const auto word_counts##BITS = static_cast<uint64_t>(pow(2, POW_COUNTS));               \
    const GenBlocksParameters wp##BITS {BITS, word_counts##BITS, LENGTH, NUM_THREADS, MODE};    \
    const auto hashes##BITS = hfl::Build##BITS##bitsHashes(hfl::BuildFlag::ALL);            \
    TestWithGeneratedBlocks(GENS, hashes##BITS, wp##BITS, ROOT)

void RunCollTestNormal(const std::vector<pcg64>& generators, uint16_t words_length, size_t num_threads,
                       ReportsRoot& reports_root) {
    RUN_COLL_TEST_NORMAL_IMPL(generators, 16, 16, words_length, num_threads, TestFlag::NORMAL, reports_root);
    RUN_COLL_TEST_NORMAL_IMPL(generators, 24, 24, words_length, num_threads, TestFlag::NORMAL, reports_root);
    RUN_COLL_TEST_NORMAL_IMPL(generators, 32, 24, words_length, num_threads, TestFlag::NORMAL, reports_root);
}

#define RUN_COLL_TEST_WITH_MASK_IMPL(GENS, BITS, POW_COUNTS, LENGTH, NUM_THREADS, MODE, ROOT)       \
    const auto word_counts##BITS = static_cast<uint64_t>(pow(2, POW_COUNTS));                       \
    const GenBlocksParameters wp##BITS {BITS, 32, word_counts##BITS, LENGTH, NUM_THREADS, MODE};    \
    const auto hashes##BITS = hfl::Build##BITS##bitsHashes(hfl::BuildFlag::ALL);                    \
    TestWithGeneratedBlocks(GENS, hashes##BITS, wp##BITS, ROOT)

void RunCollTestWithMask(const std::vector<pcg64>& generators, uint16_t words_length, size_t num_threads,
                       ReportsRoot& reports_root) {
    RUN_COLL_TEST_WITH_MASK_IMPL(generators, 48, 24, words_length, num_threads, TestFlag::MASK, reports_root);
    RUN_COLL_TEST_WITH_MASK_IMPL(generators, 64, 24, words_length, num_threads, TestFlag::MASK, reports_root);
}

std::vector<pcg64> GetGenerators(uint16_t words_length) {
    const size_t hardware_threads = std::thread::hardware_concurrency();
    const size_t num_threads = (hardware_threads != 0 && words_length > 1024) ? hardware_threads : 1;
    //const size_t num_threads = hardware_threads != 0 ? hardware_threads : 1;
    std::vector<pcg64> generators(num_threads);

    for (auto& generator : generators) {
        pcg_extras::seed_seq_from<std::random_device> seed_source;
        generator.seed(seed_source);
    }

    std::cout << boost::format("num_threads = %1%\n") % num_threads;
    return generators;
}

void RunTestWithGeneratedBlocks(uint16_t words_length, ReportsRoot& reports_root) {
    std::vector<pcg64> generators = GetGenerators(words_length);

    RunCollTestNormal(generators, words_length, generators.size(), reports_root);
    RunCollTestWithMask(generators, words_length, generators.size(), reports_root);
}