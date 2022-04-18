#ifndef FARM_HASH_INLINE_H
#define FARM_HASH_INLINE_H

// Copyright (c) 2014 Google, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// FarmHash, by Geoff Pike

//
// http://code.google.com/p/farmhash/
//
// This file provides a few functions for hashing strings and other
// data.  All of them are high-quality functions in the sense that
// they do well on standard tests such as Austin Appleby's SMHasher.
// They're also fast.  FarmHash is the successor to CityHash.
//
// Functions in the FarmHash family are not suitable for cryptography.
//
// WARNING: This code has been only lightly tested on big-endian platforms!
// It is known to work well on little-endian platforms that have a small penalty
// for unaligned reads, such as current Intel and AMD moderate-to-high-end CPUs.
// It should work on all 32-bit and 64-bit platforms that allow unaligned reads;
// bug reports are welcome.
//
// By the way, for some hash functions, given strings a and b, the hash
// of a+b is easily derived from the hashes of a and b.  This property
// doesn't hold for any hash functions in this file.

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>   // for memcpy and memset
#include <utility>

#include <byteswap.h>

#ifndef NAMESPACE_FOR_HASH_FUNCTIONS
#define NAMESPACE_FOR_HASH_FUNCTIONS util
#endif

#ifdef FARMHASH_BIG_ENDIAN
#define uint32_in_expected_order(x) (bswap_32(x))
#define uint64_in_expected_order(x) (bswap_64(x))
#else
#define uint32_in_expected_order(x) (x)
#define uint64_in_expected_order(x) (x)
#endif

// PLATFORM-SPECIFIC FUNCTIONS AND MACROS

#undef x86_64
#if defined (__x86_64) || defined (__x86_64__)
#define x86_64 1
#else
#define x86_64 0
#endif

#undef x86
#if defined(__i386__) || defined(__i386) || defined(__X86__)
#define x86 1
#else
#define x86 x86_64
#endif

#if !defined(is_64bit)
#define is_64bit (x86_64 || (sizeof(void*) == 8))
#endif

#undef can_use_ssse3
#if defined(__SSSE3__) || defined(FARMHASH_ASSUME_SSSE3)

#include <immintrin.h>
#define can_use_ssse3 1
// Now we can use _mm_hsub_epi16 and so on.

#else
#define can_use_ssse3 0
#endif

#undef can_use_sse41
#if defined(__SSE4_1__) || defined(FARMHASH_ASSUME_SSE41)

#include <immintrin.h>
#define can_use_sse41 1
// Now we can use _mm_insert_epi64 and so on.

#else
#define can_use_sse41 0
#endif

#undef can_use_sse42
#if defined(__SSE4_2__) || defined(FARMHASH_ASSUME_SSE42)

#include <nmmintrin.h>
#define can_use_sse42 1
// Now we can use _mm_crc32_u{32,16,8}.  And on 64-bit platforms, _mm_crc32_u64.

#else
#define can_use_sse42 0
#endif

namespace NAMESPACE_FOR_HASH_FUNCTIONS::s_inline {

#if defined(FARMHASH_UINT128_T_DEFINED)
    #if defined(__clang__)
#if !defined(uint128_t)
#define uint128_t __uint128_t
#endif
#endif
inline uint64_t Uint128Low64(const uint128_t x) {
  return static_cast<uint64_t>(x);
}
inline uint64_t Uint128High64(const uint128_t x) {
  return static_cast<uint64_t>(x >> 64);
}
inline uint128_t Uint128(uint64_t lo, uint64_t hi) {
  return lo + (((uint128_t)hi) << 64);
}
#else
    typedef std::pair<uint64_t, uint64_t> uint128_t;
    inline uint64_t Uint128Low64(const uint128_t x) { return x.first; }
    inline uint64_t Uint128High64(const uint128_t x) { return x.second; }
    inline uint128_t Uint128(uint64_t lo, uint64_t hi) { return uint128_t(lo, hi); }
#endif

#ifndef STATIC_INLINE
#define STATIC_INLINE static inline
#endif

#if !defined(FARMHASH_DEBUG) && (!defined(NDEBUG) || defined(_DEBUG))
#define FARMHASH_DEBUG 1
#endif

#undef debug_mode
#if FARMHASH_DEBUG
#define debug_mode 1
#else
#define debug_mode 0
#endif

