#ifndef THESIS_WORK_GENERATORS_H
#define THESIS_WORK_GENERATORS_H


#include <random>
#include <string>
#include <vector>

std::string GenerateWord(std::mt19937& generator, int min_length, int max_length);

std::vector<std::string> GenerateWords(std::mt19937& generator, int word_count, int min_length, int max_length);

#endif //THESIS_WORK_GENERATORS_H
