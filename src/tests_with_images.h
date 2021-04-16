#ifndef THESIS_WORK_TESTS_WITH_IMAGES_H
#define THESIS_WORK_TESTS_WITH_IMAGES_H

#include <fstream>
#include <filesystem>

#include "hashes.h"
#include "log_duration.h"
#include "tests.h"

template <typename HashStruct>
[[maybe_unused]] void TestImagesInDir(const HashStruct& hash_struct, const std::filesystem::path& p, std::ostream& report) {
    std::filesystem::path hash_report_path("reports/"s + hash_struct.hash_name);
    std::ofstream hash_report_file(hash_report_path);
    std::unordered_map<size_t, size_t> hashes;
    for (const auto& dir_entry: std::filesystem::directory_iterator(p)) {
        ifstream file(dir_entry.path(), ios::binary);
        ++hashes[hash_struct.hash_function(file)];
        hash_report_file << dir_entry.path().filename().string() << ": " << hash_struct.hash_function(file) << std::endl;
    }

    report << hash_struct.hash_name << " collisions = " << CountCollisions(hashes) << std::endl;
}

template <typename HashStruct>
void TestImages(const std::vector<HashStruct>& hashes, const std::filesystem::path& p, std::ostream& report) {
    for (const auto& hash_struct : hashes) {
        LOG_DURATION_STREAM(hash_struct.hash_name, std::cout);
        TestImagesInDir(hash_struct, p, report);
    }
}

[[maybe_unused]] void RunAllImagesTests();

#endif //THESIS_WORK_TESTS_WITH_IMAGES_H