    // Some primes between 2^63 and 2^64 for various uses.
    static const uint64_t k0 = 0xc3a5c85c97cb3127ULL;
    static const uint64_t k1 = 0xb492b66fbe98f273ULL;
    static const uint64_t k2 = 0x9ae16a3b2f90404fULL;

    // Magic numbers for 32-bit hashing.  Copied from Murmur3.
    static const uint32_t c1 = 0xcc9e2d51;
    static const uint32_t c2 = 0x1b873593;

#if can_use_ssse3 || can_use_sse41 || can_use_sse42 || can_use_aesni || can_use_avx
    STATIC_INLINE __m128i Fetch128(const char* s) {
        return _mm_loadu_si128(reinterpret_cast<const __m128i*>(s));
    }
#endif
// Building blocks for hash functions


    STATIC_INLINE uint32_t Bswap32(uint32_t val) { return bswap_32(val); }
    STATIC_INLINE uint64_t Bswap64(uint64_t val) { return bswap_64(val); }

    template <typename T> STATIC_INLINE T DebugTweak(T x) {
        if (debug_mode) {
            if (sizeof(x) == 4) {
                x = ~Bswap32(x * c1);
            } else {
                x = ~Bswap64(x * k1);
            }
        }
        return x;
    }

    STATIC_INLINE uint64_t Fetch64(const char *p) {
        uint64_t result;
        memcpy(&result, p, sizeof(result));
        return uint64_in_expected_order(result);
    }

    STATIC_INLINE uint32_t Fetch32(const char *p) {
        uint32_t result;
        memcpy(&result, p, sizeof(result));
        return uint32_in_expected_order(result);
    }

    // BASIC STRING HASHING

    STATIC_INLINE uint32_t BasicRotate32(uint32_t val, int shift) {
        // Avoid shifting by 32: doing so yields an undefined result.
        return shift == 0 ? val : ((val >> shift) | (val << (32 - shift)));
    }

    STATIC_INLINE uint64_t BasicRotate64(uint64_t val, int shift) {
        // Avoid shifting by 64: doing so yields an undefined result.
        return shift == 0 ? val : ((val >> shift) | (val << (64 - shift)));
    }

    STATIC_INLINE uint32_t Rotate32(uint32_t val, int shift) {
        return BasicRotate32(val, shift);
    }
    STATIC_INLINE uint64_t Rotate64(uint64_t val, int shift) {
        return BasicRotate64(val, shift);
    }

#undef Fetch
#define Fetch Fetch64

#undef Rotate
#define Rotate Rotate64

#undef Bswap
#define Bswap Bswap64

// Return a 16-byte hash for 48 bytes.  Quick and dirty.
// Callers do best to use "random-looking" values for a and b.
    STATIC_INLINE std::pair<uint64_t, uint64_t> WeakHashLen32WithSeeds(
            uint64_t w, uint64_t x, uint64_t y, uint64_t z, uint64_t a, uint64_t b) {
        a += w;
        b = Rotate(b + a + z, 21);
        uint64_t c = a;
        a += x;
        a += y;
        b += Rotate(a, 44);
        return std::make_pair(a + z, b + c);
    }

    // Return a 16-byte hash for s[0] ... s[31], a, and b.  Quick and dirty.
    STATIC_INLINE std::pair<uint64_t, uint64_t> WeakHashLen32WithSeeds(
            const char* s, uint64_t a, uint64_t b) {
        return WeakHashLen32WithSeeds(Fetch(s),
                                      Fetch(s + 8),
                                      Fetch(s + 16),
                                      Fetch(s + 24),
                                      a,
                                      b);
    }

