#include <iostream>
#include <random>

#include "words_tests.h"

void CheckDistribution() {/***/}



void RunTests() {
    int test_number = 2;

    while(true) {
        switch (test_number) {
            case 1:
                CheckDistribution();
                return;
            case 2:
                RunTestWithRandomWords();
                return;
            default:
                continue;
        }
    }

}

int main() {
    RunTests();
}

