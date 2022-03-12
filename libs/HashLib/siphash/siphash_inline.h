#ifndef SIPHASH_H
#define SIPHASH_H

#include <stdint.h>
#include <stdlib.h>

#include "siphash_impl.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef U8TO64
#define U8TO64
static uint64_t INLINE
U8TO64_LE(const unsigned char *p) {
    return *(const uint64_t *)p;
}
#endif

#define SIPCOMPRESS \
	v0 += v1; v2 += v3; \
	v1 = ROTL64(v1,13); v3 = ROTL64(v3,16); \
	v1 ^= v0; v3 ^= v2; \
	v0 = ROTL64(v0,32); \
	v2 += v1; v0 += v3; \
	v1 = ROTL64(v1,17); v3 = ROTL64(v3,21); \
	v1 ^= v2; v3 ^= v0; \
	v2 = ROTL64(v2,32)

static inline uint64_t siphash(const unsigned char key[16], const unsigned char *m, size_t len) {
    uint64_t v0, v1, v2, v3;
    uint64_t mi, k0, k1;
    uint64_t last7;
    size_t i, blocks;

    k0 = U8TO64_LE(key + 0);
    k1 = U8TO64_LE(key + 8);
    v0 = k0 ^ 0x736f6d6570736575ull;
    v1 = k1 ^ 0x646f72616e646f6dull;
    v2 = k0 ^ 0x6c7967656e657261ull;
    v3 = k1 ^ 0x7465646279746573ull;

    last7 = (uint64_t)(len & 0xff) << 56;

    for (i = 0, blocks = (len & ~7); i < blocks; i += 8) {
        mi = U8TO64_LE(m + i);
        v3 ^= mi;
        SIPCOMPRESS; /* 2 c rounds */
        SIPCOMPRESS;
        v0 ^= mi;
    }

    switch (len - blocks) {
        case 7: last7 |= (uint64_t)m[i + 6] << 48;
        case 6: last7 |= (uint64_t)m[i + 5] << 40;
        case 5: last7 |= (uint64_t)m[i + 4] << 32;
        case 4: last7 |= (uint64_t)m[i + 3] << 24;
        case 3: last7 |= (uint64_t)m[i + 2] << 16;
        case 2: last7 |= (uint64_t)m[i + 1] <<  8;
        case 1: last7 |= (uint64_t)m[i + 0]      ;
        case 0:
        default:;
    };
    v3 ^= last7;
    SIPCOMPRESS; /* 2 more c rounds */
    SIPCOMPRESS;
    v0 ^= last7;
    v2 ^= 0xff;
    SIPCOMPRESS; /* and 4 final d rounds */
    SIPCOMPRESS;
    SIPCOMPRESS;
    SIPCOMPRESS;
    return v0 ^ v1 ^ v2 ^ v3;
}

static inline uint64_t siphash13(const unsigned char key[16], const unsigned char *m, size_t len) {
    uint64_t v0, v1, v2, v3;
    uint64_t mi, k0, k1;
    uint64_t last7;
    size_t i, blocks;

    k0 = U8TO64_LE(key + 0);
    k1 = U8TO64_LE(key + 8);
    v0 = k0 ^ 0x736f6d6570736575ull;
    v1 = k1 ^ 0x646f72616e646f6dull;
    v2 = k0 ^ 0x6c7967656e657261ull;
    v3 = k1 ^ 0x7465646279746573ull;

    last7 = (uint64_t)(len & 0xff) << 56;

    for (i = 0, blocks = (len & ~7); i < blocks; i += 8) {
        mi = U8TO64_LE(m + i);
        v3 ^= mi;
        SIPCOMPRESS; /* 1 c round */
        v0 ^= mi;
    }

    switch (len - blocks) {
        case 7: last7 |= (uint64_t)m[i + 6] << 48;
        case 6: last7 |= (uint64_t)m[i + 5] << 40;
        case 5: last7 |= (uint64_t)m[i + 4] << 32;
        case 4: last7 |= (uint64_t)m[i + 3] << 24;
        case 3: last7 |= (uint64_t)m[i + 2] << 16;
        case 2: last7 |= (uint64_t)m[i + 1] <<  8;
        case 1: last7 |= (uint64_t)m[i + 0]      ;
        case 0:
        default:;
    };
    v3 ^= last7;
    SIPCOMPRESS; /* 1 more c round */
    v0 ^= last7;
    v2 ^= 0xff;
    SIPCOMPRESS; /* and 3 final d rounds */
    SIPCOMPRESS;
    SIPCOMPRESS;
    return v0 ^ v1 ^ v2 ^ v3;
}

#define ROTL(x, b) (uint32_t)(((x) << (b)) | ((x) >> (32 - (b))))
#define U8TO32_LE(p)                                                    \
    (((uint32_t)((p)[0])) | ((uint32_t)((p)[1]) << 8) |                 \
     ((uint32_t)((p)[2]) << 16) | ((uint32_t)((p)[3]) << 24))
#define SIPROUND                                       \
    do {                                               \
        v0 += v1;                                      \
        v1 = ROTL(v1, 5);                              \
        v1 ^= v0;                                      \
        v0 = ROTL(v0, 16);                             \
        v2 += v3;                                      \
        v3 = ROTL(v3, 8);                              \
        v3 ^= v2;                                      \
        v0 += v3;                                      \
        v3 = ROTL(v3, 7);                              \
        v3 ^= v0;                                      \
        v2 += v1;                                      \
        v1 = ROTL(v1, 13);                             \
        v1 ^= v2;                                      \
        v2 = ROTL(v2, 16);                             \
    } while (0)


static inline uint32_t halfsiphash(const unsigned char key[16], const unsigned char *m, size_t len) {
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t v2 = 0x6c796765;
    uint32_t v3 = 0x74656462;
    uint32_t k0 = U8TO32_LE(key);
    uint32_t k1 = U8TO32_LE(key + 8);
    uint32_t mi;
    const uint8_t *end = m + len - (len % sizeof(uint32_t));
    const int left = len & 3;
    uint32_t b = ((uint32_t)len) << 24;
    v3 ^= k1;
    v2 ^= k0;
    v1 ^= k1;
    v0 ^= k0;

    for (; m != end; m += 4) {
        mi = U8TO32_LE(m);
        v3 ^= mi;
        SIPROUND;
        SIPROUND;
        v0 ^= mi;
    }

    switch (left) {
        case 3:
            b |= ((uint32_t)m[2]) << 16;
        case 2:
            b |= ((uint32_t)m[1]) << 8;
        case 1:
            b |= ((uint32_t)m[0]);
            break;
        case 0:
            break;
    }

    v3 ^= b;
    SIPROUND;
    SIPROUND;
    v0 ^= b;
    v2 ^= 0xff;
    SIPROUND;
    SIPROUND;
    SIPROUND;
    SIPROUND;
    return v1 ^ v3;
}
 
#ifdef __cplusplus
}
#endif

#endif // SIPHASH_H