    // Helpers for data-parallel operations (1x 128 bits or 2x 64 or 4x 32).
    STATIC_INLINE __m128i Add(__m128i x, __m128i y) { return _mm_add_epi64(x, y); }
    STATIC_INLINE __m128i Xor(__m128i x, __m128i y) { return _mm_xor_si128(x, y); }
    STATIC_INLINE __m128i Mul(__m128i x, __m128i y) { return _mm_mullo_epi32(x, y); }
    STATIC_INLINE __m128i Shuf(__m128i x, __m128i y) { return _mm_shuffle_epi8(y, x); }

    // Hash 128 input bits down to 64 bits of output.
    // This is intended to be a reasonably good hash function.
    // May change from time to time, may differ on different platforms, may differ
    // depending on NDEBUG.
    inline uint64_t Hash128to64(uint128_t x) {
        // Murmur-inspired hashing.
        const uint64_t kMul = 0x9ddfea08eb382d69ULL;
        uint64_t a = (Uint128Low64(x) ^ Uint128High64(x)) * kMul;
        a ^= (a >> 47);
        uint64_t b = (Uint128High64(x) ^ a) * kMul;
        b ^= (b >> 47);
        b *= kMul;
        return b;
    }

    STATIC_INLINE uint64_t ShiftMix(uint64_t val) {
        return val ^ (val >> 47);
    }

    STATIC_INLINE uint64_t HashLen16(uint64_t u, uint64_t v) {
        return Hash128to64(Uint128(u, v));
    }

    STATIC_INLINE uint64_t HashLen16(uint64_t u, uint64_t v, uint64_t mul) {
        // Murmur-inspired hashing.
        uint64_t a = (u ^ v) * mul;
        a ^= (a >> 47);
        uint64_t b = (v ^ a) * mul;
        b ^= (b >> 47);
        b *= mul;
        return b;
    }

    STATIC_INLINE uint64_t HashLen0to16(const char *s, size_t len) {
        if (len >= 8) {
            uint64_t mul = k2 + len * 2;
            uint64_t a = Fetch(s) + k2;
            uint64_t b = Fetch(s + len - 8);
            uint64_t c = Rotate(b, 37) * mul + a;
            uint64_t d = (Rotate(a, 25) + b) * mul;
            return HashLen16(c, d, mul);
        }
        if (len >= 4) {
            uint64_t mul = k2 + len * 2;
            uint64_t a = Fetch32(s);
            return HashLen16(len + (a << 3), Fetch32(s + len - 4), mul);
        }
        if (len > 0) {
            uint8_t a = s[0];
            uint8_t b = s[len >> 1];
            uint8_t c = s[len - 1];
            uint32_t y = static_cast<uint32_t>(a) + (static_cast<uint32_t>(b) << 8);
            uint32_t z = len + (static_cast<uint32_t>(c) << 2);
            return ShiftMix(y * k2 ^ z * k0) * k2;
        }
        return k2;
    }

    // This probably works well for 16-byte strings as well, but it may be overkill
    // in that case.
    STATIC_INLINE uint64_t HashLen17to32(const char *s, size_t len) {
        uint64_t mul = k2 + len * 2;
        uint64_t a = Fetch(s) * k1;
        uint64_t b = Fetch(s + 8);
        uint64_t c = Fetch(s + len - 8) * mul;
        uint64_t d = Fetch(s + len - 16) * k2;
        return HashLen16(Rotate(a + b, 43) + Rotate(c, 30) + d,
                         a + Rotate(b + k2, 18) + c, mul);
    }

