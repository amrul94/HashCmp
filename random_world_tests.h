//
// Created by amrulla on 17.03.2021.
//

#ifndef THESIS_WORK_RANDOM_WORLD_TESTS_H
#define THESIS_WORK_RANDOM_WORLD_TESTS_H

#include <functional>
#include <random>
#include <string>


std::string GenerateWord(std::mt19937& generator, int min_length, int max_length);

std::vector<std::string> GenerateWords(std::mt19937& generator, int word_count, int min_length, int max_length);

void TestWithRandomWords(const hash_wrappers::Hasher& hasher, const std::vector<std::string>& random_words,
                         std::ostream& out = std::cout);


void RunConcreteRandomTest(const std::vector<hash_wrappers::Hasher>& hashes, const std::vector<std::string>& words,
                           std::ostream& out = std::cout);
void RunAllRandomTests();

#endif //THESIS_WORK_RANDOM_WORLD_TESTS_H
