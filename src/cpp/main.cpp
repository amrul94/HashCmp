#include "generated_tests.h"
#include "distribution_tests.h"
#include "english_tests.h"
#include "log_duration.h"
#include "speed_tests.h"

#include <boost/format.hpp>
#include <boost/container/static_vector.hpp>

#include <pcg_random.hpp>
#include <random>
#include <algorithm>
#include <execution>
#include <set>

#include "hashes.h"

void TempTests(tests::ReportsRoot& report_root) {

}



void RunTests(const std::vector<int>& test_numbers, tests::ReportsRoot& reports_root) {

    for (int test_number : test_numbers) {
        switch (test_number) {
            case 1:
                reports_root.logger << "start DistributionTest\n\n";
                tests::RunDistributionTests(reports_root);
                reports_root.logger << "end DistributionTest\n\n\n";
                break;
            case 2:
                reports_root.logger << "start generated blocks test (length = 16)\n\n";
                tests::RunTestWithGeneratedBlocks(16, reports_root);
                reports_root.logger << "end generated blocks test (length = 16)\n\n\n";
                break;
            case 3:
                reports_root.logger << "start generated blocks test (length = 4096)\n\n";
                tests::RunTestWithGeneratedBlocks(FOUR_KILOBYTES, reports_root);
                reports_root.logger << "end generated blocks test (length = 4096)\n\n\n";
                break;
            case 4:
                reports_root.logger << "start english words test\n\n";
                tests::RunTestWithEnglishWords(reports_root);
                reports_root.logger << "end english words test\n\n\n";
                break;
            case 5:
                reports_root.logger << "start speed test\n\n";
                tests::RunSpeedTests(200'000, FOUR_KILOBYTES, reports_root);
                reports_root.logger << "end speed test\n\n\n";
                break;
            default:
                TempTests(reports_root);
                break;
        }
    }
}

tests::ReportsRoot CreateReportsRoot() {
    const auto current_path = std::filesystem::current_path();
    const std::filesystem::path reports_dir{"reports"};
    const auto reports_path = current_path / reports_dir / CurrentTime() / "cpp";
    std::filesystem::create_directories(reports_path);
    return tests::ReportsRoot{reports_path};
}

int main() {
    const std::vector test_numbers{1, 2, 3, 4, 5};
    tests::ReportsRoot reports_root = CreateReportsRoot();

    RunTests(test_numbers, reports_root);
}