    STATIC_INLINE uint64_t H32(const char *s, size_t len, uint64_t mul,
                               uint64_t seed0 = 0, uint64_t seed1 = 0) {
        uint64_t a = Fetch(s) * k1;
        uint64_t b = Fetch(s + 8);
        uint64_t c = Fetch(s + len - 8) * mul;
        uint64_t d = Fetch(s + len - 16) * k2;
        uint64_t u = Rotate(a + b, 43) + Rotate(c, 30) + d + seed0;
        uint64_t v = a + Rotate(b + k2, 18) + c + seed1;
        a = ShiftMix((u ^ v) * mul);
        b = ShiftMix((v ^ a) * mul);
        return b;
    }

    // Return an 8-byte hash for 33 to 64 bytes.
    STATIC_INLINE uint64_t HashLen33to64(const char *s, size_t len) {
        uint64_t mul0 = k2 - 30;
        uint64_t mul1 = k2 - 30 + 2 * len;
        uint64_t h0 = H32(s, 32, mul0);
        uint64_t h1 = H32(s + len - 32, 32, mul1);
        return ((h1 * mul1) + h0) * mul1;
    }

// Return an 8-byte hash for 65 to 96 bytes.
    STATIC_INLINE uint64_t HashLen65to96(const char *s, size_t len) {
        uint64_t mul0 = k2 - 114;
        uint64_t mul1 = k2 - 114 + 2 * len;
        uint64_t h0 = H32(s, 32, mul0);
        uint64_t h1 = H32(s + 32, 32, mul1);
        uint64_t h2 = H32(s + len - 32, 32, mul1, h0, h1);
        return (h2 * 9 + (h0 >> 17) + (h1 >> 21)) * mul1;
    }

    STATIC_INLINE uint64_t HashLen97to256(const char *s, size_t len) {
        const uint64_t seed = 81;
        if (len <= 32) {
            if (len <= 16) {
                return HashLen0to16(s, len);
            } else {
                return HashLen17to32(s, len);
            }
        } else if (len <= 64) {
            return HashLen33to64(s, len);
        }

        // For strings over 64 bytes we loop.  Internal state consists of
        // 56 bytes: v, w, x, y, and z.
        uint64_t x = seed;
        uint64_t y = seed * k1 + 113;
        uint64_t z = ShiftMix(y * k2 + 113) * k2;
        std::pair<uint64_t, uint64_t> v = std::make_pair(0, 0);
        std::pair<uint64_t, uint64_t> w = std::make_pair(0, 0);
        x = x * k2 + Fetch(s);

        // Set end so that after the loop we have 1 to 64 bytes left to process.
        const char* end = s + ((len - 1) / 64) * 64;
        const char* last64 = end + ((len - 1) & 63) - 63;
        assert(s + len - 64 == last64);
        do {
            x = Rotate(x + y + v.first + Fetch(s + 8), 37) * k1;
            y = Rotate(y + v.second + Fetch(s + 48), 42) * k1;
            x ^= w.second;
            y += v.first + Fetch(s + 40);
            z = Rotate(z + w.first, 33) * k1;
            v = WeakHashLen32WithSeeds(s, v.second * k1, x + w.first);
            w = WeakHashLen32WithSeeds(s + 32, z + w.second, y + Fetch(s + 16));
            std::swap(z, x);
            s += 64;
        } while (s != end);
        uint64_t mul = k1 + ((z & 0xff) << 1);
        // Make s point to the last 64 bytes of input.
        s = last64;
        w.first += ((len - 1) & 63);
        v.first += w.first;
        w.first += v.first;
        x = Rotate(x + y + v.first + Fetch(s + 8), 37) * mul;
        y = Rotate(y + v.second + Fetch(s + 48), 42) * mul;
        x ^= w.second * 9;
        y += v.first * 9 + Fetch(s + 40);
        z = Rotate(z + w.first, 33) * mul;
        v = WeakHashLen32WithSeeds(s, v.second * mul, x + w.first);
        w = WeakHashLen32WithSeeds(s + 32, z + w.second, y + Fetch(s + 16));
        std::swap(z, x);
        return HashLen16(HashLen16(v.first, w.first, mul) + ShiftMix(y) * k0 + z,
                         HashLen16(v.second, w.second, mul) + x,
                         mul);
    }

