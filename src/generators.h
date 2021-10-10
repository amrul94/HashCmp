#ifndef THESIS_WORK_GENERATORS_H
#define THESIS_WORK_GENERATORS_H

#include <array>
#include <random>
#include <string>
#include <vector>

std::string GenerateRandomWord(std::mt19937& generator, uint32_t min_length, uint32_t max_length);

std::vector<std::string> GenerateRandomWords(std::mt19937& generator, int word_count, int min_length, int max_length);

std::vector<std::string> GenerateSimilarWord(char ch, int length);

std::vector<std::string> PJWCrash(char ch, int length);

std::vector<std::string> CrashDJB2(int word_count, uint64_t crash_word);

#endif //THESIS_WORK_GENERATORS_H
