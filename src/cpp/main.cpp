#include "collision_tests.h"
#include "distribution_tests.h"
#include "log_duration.h"

#include <boost/format.hpp>
#include <boost/container/static_vector.hpp>

#include <pcg_random.hpp>
#include <random>
#include <algorithm>
#include <execution>
#include <set>

#include "hashes.h"

template <typename Type>
class [[maybe_unused]] DJB2HashWrapper : public hfl::detail::BaseHashWrapper<Type> {
private:
    [[nodiscard]] Type Hash(std::string_view str) const override {
        return DJB2Hash<Type>(str);
    }
};

void TempTests() {
    std::cerr << "TempTests\n";
    const auto num_hashes = static_cast<uint64_t>(std::pow(2, 32));
    /*std::vector<std::uint16_t> vec (num_hashes);
    std::mutex m;
    auto lambda = [&vec, &m](uint64_t start, uint64_t end){
        {
            std::lock_guard guard{m};
            std::cout << "start: " << start << std::endl;
            std::cout << "end: " << end << std::endl;
        }

        for (uint64_t i = start; i < end; ++i) {
            auto hash = hfl::murmur_hash1(i);
            std::lock_guard guard{m};
            vec[hash] = (hash != UINT16_MAX) ? (vec[hash] + 1) : vec[hash];
        }
    };
    const auto count = static_cast<uint64_t>(std::pow(2, 32));
    LOG_DURATION("atomic " + std::to_string(count));
    uint64_t start = 0;
    uint64_t step = count / 4;
    std::jthread jt1(lambda, start, start + step); start += step;
    std::jthread jt2(lambda, start, start + step); start += step;
    std::jthread jt3(lambda, start, start + step); start += step;
    lambda(start, start + step);*/
    //lambda(0, count);
}



void RunTests(const std::vector<int>& test_numbers, ReportsRoot& reports_root) {

    for (int test_number : test_numbers) {
        switch (test_number) {
            case 1:
                std::cout << "start DistributionTest\n\n";
                RunDistributionTests(reports_root);
                std::cout << "end DistributionTest\n\n\n";
                break;
            case 2:
                std::cout << "start generated blocks test (length = 16)\n\n";
                RunTestWithGeneratedBlocks(16, reports_root);
                std::cout << "end generated blocks test (length = 16)\n\n\n";
                break;
            case 3:
                std::cout << "start generated blocks test (length = 4096)\n\n";
                RunTestWithGeneratedBlocks(KILOBYTE, reports_root);
                std::cout << "end generated blocks test (length = 4096)\n\n\n";
            case 4:
                std::cout << "start english words test\n\n";
                RunTestWithEnglishWords(reports_root);
                std::cout << "end english words test\n\n\n";
                break;
            default:
                TempTests();
                break;
        }
    }
}

ReportsRoot CreateReportsRoot() {
    const auto current_path = std::filesystem::current_path();
    const std::filesystem::path reports_dir{"reports"};
    const auto reports_path = current_path / reports_dir / CurrentTime() / "cpp";
    std::filesystem::create_directories(reports_path);
    return ReportsRoot{reports_path};
}

int main() {
    const std::vector test_numbers{4};
    ReportsRoot reports_root = CreateReportsRoot();
    RunTests(test_numbers, reports_root);
}

