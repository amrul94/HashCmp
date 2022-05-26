#pragma once

#include <cassert>  /* for assert() */
#include <cstring>  /* for memcpy() */
#include <climits>
#include <cstddef>
#include <cstdint>

#define PAGESIZE 4096

#ifndef likely
#define likely(cond) __builtin_expect(!!(cond), 1)
#endif

#ifndef unlikely
#define unlikely(cond) __builtin_expect(!!(cond), 0)
#endif

#define prefetch(ptr)                                                          \
  do {                                                                         \
    (void)(ptr);                                                               \
  } while (0)

#define T1HA_ALIGN_PREFIX
#define T1HA_ALIGN_SUFFIX                                                      \
  __attribute__((__aligned__(32))) /* required only for SIMD */

typedef union T1HA_ALIGN_PREFIX t1ha_state256 {
    uint8_t bytes[32];
    uint32_t u32[8];
    uint64_t u64[4];
    struct {
        uint64_t a, b, c, d;
    } n;
} t1ha_state256_t T1HA_ALIGN_SUFFIX;

#if !defined(__maybe_unused) &&                                                \
    (__GNUC_PREREQ(4, 3) || __has_attribute(__unused__))
#define __maybe_unused __attribute__((__unused__))
#endif

static __maybe_unused __always_inline uint64_t rot64(uint64_t v, unsigned s) {
return (v >> s) | (v << (64 - s));
}

/* xor-mul-xor mixer */
static __inline uint64_t mix64(uint64_t v, uint64_t p) {
    v *= p;
    return v ^ rot64(v, 41);
}

/* 'magic' primes */
static const uint64_t prime_0 = UINT64_C(0xEC99BF0D8372CAAB);
static const uint64_t prime_1 = UINT64_C(0x82434FE90EDCEF39);
static const uint64_t prime_2 = UINT64_C(0xD4F06DB99D67BE4B);
static const uint64_t prime_3 = UINT64_C(0xBD9CACC22C6E9571);
static const uint64_t prime_4 = UINT64_C(0x9C06FAF4D023E3AB);
static const uint64_t prime_5 = UINT64_C(0xC060724A8424F345);
static const uint64_t prime_6 = UINT64_C(0xCB5AF53AE3AAAC31);

static __maybe_unused __always_inline uint64_t mul_64x64_128(uint64_t a,
                                                             uint64_t b,
                                                             uint64_t *h) {
    __uint128_t r = (__uint128_t) a * (__uint128_t) b;
    /* modern GCC could nicely optimize this */
    *h = (uint64_t) (r >> 64);
    return (uint64_t) r;
}

/* xor high and low parts of full 128-bit product */
static __maybe_unused __always_inline uint64_t mux64(uint64_t v,
                                                     uint64_t prime) {
    uint64_t l, h;
    l = mul_64x64_128(v, prime, &h);
    return l ^ h;
}

static __inline uint64_t final_weak_avalanche(uint64_t a, uint64_t b) {
    /* LY: for performance reason on a some not high-end CPUs
     * I replaced the second mux64() operation by mix64().
     * Unfortunately this approach fails the "strict avalanche criteria",
     * see test results at https://github.com/demerphq/smhasher. */
    return mux64(rot64(a + b, 17), prime_4) + mix64(a ^ b, prime_0);
}

#define read_unaligned(ptr, bits) (*(const uint##bits##_t *__restrict)(ptr))

static __maybe_unused __always_inline uint64_t fetch64_le_unaligned(const void *v) {
    return read_unaligned(v, 64);
}

#define can_read_underside(ptr, size)                                          \
  (((PAGESIZE - (size)) & (uintptr_t)(ptr)) != 0)

static __maybe_unused __always_inline uint64_t
tail64_le_unaligned(const void *v, size_t tail) {
    const uint8_t *p = (const uint8_t *) v;
    /* On some systems (e.g. x86_64) we can perform a 'oneshot' read, which
   * is little bit faster. Thanks Marcin Żukowski <marcin.zukowski@gmail.com>
   * for the reminder. */
    const unsigned offset = (8 - tail) & 7;
    const unsigned shift = offset << 3;
    if (likely(can_read_underside(p, 8))) {
        p -= offset;
        return fetch64_le_unaligned(p) >> shift;
    }
    return fetch64_le_unaligned(p) & ((~UINT64_C(0)) >> shift);
}

