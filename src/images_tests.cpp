#include <filesystem>
#include <iostream>

#include "count_collisions.h"
#include "img.h"
#include "jpeg_image.h"
#include "hashes.h"
#include "log_duration.h"


using namespace std;
using namespace img;


template <typename HashStruct>
[[maybe_unused]] void TestImagesInDir(const HashStruct& hash_struct, const std::filesystem::path& p, std::ostream& report) {
    std::filesystem::path hash_report_path("reports_/"s + hash_struct.hash_name);
    std::ofstream hash_report_file(hash_report_path);
    std::unordered_map<size_t, size_t> hashes;
    for (const auto& dir_entry: std::filesystem::directory_iterator(p)) {
        Image image = LoadJPEG(dir_entry.path());
        auto hash = hash_struct.hash_function(image);
        ++hashes[hash];
        hash_report_file << dir_entry.path().filename().string() << ": " << hash << std::endl;
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

int main(int argc, const char** argv) {
    filesystem::path image_dir = "/general/Amrulla/Programming/C++/Дипломная работа/Thesis work/data/images";

    std::cout << "Test with 32 bits hashes START" << std::endl;
    const auto hashes_32_bits = hashes::Build32bitsHashes();
    std::ofstream file_report_32_hashes("reports/test_with_images_with_32_bits_hashes.txt");
    TestImages(hashes_32_bits, image_dir, file_report_32_hashes);
    std::cout << "Test with 32 bits hashes END\n" << std::endl;

    std::cout << "Test with 64 bits hashes START" << std::endl;
    const auto hashes_64_bits = hashes::Build64bitsHashes();
    std::ofstream file_report_64_hashes("reports/test_with_images_with_64_bits_hashes.txt");
    TestImages(hashes_64_bits, image_dir, file_report_64_hashes);
    std::cout << "Test with 64 bits hashes END\n" << std::endl;

}