#ifndef THESIS_WORK_GENERATORS_H
#define THESIS_WORK_GENERATORS_H

#include <array>
#include <string>
#include <vector>

#include <pcg_random.hpp>

/*
 *  Генерирует сиды (seeds) для хеш-функций
 *  Параметры шаблона:
 *      1. UintT - целое беззнаковое число (тип сида)
 *      2. Generator - тип ГСПЧ
 *  Входной параметр: generator - ГСПЧ
 *  Выходное значение: массив сидов
*/
template<std::unsigned_integral UintT, size_t num_seeds, typename Generator>
static inline std::array<UintT, num_seeds> GenerateSeedsImpl(Generator& generator) {
    std::array<UintT, num_seeds> seeds{};
    for (UintT& seed : seeds) {
        seed = generator();
    }
    return seeds;
}

/*
 * Генерирует 8-битные сиды (seeds)
 *  Параметры шаблона: num_seeds - число сидов
 *  Выходное значение: массив сидов
 */
template<size_t num_seeds>
static inline std::array<uint8_t, num_seeds> Generate8BitsSeeds() {
    pcg8_oneseq_once_insecure rng;
    return GenerateSeedsImpl<uint8_t, num_seeds>(rng);
}

/*
 *  Генерирует 32-битные сиды (seeds)
 *  Параметры шаблона: num_seeds - число сидов
 *  Выходное значение: массив сидов
 */
template<size_t num_seeds>
static inline std::array<uint32_t, num_seeds> Generate32BitsSeeds() {
    pcg32 rng;
    return GenerateSeedsImpl<uint32_t, num_seeds>(rng);
}

/*
 *  Генерирует 64-битные сиды (seeds)
 *  Параметры шаблона: num_seeds - число сидов
 *  Выходное значение: массив сидов
 */
template<size_t num_seeds>
static inline std::array<uint64_t, num_seeds>  Generate64BitsSeeds() {
    pcg64 rng;
    return GenerateSeedsImpl<uint64_t, num_seeds>(rng);
}

// Число 64-битных сидов (seeds)
inline constexpr uint64_t NUM_64_BITS_SEEDS = 4;

// 8-битные сиды (seeds)
const auto [SEED_8_1, SEED_8_2, SEED_8_3, SEED_8_4,
            SEED_8_5, SEED_8_6, SEED_8_7, SEED_8_8,
            SEED_8_9, SEED_8_10, SEED_8_11, SEED_8_12,
            SEED_8_13, SEED_8_14, SEED_8_15, SEED_8_16] = Generate8BitsSeeds<16>();

// 32-битный сид (seed)
const auto [SEED_32] = Generate32BitsSeeds<1>();

// 64-битные сиды (seeds)
const auto [SEED_64_1, SEED_64_2, SEED_64_3, SEED_64_4] = Generate64BitsSeeds<NUM_64_BITS_SEEDS>();

/*
 *  Конструирует вектор генераторов с заданным шагом
 *  Входные параметры:
 *      1. num_generators - число генераторов
 *      2. num_generate_numbers - число генерируемых чисел
 *  Выходное значение: вектор генераторов
 */
std::vector<pcg64> GetGenerators(size_t num_generators, size_t num_generate_numbers);

/*  Генерирует случайный блок данных заданной длины
 *  Входные параметры:
 *      1. rng - генератор PCG-64
 *      2. length - размер блока данных
 *  Выходное значение: блок данных
 */
std::string GenerateRandomDataBlock(pcg64& rng, uint32_t length);

/*
 *  Генерирует вектор из случайных блоков данных заданной длины
 *  Входные параметры:
 *      1. rng - генератор PCG-64
 *      2. num_blocks - число блоков
 *      3. block_length - размер блока данных
 *  Выходное значение: массив блоков данных
 */
std::vector<std::string> GenerateRandomDataBlocks(pcg64& rng, uint64_t num_blocks, uint32_t block_length);

#endif //THESIS_WORK_GENERATORS_H