#define T1HA1_BODY(ENDIANNES, ALIGNESS)                                        \
  const uint64_t *v = (const uint64_t *)data;                                  \
  if (unlikely(len > 32)) {                                                    \
    uint64_t c = rot64(len, 17) + seed;                                        \
    uint64_t d = len ^ rot64(seed, 17);                                        \
    const uint64_t *detent =                                                   \
        (const uint64_t *)((const uint8_t *)data + len - 31);                  \
    do {                                                                       \
      const uint64_t w0 = fetch64_##ENDIANNES##_##ALIGNESS(v + 0);             \
      const uint64_t w1 = fetch64_##ENDIANNES##_##ALIGNESS(v + 1);             \
      const uint64_t w2 = fetch64_##ENDIANNES##_##ALIGNESS(v + 2);             \
      const uint64_t w3 = fetch64_##ENDIANNES##_##ALIGNESS(v + 3);             \
      v += 4;                                                                  \
      prefetch(v);                                                             \
                                                                               \
      const uint64_t d02 = w0 ^ rot64(w2 + d, 17);                             \
      const uint64_t c13 = w1 ^ rot64(w3 + c, 17);                             \
      d -= b ^ rot64(w1, 31);                                                  \
      c += a ^ rot64(w0, 41);                                                  \
      b ^= prime_0 * (c13 + w2);                                               \
      a ^= prime_1 * (d02 + w3);                                               \
    } while (likely(v < detent));                                              \
                                                                               \
    a ^= prime_6 * (rot64(c, 17) + d);                                         \
    b ^= prime_5 * (c + rot64(d, 17));                                         \
    len &= 31;                                                                 \
  }                                                                            \
                                                                               \
  switch (len) {                                                               \
  default:                                                                     \
    b += mux64(fetch64_##ENDIANNES##_##ALIGNESS(v++), prime_4);                \
  /* fall through */                                                           \
  case 24:                                                                     \
  case 23:                                                                     \
  case 22:                                                                     \
  case 21:                                                                     \
  case 20:                                                                     \
  case 19:                                                                     \
  case 18:                                                                     \
  case 17:                                                                     \
    a += mux64(fetch64_##ENDIANNES##_##ALIGNESS(v++), prime_3);                \
  /* fall through */                                                           \
  case 16:                                                                     \
  case 15:                                                                     \
  case 14:                                                                     \
  case 13:                                                                     \
  case 12:                                                                     \
  case 11:                                                                     \
  case 10:                                                                     \
  case 9:                                                                      \
    b += mux64(fetch64_##ENDIANNES##_##ALIGNESS(v++), prime_2);                \
  /* fall through */                                                           \
  case 8:                                                                      \
  case 7:                                                                      \
  case 6:                                                                      \
  case 5:                                                                      \
  case 4:                                                                      \
  case 3:                                                                      \
  case 2:                                                                      \
  case 1:                                                                      \
    a += mux64(tail64_##ENDIANNES##_##ALIGNESS(v, len), prime_1);              \
  /* fall through */                                                           \
  case 0:                                                                      \
    return final_weak_avalanche(a, b);                                         \
  }

/* The little-endian variant. */
static inline uint64_t t1ha1_inline(const void *data, size_t len, uint64_t seed) {
    uint64_t a = seed;
    uint64_t b = len;

    T1HA1_BODY(le, unaligned);
}

static __maybe_unused __always_inline uint64_t final64(uint64_t a, uint64_t b) {
    uint64_t x = (a + rot64(b, 41)) * prime_0;
    uint64_t y = (rot64(a, 23) + b) * prime_6;
    return mux64(x ^ y, prime_5);
}

static __maybe_unused __always_inline void mixup64(uint64_t *__restrict a,
                                                   uint64_t *__restrict b,
                                                   uint64_t v, uint64_t prime) {
    uint64_t h;
    *a ^= mul_64x64_128(*b + v, prime, &h);
    *b += h;
}

static __always_inline void init_ab(t1ha_state256_t *s, uint64_t x,
                                    uint64_t y) {
    s->n.a = x;
    s->n.b = y;
}

static __always_inline void init_cd(t1ha_state256_t *s, uint64_t x,
                                    uint64_t y) {
    s->n.c = rot64(y, 23) + ~x;
    s->n.d = ~y + rot64(x, 19);
}

#define T1HA2_UPDATE(ENDIANNES, ALIGNESS, state, v)                            \
  do {                                                                         \
    t1ha_state256_t *const s = state;                                          \
    const uint64_t w0 = fetch64_##ENDIANNES##_##ALIGNESS(v + 0);               \
    const uint64_t w1 = fetch64_##ENDIANNES##_##ALIGNESS(v + 1);               \
    const uint64_t w2 = fetch64_##ENDIANNES##_##ALIGNESS(v + 2);               \
    const uint64_t w3 = fetch64_##ENDIANNES##_##ALIGNESS(v + 3);               \
                                                                               \
    const uint64_t d02 = w0 + rot64(w2 + s->n.d, 56);                          \
    const uint64_t c13 = w1 + rot64(w3 + s->n.c, 19);                          \
    s->n.d ^= s->n.b + rot64(w1, 38);                                          \
    s->n.c ^= s->n.a + rot64(w0, 57);                                          \
    s->n.b ^= prime_6 * (c13 + w2);                                            \
    s->n.a ^= prime_5 * (d02 + w3);                                            \
  } while (0)

static __always_inline void squash(t1ha_state256_t *s) {
    s->n.a ^= prime_6 * (s->n.c + rot64(s->n.d, 23));
    s->n.b ^= prime_5 * (rot64(s->n.c, 19) + s->n.d);
}

#define T1HA2_LOOP(ENDIANNES, ALIGNESS, state, data, len)                      \
  do {                                                                         \
    const void *detent = (const uint8_t *)data + len - 31;                     \
    do {                                                                       \
      const uint64_t *v = (const uint64_t *)data;                              \
      data = (const uint64_t *)data + 4;                                       \
      prefetch(data);                                                          \
      T1HA2_UPDATE(le, ALIGNESS, state, v);                                    \
    } while (likely(data < detent));                                           \
  } while (0)

#define T1HA2_TAIL_AB(ENDIANNES, ALIGNESS, state, data, len)                   \
  do {                                                                         \
    t1ha_state256_t *const s = state;                                          \
    const uint64_t *v = (const uint64_t *)data;                                \
    switch (len) {                                                             \
    default:                                                                   \
      mixup64(&s->n.a, &s->n.b, fetch64_##ENDIANNES##_##ALIGNESS(v++),         \
              prime_4);                                                        \
    /* fall through */                                                         \
    case 24:                                                                   \
    case 23:                                                                   \
    case 22:                                                                   \
    case 21:                                                                   \
    case 20:                                                                   \
    case 19:                                                                   \
    case 18:                                                                   \
    case 17:                                                                   \
      mixup64(&s->n.b, &s->n.a, fetch64_##ENDIANNES##_##ALIGNESS(v++),         \
              prime_3);                                                        \
    /* fall through */                                                         \
    case 16:                                                                   \
    case 15:                                                                   \
    case 14:                                                                   \
    case 13:                                                                   \
    case 12:                                                                   \
    case 11:                                                                   \
    case 10:                                                                   \
    case 9:                                                                    \
      mixup64(&s->n.a, &s->n.b, fetch64_##ENDIANNES##_##ALIGNESS(v++),         \
              prime_2);                                                        \
    /* fall through */                                                         \
    case 8:                                                                    \
    case 7:                                                                    \
    case 6:                                                                    \
    case 5:                                                                    \
    case 4:                                                                    \
    case 3:                                                                    \
    case 2:                                                                    \
    case 1:                                                                    \
      mixup64(&s->n.b, &s->n.a, tail64_##ENDIANNES##_##ALIGNESS(v, len),       \
              prime_1);                                                        \
    /* fall through */                                                         \
    case 0:                                                                    \
      return final64(s->n.a, s->n.b);                                          \
    }                                                                          \
  } while (0)

static inline uint64_t t1ha2_atonce_inline(const void *data, size_t length, uint64_t seed) {
    t1ha_state256_t state;
    init_ab(&state, seed, length);

    if (unlikely(length > 32)) {
        init_cd(&state, seed, length);
        T1HA2_LOOP(le, unaligned, &state, data, length);
        squash(&state);
        length &= 31;
    }
    T1HA2_TAIL_AB(le, unaligned, &state, data, length);
}