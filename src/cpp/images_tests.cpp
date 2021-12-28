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
    std::unordered_map<size_t, size_t> hashes;
    for (const auto& dir_entry: std::filesystem::directory_iterator(p)) {
        Image image = LoadJPEG(dir_entry.path());
        auto hash = hash_struct.hasher(image);
        ++hashes[hash];
    }

    report << hash_struct.name << " collisions = " << CountCollisions(hashes) << std::endl;
}

template <typename HashStruct>
void TestImages(const std::vector<HashStruct>& hashes, const std::filesystem::path& p, std::ostream& report) {
    for (const auto& hash_struct : hashes) {
        LOG_DURATION_STREAM(hash_struct.name, std::cout);
        TestImagesInDir(hash_struct, p, report);
    }
}

void RunTests(const filesystem::path& image_dir) {
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

int main(int argc, const char** argv) {
    if (argc != 2) {
        cerr << "Usage: "sv << argv[0] << " <folder with images>"sv << endl;
        return 1;
    }

    filesystem::path image_dir = argv[1];
    RunTests(image_dir);
}