#include <fstream>
#include <iostream>
#include <string>

#include "count_collisions.h"
#include "generators.h"
#include "hashes.h"
#include "log_duration.h"

constexpr int GIGABYTE = 1'073'741'824;

constexpr uint64_t CRASH_DJB2_NUMBER_1 = 2170879;
constexpr uint64_t CRASH_DJB2_NUMBER_2 = 8447;
constexpr uint64_t CRASH_DJB2_NUMBER_3 = 1107165184;
constexpr uint64_t CRASH_DJB2_NUMBER_4 = 555753471;

constexpr uint64_t CRASH_PJW_32_NUMBER = 4095;

constexpr uint64_t CRASH_SFH_NUMBER_1 = 34342961144;
constexpr uint64_t CRASH_SFH_NUMBER_2 = 34376515576;
constexpr uint64_t CRASH_SFH_NUMBER_3 = 68753031152;


template <typename HashStruct>
void CreateDir(const string& path, const std::vector<HashStruct>& hashes) {
    for (const auto& hash_struct : hashes) {
        filesystem::path dir = path + "/" + hash_struct.hash_name;
        filesystem::create_directory(dir);
    }
}

void CreateDirs(const string& path) {
    const auto hashes_32_bits = hashes::Build32bitsHashes();
    CreateDir(path, hashes_32_bits);
    const auto hashes_64_bits = hashes::Build64bitsHashes();
    CreateDir(path, hashes_64_bits);
}

std::vector<std::string> ParseWords(const  filesystem::path& file_name) {
    std::ifstream input(file_name);
    if (!input) {
        assert(false);
    }

    std::vector<std::string> words;
    while (input) {
        string str;
        input >> str;
        words.push_back(std::move(str));
    }
    return words;
}


template <typename HashStruct>
void TestWithWords(const HashStruct& hash_struct, const std::vector<std::string>& random_words,
                   const string& test_name, std::ostream& general_report = std::cout) {
    general_report << std::endl;
    LOG_DURATION_STREAM(hash_struct.hash_name, general_report);
    CollisionsMap hashes;
    for (const std::string& word : random_words) {
        auto hash = hash_struct.hash_function(word);
        CollisionsInfo& info = hashes[hash];
        ++info.count;
        info.words.insert(word);
    }

    std::ofstream local_report {"reports/local_reports/" + hash_struct.hash_name + "/collisions_with_" + test_name + ".txt"};
    general_report << hash_struct.hash_name << " collisions = " << CountCollisions(hashes, local_report) << std::endl;
}

template <typename HashStruct>
void TestsWithWords(const std::vector<HashStruct>& hashes, const std::vector<std::string>& words,
                    const string& test_name, std::ostream& out = std::cout) {
    out << "words: " << words.size() << std::endl;
    for (const auto& hash_struct : hashes) {
        LOG_DURATION_STREAM(hash_struct.hash_name, std::cout);
        TestWithWords(hash_struct, words, test_name, out);
    }
}

template <typename Function>
void TestsWithWordsImpl(Function function, const string& test_name) {
    std::cout << "Build " << test_name << " START" << std::endl;
    const std::vector<std::string> words = function();
    std::cout << "Build " << test_name << " worlds END\n\n";

    std::cout << "Test " << test_name << " with 32 bits hashes START" << std::endl;
    const auto hashes_32_bits = hashes::Build32bitsHashes();
    std::ofstream file_report_32_hashes("reports/report_" + test_name + "_with_32_bits_hashes.txt");
    TestsWithWords(hashes_32_bits, words, test_name, file_report_32_hashes);
    std::cout << "Test " << test_name << " with 32 bits hashes END\n" << std::endl;

    std::cout << "Test " << test_name << " with 64 bits hashes START" << std::endl;
    const auto hashes_64_bits = hashes::Build64bitsHashes();
    std::ofstream file_report_64_hashes("reports/report_" + test_name + "_with_64_bits_hashes.txt");
    TestsWithWords(hashes_64_bits, words, test_name, file_report_64_hashes);
    std::cout << "Test " << test_name << " with 64 bits hashes END\n" << std::endl;
}

void TestWithEnglishWords(const  filesystem::path& file_name) {
    auto lambda = [&file_name]() {
        return ParseWords(file_name);
    };
    TestsWithWordsImpl(lambda, "english_words");
}

void TestsWithRandomWorlds(int word_count, int min_word_size, int max_word_size) {
    std::random_device rd;
    std::mt19937 generator{rd()};

    auto lambda = [&generator, word_count, min_word_size, max_word_size]() {
        return GenerateRandomWords(generator, word_count, min_word_size, max_word_size);
    };

    TestsWithWordsImpl(lambda, "random_words");
}

