#include <fstream>
#include <iostream>

#include "london_crime_tests.h"
#include "random_world_tests.h"

#include "hashes/t1ha/t1ha.h"

int main() {
    //RunLondonCrimeTests();

    RunAllRandomTests();

    std::string test_string = "Test string";
    std::string test_string2 = "New testing";
    //std::cout << test_string << " -> " << t1ha2_atonce(test_string.c_str(), test_string.size(), 0) << std::endl;


}