    STATIC_INLINE uint64_t H(uint64_t x, uint64_t y, uint64_t mul, int r) {
        uint64_t a = (x ^ y) * mul;
        a ^= (a >> 47);
        uint64_t b = (y ^ a) * mul;
        return Rotate(b, r) * mul;
    }

    STATIC_INLINE uint64_t Hash64WithSeeds(const char *s, size_t len,
                                           uint64_t seed0, uint64_t seed1) {
        if (len <= 64) {
            return HashLen16(HashLen97to256(s, len) - seed0, seed1);
        }

        // For strings over 64 bytes we loop.  Internal state consists of
        // 64 bytes: u, v, w, x, y, and z.
        uint64_t x = seed0;
        uint64_t y = seed1 * k2 + 113;
        uint64_t z = ShiftMix(y * k2) * k2;
        std::pair<uint64_t, uint64_t> v = std::make_pair(seed0, seed1);
        std::pair<uint64_t, uint64_t> w = std::make_pair(0, 0);
        uint64_t u = x - z;
        x *= k2;
        uint64_t mul = k2 + (u & 0x82);

        // Set end so that after the loop we have 1 to 64 bytes left to process.
        const char* end = s + ((len - 1) / 64) * 64;
        const char* last64 = end + ((len - 1) & 63) - 63;
        assert(s + len - 64 == last64);
        do {
            uint64_t a0 = Fetch(s);
            uint64_t a1 = Fetch(s + 8);
            uint64_t a2 = Fetch(s + 16);
            uint64_t a3 = Fetch(s + 24);
            uint64_t a4 = Fetch(s + 32);
            uint64_t a5 = Fetch(s + 40);
            uint64_t a6 = Fetch(s + 48);
            uint64_t a7 = Fetch(s + 56);
            x += a0 + a1;
            y += a2;
            z += a3;
            v.first += a4;
            v.second += a5 + a1;
            w.first += a6;
            w.second += a7;

            x = Rotate(x, 26);
            x *= 9;
            y = Rotate(y, 29);
            z *= mul;
            v.first = Rotate(v.first, 33);
            v.second = Rotate(v.second, 30);
            w.first ^= x;
            w.first *= 9;
            z = Rotate(z, 32);
            z += w.second;
            w.second += z;
            z *= 9;
            std::swap(u, y);

            z += a0 + a6;
            v.first += a2;
            v.second += a3;
            w.first += a4;
            w.second += a5 + a6;
            x += a1;
            y += a7;

            y += v.first;
            v.first += x - y;
            v.second += w.first;
            w.first += v.second;
            w.second += x - y;
            x += w.second;
            w.second = Rotate(w.second, 34);
            std::swap(u, z);
            s += 64;
        } while (s != end);
        // Make s point to the last 64 bytes of input.
        s = last64;
        u *= 9;
        v.second = Rotate(v.second, 28);
        v.first = Rotate(v.first, 20);
        w.first += ((len - 1) & 63);
        u += y;
        y += u;
        x = Rotate(y - x + v.first + Fetch(s + 8), 37) * mul;
        y = Rotate(y ^ v.second ^ Fetch(s + 48), 42) * mul;
        x ^= w.second * 9;
        y += v.first + Fetch(s + 40);
        z = Rotate(z + w.first, 33) * mul;
        v = WeakHashLen32WithSeeds(s, v.second * mul, x + w.first);
        w = WeakHashLen32WithSeeds(s + 32, z + w.second, y + Fetch(s + 16));
        return H(HashLen16(v.first + x, w.first ^ y, mul) + z - u,
                 H(v.second + y, w.second + z, k2, 30) ^ x,
                 k2,
                 31);
    }

