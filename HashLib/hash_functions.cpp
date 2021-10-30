#include "hash_functions.h"



// i.e. FNV1a
// objsize: 0xd60-0xe2c: 204
uint32_t FNV32a(const void *key, int len, uint32_t seed)
{
    uint32_t h = seed;
    const auto  *data = (const uint8_t *)key;

    h ^= UINT32_C(2166136261);
    for (int i = 0; i < len; i++) {
        h ^= data[i];
        h *= 16777619;
    }
    return h;
}

uint64_t FNV64a(const char *key, int len, uint64_t seed)
{
    uint64_t  h = seed;
    auto  *data = (uint8_t *)key;
    const uint8_t *const end = &data[len];

    h ^= UINT64_C(0xcbf29ce484222325);
    while (data < end) {
        h ^= *data++;
        h *= UINT64_C(0x100000001b3);
    }
    return h;
}