void TestAvalancheEffect(char ch, int word_size) {
    auto lambda = [ch, word_size]() {
        return GenerateSimilarWord(ch, word_size);
    };

    TestsWithWordsImpl(lambda, "similar_words");
}

void TestCrashPJW(char ch, int word_size) {
    auto lambda = [ch, word_size]() {
        return PJWCrash(ch, word_size);
    };

    TestsWithWordsImpl(lambda, "crash_djb2_pjw_sdbm");
}

void TestCrashDJB2(int word_count, uint64_t crash_number) {
    auto lambda = [word_count, crash_number]() {
        return CrashDJB2(word_count, crash_number);
    };

    TestsWithWordsImpl(lambda, "crash_djb2");
}

template <typename HashStruct>
void TestCycleCrash(const HashStruct& hash_struct, int word_count, uint64_t crash_number,
                    std::ostream& out) {
    auto words = CrashDJB2(word_count, crash_number);
    LOG_DURATION_STREAM(hash_struct.hash_name, std::cout);
    TestWithWords(hash_struct, words, "cycle_crash_" + to_string(crash_number), out);
}

void RunCyclesCrashes32(hashes::BaseHash32Wrapper& hasher, string hash_name, int word_count, int64_t crash_word,
                        std::ostream& out) {

    hashes::Hash32Struct hash_struct{std::move(hash_name), hasher};
    TestCycleCrash(hash_struct, word_count, crash_word, out);
}

void RunCyclesCrashes64(hashes::BaseHash64Wrapper& hasher, string hash_name, int word_count, int64_t crash_word,
                        std::ostream& out) {
    hashes::Hash64Struct hash_struct{std::move(hash_name), hasher};
    TestCycleCrash(hash_struct, word_count, crash_word, out);
}

void RunCyclesCrashes(int word_count) {
    using namespace hashes;
    std::cout << "Run Cycles Crashes START" << std::endl;

    std::ofstream out("reports/report_cycles_crashes");
    RunCyclesCrashes32(djb2_hash_32, DJB2_HASH_32_NAME, word_count, CRASH_DJB2_NUMBER_1, out);
    RunCyclesCrashes32(djb2_hash_32, DJB2_HASH_32_NAME, word_count, CRASH_DJB2_NUMBER_2, out);
    RunCyclesCrashes32(djb2_hash_32, DJB2_HASH_32_NAME, word_count, CRASH_DJB2_NUMBER_3, out);
    RunCyclesCrashes32(djb2_hash_32, DJB2_HASH_32_NAME, word_count, CRASH_DJB2_NUMBER_4, out);

    RunCyclesCrashes64(djb2_hash_64, DJB2_HASH_64_NAME, word_count, CRASH_DJB2_NUMBER_1, out);
    RunCyclesCrashes64(djb2_hash_64, DJB2_HASH_64_NAME, word_count, CRASH_DJB2_NUMBER_2, out);
    RunCyclesCrashes64(djb2_hash_64, DJB2_HASH_64_NAME, word_count, CRASH_DJB2_NUMBER_3, out);
    RunCyclesCrashes64(djb2_hash_64, DJB2_HASH_64_NAME, word_count, CRASH_DJB2_NUMBER_4, out);

    RunCyclesCrashes32(pjw_hash_32, PJW_HASH_32_NAME, word_count, CRASH_PJW_32_NUMBER, out);

    RunCyclesCrashes32(super_fast_hash, SUPER_FAST_HASH_NAME, word_count, CRASH_SFH_NUMBER_1, out);
    RunCyclesCrashes32(super_fast_hash, SUPER_FAST_HASH_NAME, word_count, CRASH_SFH_NUMBER_2, out);
    RunCyclesCrashes32(super_fast_hash, SUPER_FAST_HASH_NAME, word_count, CRASH_SFH_NUMBER_3, out);

    std::cout << "Run Cycles Crashes END\n\n";
}

int main(int argc, const char** argv) {
    int word_count = 1'000'000;
    int min_word_size = 100;
    int max_word_size = 100;

    CreateDirs("reports/local_reports");

    assert(word_count * max_word_size < GIGABYTE);
    //TestWithEnglishWords("data/english_words.txt");
    //TestsWithRandomWorlds(word_count, min_word_size, max_word_size);

    //TestAvalancheEffect('a', max_word_size);
    //TestCrashPJW('a', max_word_size);
    //TestCrashDJB2(word_count, CRASH_DJB2_NUMBER_1);

    RunCyclesCrashes(word_count);
}

