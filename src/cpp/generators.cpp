#include "generators.h"

#include <boost/assert.hpp>

// Конструирует num_generators хешей с шагом заданным шагом
std::vector<pcg64> GetGenerators(size_t num_generators, size_t num_generate_numbers) {
    BOOST_ASSERT_MSG(num_generators != 0, "num_generators must be greater than 0");

    // шаг генерации
    const uint64_t step = num_generate_numbers / num_generators;
    pcg64 rng; // ГПСЧ

    // Сдвигает шаг генерации на количество 64-битных хешей,
    // чтобы сиды (seeds) хеш-функций не совпадали со сгенерированными
    // числами, которые хэшируются
    num_generate_numbers += NUM_64_BITS_SEEDS;
    rng.advance(NUM_64_BITS_SEEDS);

    // Создается вектор, в который будут помещены генераторы
    std::vector<pcg64> generators;
    generators.reserve(num_generators);

    // В цикле помещаются генераторы в вектор с заданным шагом
    for (size_t i = 0; i < num_generators - 1; ++i) {
        generators.emplace_back(rng);
        rng.advance(step);
    }

    return generators;
}

// Генерирует случайный блок данных заданной длины (length)
std::string GenerateRandomDataBlock(pcg64& rng, uint32_t length) {
    // Создается строка, в которую будет помещен сгенерированный
    // блок данных
    std::string word;
    word.resize(length);

    // Размер генерируемого блока
    constexpr uint64_t chunk_size = sizeof(uint64_t);
    // Число генерируемых блоков
    const uint64_t num_chunks = length / chunk_size;

    // В цикле генерируются блоки и помещаются в строку
    for (uint64_t i = 0; i < num_chunks; ++i) {
        uint64_t src = rng();
        uint64_t step = i * chunk_size;
        std::memcpy(word.data() + step, &src, chunk_size);
    }

    // Генерация последнего блока данных
    uint64_t src = rng();
    const uint64_t all_chunks_size = num_chunks * chunk_size;
    const uint64_t diff = length - all_chunks_size;
    std::memcpy(word.data() + all_chunks_size, &src, diff);

    BOOST_ASSERT_MSG(word.size() == length, "words.size() != length");
    return word;
}

// Генерирует вектор из случайных блоков данных заданной длины (length)
std::vector<std::string> GenerateRandomDataBlocks(pcg64& rng, uint64_t num_blocks, uint32_t block_length) {
    // Создается вектор, в который будут помещены
    // сгенерированные блоки данных
    std::vector<std::string> generated_blocks;
    generated_blocks.reserve(num_blocks);

    // В цикле генерируются блоки данных и помещаются в вектор
    for (uint64_t i = 0; i < num_blocks; ++i) {
        std::string str = GenerateRandomDataBlock(rng, block_length);
        generated_blocks.push_back(std::move(str));
    }

    return generated_blocks;
}