    STATIC_INLINE uint64_t Hash64WithSeed(const char* s, size_t len, uint64_t seed) {
        return Hash64WithSeeds(s, len, k2, seed);
    }

    STATIC_INLINE uint64_t HashLen257to512(const char *s, size_t len) {
        return len <= 64 ? HashLen33to64(s, len) :
               Hash64WithSeeds(s, len, 81, 0);
    }

    STATIC_INLINE uint64_t Hash64Short(const char *s, size_t len) {
        if (len <= 32) {
            if (len <= 16) {
                return HashLen0to16(s, len);
            } else {
                return HashLen17to32(s, len);
            }
        } else if (len <= 64) {
            return HashLen33to64(s, len);
        } else if (len <= 96) {
            return HashLen65to96(s, len);
        } else if (len <= 256) {
            return HashLen97to256(s, len);
        } else {
            return HashLen257to512(s, len);
        }
    }

    // Requires n >= 256.  Requires SSE4.1. Should be slightly faster if the
    // compiler uses AVX instructions (e.g., use the -mavx flag with GCC).
    STATIC_INLINE uint64_t Hash64Long(const char* s, size_t n,
                                      uint64_t seed0, uint64_t seed1) {
        const __m128i kShuf =
                _mm_set_epi8(4, 11, 10, 5, 8, 15, 6, 9, 12, 2, 14, 13, 0, 7, 3, 1);
        const __m128i kMult =
                _mm_set_epi8(0xbd, 0xd6, 0x33, 0x39, 0x45, 0x54, 0xfa, 0x03,
                             0x34, 0x3e, 0x33, 0xed, 0xcc, 0x9e, 0x2d, 0x51);
        uint64_t seed2 = (seed0 + 113) * (seed1 + 9);
        uint64_t seed3 = (Rotate(seed0, 23) + 27) * (Rotate(seed1, 30) + 111);
        __m128i d0 = _mm_cvtsi64_si128(seed0);
        __m128i d1 = _mm_cvtsi64_si128(seed1);
        __m128i d2 = Shuf(kShuf, d0);
        __m128i d3 = Shuf(kShuf, d1);
        __m128i d4 = Xor(d0, d1);
        __m128i d5 = Xor(d1, d2);
        __m128i d6 = Xor(d2, d4);
        __m128i d7 = _mm_set1_epi32(seed2 >> 32);
        __m128i d8 = Mul(kMult, d2);
        __m128i d9 = _mm_set1_epi32(seed3 >> 32);
        __m128i d10 = _mm_set1_epi32(seed3);
        __m128i d11 = Add(d2, _mm_set1_epi32(seed2));
        const char* end = s + (n & ~static_cast<size_t>(255));
        do {
            __m128i z;
            z = Fetch128(s);
            d0 = Add(d0, z);
            d1 = Shuf(kShuf, d1);
            d2 = Xor(d2, d0);
            d4 = Xor(d4, z);
            d4 = Xor(d4, d1);
            std::swap(d0, d6);
            z = Fetch128(s + 16);
            d5 = Add(d5, z);
            d6 = Shuf(kShuf, d6);
            d8 = Shuf(kShuf, d8);
            d7 = Xor(d7, d5);
            d0 = Xor(d0, z);
            d0 = Xor(d0, d6);
            std::swap(d5, d11);
            z = Fetch128(s + 32);
            d1 = Add(d1, z);
            d2 = Shuf(kShuf, d2);
            d4 = Shuf(kShuf, d4);
            d5 = Xor(d5, z);
            d5 = Xor(d5, d2);
            std::swap(d10, d4);
            z = Fetch128(s + 48);
            d6 = Add(d6, z);
            d7 = Shuf(kShuf, d7);
            d0 = Shuf(kShuf, d0);
            d8 = Xor(d8, d6);
            d1 = Xor(d1, z);
            d1 = Add(d1, d7);
            z = Fetch128(s + 64);
            d2 = Add(d2, z);
            d5 = Shuf(kShuf, d5);
            d4 = Add(d4, d2);
            d6 = Xor(d6, z);
            d6 = Xor(d6, d11);
            std::swap(d8, d2);
            z = Fetch128(s + 80);
            d7 = Xor(d7, z);
            d8 = Shuf(kShuf, d8);
            d1 = Shuf(kShuf, d1);
            d0 = Add(d0, d7);
            d2 = Add(d2, z);
            d2 = Add(d2, d8);
            std::swap(d1, d7);
            z = Fetch128(s + 96);
            d4 = Shuf(kShuf, d4);
            d6 = Shuf(kShuf, d6);
            d8 = Mul(kMult, d8);
            d5 = Xor(d5, d11);
            d7 = Xor(d7, z);
            d7 = Add(d7, d4);
            std::swap(d6, d0);
            z = Fetch128(s + 112);
            d8 = Add(d8, z);
            d0 = Shuf(kShuf, d0);
            d2 = Shuf(kShuf, d2);
            d1 = Xor(d1, d8);
            d10 = Xor(d10, z);
            d10 = Xor(d10, d0);
            std::swap(d11, d5);
            z = Fetch128(s + 128);
            d4 = Add(d4, z);
            d5 = Shuf(kShuf, d5);
            d7 = Shuf(kShuf, d7);
            d6 = Add(d6, d4);
            d8 = Xor(d8, z);
            d8 = Xor(d8, d5);
            std::swap(d4, d10);
            z = Fetch128(s + 144);
            d0 = Add(d0, z);
            d1 = Shuf(kShuf, d1);
            d2 = Add(d2, d0);
            d4 = Xor(d4, z);
            d4 = Xor(d4, d1);
            z = Fetch128(s + 160);
            d5 = Add(d5, z);
            d6 = Shuf(kShuf, d6);
            d8 = Shuf(kShuf, d8);
            d7 = Xor(d7, d5);
            d0 = Xor(d0, z);
            d0 = Xor(d0, d6);
            std::swap(d2, d8);
            z = Fetch128(s + 176);
            d1 = Add(d1, z);
            d2 = Shuf(kShuf, d2);
            d4 = Shuf(kShuf, d4);
            d5 = Mul(kMult, d5);
            d5 = Xor(d5, z);
            d5 = Xor(d5, d2);
            std::swap(d7, d1);
            z = Fetch128(s + 192);
            d6 = Add(d6, z);
            d7 = Shuf(kShuf, d7);
            d0 = Shuf(kShuf, d0);
            d8 = Add(d8, d6);
            d1 = Xor(d1, z);
            d1 = Xor(d1, d7);
            std::swap(d0, d6);
            z = Fetch128(s + 208);
            d2 = Add(d2, z);
            d5 = Shuf(kShuf, d5);
            d4 = Xor(d4, d2);
            d6 = Xor(d6, z);
            d6 = Xor(d6, d9);
            std::swap(d5, d11);
            z = Fetch128(s + 224);
            d7 = Add(d7, z);
            d8 = Shuf(kShuf, d8);
            d1 = Shuf(kShuf, d1);
            d0 = Xor(d0, d7);
            d2 = Xor(d2, z);
            d2 = Xor(d2, d8);
            std::swap(d10, d4);
            z = Fetch128(s + 240);
            d3 = Add(d3, z);
            d4 = Shuf(kShuf, d4);
            d6 = Shuf(kShuf, d6);
            d7 = Mul(kMult, d7);
            d5 = Add(d5, d3);
            d7 = Xor(d7, z);
            d7 = Xor(d7, d4);
            std::swap(d3, d9);
            s += 256;
        } while (s != end);
        d6 = Add(Mul(kMult, d6), _mm_cvtsi64_si128(n));
        if (n % 256 != 0) {
            d7 = Add(_mm_shuffle_epi32(d8, (0 << 6) + (3 << 4) + (2 << 2) + (1 << 0)), d7);
            d8 = Add(Mul(kMult, d8), _mm_cvtsi64_si128(Hash64Short(s, n % 256)));
        }
        __m128i t[8];
        d0 = Mul(kMult, Shuf(kShuf, Mul(kMult, d0)));
        d3 = Mul(kMult, Shuf(kShuf, Mul(kMult, d3)));
        d9 = Mul(kMult, Shuf(kShuf, Mul(kMult, d9)));
        d1 = Mul(kMult, Shuf(kShuf, Mul(kMult, d1)));
        d0 = Add(d11, d0);
        d3 = Xor(d7, d3);
        d9 = Add(d8, d9);
        d1 = Add(d10, d1);
        d4 = Add(d3, d4);
        d5 = Add(d9, d5);
        d6 = Xor(d1, d6);
        d2 = Add(d0, d2);
        t[0] = d0;
        t[1] = d3;
        t[2] = d9;
        t[3] = d1;
        t[4] = d4;
        t[5] = d5;
        t[6] = d6;
        t[7] = d2;
        return Hash64Short(reinterpret_cast<const char*>(t), sizeof(t));
    }

