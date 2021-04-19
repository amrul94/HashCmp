#include <fstream>
#include <iostream>

#include "london_crime_tests.h"
#include "random_world_tests.h"
#include "tests_with_images.h"
#include "hashes.h"
#include <ranges>

std::string ReadFile() {
    std::string result;
    size_t source_size = 0;
    ifstream in("data/look.com.ua-114349.jpg", ios::binary);
    do {
        char buff[1024];
        in.read(buff, sizeof buff);
        size_t read_size = in.gcount();
        source_size += read_size;
        result.append(buff, read_size);
    } while (in);
    in.close();
    return result;
}



int main() {
    RunAllRandomTests();
    //RunAllImagesTests();
    /*auto dir = std::filesystem::path("data/images/look.com.ua_2015.02-111-1366x768");
    int size = 0;
    for (const auto& dir_entry: std::filesystem::directory_iterator(dir)) {
        ++size;
    }
    std::cout << size << std::endl;
*/
/*
    std::string image = ReadFile();
    assert(!image.empty());
    //std::string image  = "image.jpg";
    const auto hashes32 = HashLib::Build32bitsHashes();
    for (const auto& [name, hash] : hashes32 ) {
        std::cout << std::endl;
        LOG_DURATION_STREAM("time", std::cout);
        std::cout << name << ": " << hash(image) << std::endl;
        assert(hash(image) == hash(image));
    }
    std::cout << std::endl;

    const auto hashes64 = HashLib::Build64bitsHashes();
    for (const auto& [name, hash] : hashes64) {
        std::cout << std::endl;
        LOG_DURATION_STREAM("time", std::cout);
        std::cout << name << ": " << hash(image) << std::endl;
        assert(hash(image) == hash(image));
    }
    using namespace std::literals;
*/
}
