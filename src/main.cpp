#include "collision_tests.h"
#include "distribution_tests.h"
#include "log_duration.h"

#include <pcg_random.hpp>
#include <algorithm>
#include <execution>

void TempTests() {
    std::cout << std::thread::hardware_concurrency() << std::endl;
}



void RunTests(const std::vector<int>& test_numbers, ReportsRoot& reports_root) {

    for (int test_number : test_numbers) {
        switch (test_number) {
            case 1:
                reports_root.log << "start DistributionTest" << std::endl;
                RunDistributionTests(reports_root);
                reports_root.log << "end DistributionTest" << std::endl;
                break;
            case 2:
                reports_root.log << "start WordsTest 1" << std::endl;
                RunTestWithGeneratedBlocks(16, reports_root);
                reports_root.log << "start WordsTest 2" << std::endl;
                RunTestWithGeneratedBlocks(FOUR_KILOBYTES, reports_root);
                reports_root.log << "end WordsTests 1 and 2" << std::endl;
                break;
            case 3:
                reports_root.log << "start WordsTest 3" << std::endl;
                RunTestWithEnglishWords(reports_root);
                reports_root.log << "end WordsTests 3" << std::endl;
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
    const auto reports_path = current_path / reports_dir / CurrentTime();
    std::filesystem::create_directories(reports_path);
    return ReportsRoot{reports_path};
}

void TryCreateDirectory(const std::filesystem::path& path, std::ostream& log) {
    while (true) {
        try {
            std::filesystem::create_directories(path);
            log << "create \"" << path << '\"' << std::endl;
            return;
        }
        catch (std::exception& e){
            log << e.what() << std::endl;
        }
    }
}

int main() {
    const std::vector test_numbers{1, 2, 3};
    ReportsRoot reports_root = CreateReportsRoot();
    assert(reports_root.log);
    RunTests(test_numbers, reports_root);
}