    // Hash function for a byte array.
    // May change from time to time, may differ on different platforms, may differ
    // depending on NDEBUG.
    STATIC_INLINE uint64_t Hash64(const char* s, size_t len) {
        // Empirically, farmhashxo seems faster until length 512.
        return len >= 512 ? Hash64Long(s, len, k2, k1) : Hash64Short(s, len);
    }

    // Hash function for a byte array.  Most useful in 32-bit binaries.
    // May change from time to time, may differ on different platforms, may differ
    // depending on NDEBUG.
    STATIC_INLINE uint32_t Hash32(const char* s, size_t len) {
        return static_cast<uint32_t>(Hash64(s, len));
    }

    STATIC_INLINE uint32_t Hash32WithSeed(const char *s, size_t len, uint32_t seed) {
        return static_cast<uint32_t>(Hash64WithSeed(s, len, seed));
    }

}  // namespace NAMESPACE_FOR_HASH_FUNCTIONS

/* gently define FARMHASH_BIG_ENDIAN when detected big-endian machine */
#if defined(__BIG_ENDIAN__)
#if !defined(FARMHASH_BIG_ENDIAN)
    #define FARMHASH_BIG_ENDIAN
  #endif
#elif defined(__LITTLE_ENDIAN__)
// nothing for little-endian
#elif defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER == __ORDER_LITTLE_ENDIAN__)
// nothing for little-endian
#elif defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && (__BYTE_ORDER == __ORDER_BIG_ENDIAN__)
#if !defined(FARMHASH_BIG_ENDIAN)
    #define FARMHASH_BIG_ENDIAN
  #endif
#elif defined(__linux__) || defined(__CYGWIN__) || defined( __GNUC__ ) && !defined(_WIN32) || defined( __GNU_LIBRARY__ )
  #include <endian.h> // libc6-dev, GLIBC
  #if BYTE_ORDER == BIG_ENDIAN
    #if !defined(FARMHASH_BIG_ENDIAN)
      #define FARMHASH_BIG_ENDIAN
    #endif
  #endif
#elif defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__) || defined(__s390x__)
  #include <sys/endian.h>
  #if BYTE_ORDER == BIG_ENDIAN
    #if !defined(FARMHASH_BIG_ENDIAN)
      #define FARMHASH_BIG_ENDIAN
    #endif
  #endif
#elif defined(_WIN32)
  // Windows is (currently) little-endian
#else
  #error "Unable to determine endianness!"
#endif /* __BIG_ENDIAN__ */

#endif //FARM_HASH_INLINE_H
