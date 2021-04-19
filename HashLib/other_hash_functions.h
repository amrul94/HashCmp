// Хдесь собраны различные хеш-функции,
// исплиминтацию которых я не нашел в виде отдельныз проектов или файлов

#ifndef THESIS_WORK_OTHER_HASH_FUNCTIONS_H
#define THESIS_WORK_OTHER_HASH_FUNCTIONS_H

#include <string>

// https://www.programmingalgorithms.com/algorithm/sdbm-hash/cpp/
uint32_t SDBMHash32(std::string_view str);
uint64_t SDBMHash64(std::string_view str);

// ttp://www.cse.yorku.ca/~oz/hash.html
uint32_t DJB2Hash32(std::string_view str);
uint64_t DJB2Hash64(std::string_view str);

// https://www.programmingalgorithms.com/algorithm/pjw-hash/cpp/
uint32_t PJWHash32(std::string_view str);
uint64_t PJWHash64(std::string_view str);

// https://en.wikipedia.org/wiki/Jenkins_hash_function
uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length);
#endif //THESIS_WORK_OTHER_HASH_FUNCTIONS_H
