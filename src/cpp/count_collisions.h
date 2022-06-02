#ifndef THESIS_WORK_COUNT_COLLISIONS_H
#define THESIS_WORK_COUNT_COLLISIONS_H

#include <cstdint>
#include <map>

/*
 *  Вычисляет число коллизий
 *  Входной параметр:
 *      - hash_values - словарь, который содержит пары хеш-значение и счетчик коллизий для этого значения
 *  Выходной параметр: число коллизий
 */
uint64_t CountCollisions(const std::map<uint64_t, uint64_t>& hash_values);

#endif //THESIS_WORK_COUNT_COLLISIONS_H
