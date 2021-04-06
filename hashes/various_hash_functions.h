// Хдесь собраны различные хеш-функции,
// исплиминтацию которых я не нашел в виде отдельныз проектов или файлов

#ifndef THESIS_WORK_VARIOUS_HASH_FUNCTIONS_H
#define THESIS_WORK_VARIOUS_HASH_FUNCTIONS_H

#include <string>


// https://www.programmingalgorithms.com/algorithm/sdbm-hash/cpp/
uint32_t SDBMHash32(const std::string& str);
uint64_t SDBMHash64(const std::string& str);

#endif //THESIS_WORK_VARIOUS_HASH_FUNCTIONS_H
