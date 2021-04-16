#include "tests_with_images.h"

#include <iostream>


void RunAllImagesTests() {
    std::filesystem::path directory("data/images/look.com.ua_2015.09-111-1920x1080");

    std::cout << "Test with 32 bits hashes START" << std::endl;
    const auto hashes_32_bits = hashes::Build32bitsHashes();
    std::ofstream file_report_32_hashes("reports/test_with_images_with_32_bits_hashes.txt");
    TestImages(hashes_32_bits, directory, file_report_32_hashes);
    std::cout << "Test with 32 bits hashes END\n" << std::endl;

    std::cout << "Test with 64 bits hashes START" << std::endl;
    const auto hashes_64_bits = hashes::Build64bitsHashes();
    std::ofstream file_report_64_hashes("reports/test_with_images_with_64_bits_hashes.txt");
    TestImages(hashes_64_bits, directory, file_report_64_hashes);
    std::cout << "Test with 64 bits hashes END\n" << std::endl;
}
