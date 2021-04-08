// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <ctype.h>
#include <stdint.h>
#include <string.h>

#include <algorithm>

#include "fnv.h"

using std::string;

namespace net {
namespace {

// We know that >= GCC 4.8 and Clang have a __uint128_t intrinsic. Other
// compilers don't necessarily, notably MSVC.
#if defined(__x86_64__) &&                                         \
    ((defined(__GNUC__) &&                                         \
      (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8))) || \
     defined(__clang__))
#define QUIC_UTIL_HAS_UINT128 1
#endif

#ifdef QUIC_UTIL_HAS_UINT128
uint128 IncrementalHashFast(uint128 uhash, const char* data, size_t len) {
  // This code ends up faster than the naive implementation for 2 reasons:
  // 1. uint128 from base/int128.h is sufficiently complicated that the compiler
  //    cannot transform the multiplication by kPrime into a shift-multiply-add;
  //    it has go through all of the instructions for a 128-bit multiply.
  // 2. Because there are so fewer instructions (around 13), the hot loop fits
  //    nicely in the instruction queue of many Intel CPUs.
  // kPrime = 309485009821345068724781371
  static const __uint128_t kPrime =
      (static_cast<__uint128_t>(16777216) << 64) + 315;
  __uint128_t xhash = (static_cast<__uint128_t>(Uint128High64(uhash)) << 64) +
                      Uint128Low64(uhash);
  const uint8_t* octets = reinterpret_cast<const uint8_t*>(data);
  for (size_t i = 0; i < len; ++i) {
    xhash = (xhash ^ octets[i]) * kPrime;
  }
  return uint128(static_cast<uint64_t>(xhash >> 64),
                 static_cast<uint64_t>(xhash & UINT64_C(0xFFFFFFFFFFFFFFFF)));
}
#endif

#ifndef QUIC_UTIL_HAS_UINT128
// Slow implementation of IncrementalHash. In practice, only used by Chromium.
uint128 IncrementalHashSlow(uint128 hash, const char* data, size_t len) {
  // kPrime = 309485009821345068724781371
  static const uint128 kPrime(16777216, 315);
  const uint8_t* octets = reinterpret_cast<const uint8_t*>(data);
  for (size_t i = 0; i < len; ++i) {
    hash = hash ^ uint128(0, octets[i]);
    hash = hash * kPrime;
  }
  return hash;
}
#endif

uint128 IncrementalHash(uint128 hash, const char* data, size_t len) {
#ifdef QUIC_UTIL_HAS_UINT128
  return IncrementalHashFast(hash, data, len);
#else
  return IncrementalHashSlow(hash, data, len);
#endif
}

}  // namespace

// static
uint64_t QuicUtils::FNV1a_64_Hash(const char* data, int len) {
  static const uint64_t kOffset = UINT64_C(14695981039346656037);
  static const uint64_t kPrime = UINT64_C(1099511628211);

  const uint8_t* octets = reinterpret_cast<const uint8_t*>(data);

  uint64_t hash = kOffset;

  for (int i = 0; i < len; ++i) {
    hash = hash ^ octets[i];
    hash = hash * kPrime;
  }

  return hash;
}

// static
uint128 QuicUtils::FNV1a_128_Hash(const char* data, int len) {
  return FNV1a_128_Hash_Two(data, len, nullptr, 0);
}

// static
uint128 QuicUtils::FNV1a_128_Hash_Two(const char* data1,
                                      int len1,
                                      const char* data2,
                                      int len2) {
  // The two constants are defined as part of the hash algorithm.
  // see http://www.isthe.com/chongo/tech/comp/fnv/
  // kOffset = 144066263297769815596495629667062367629
  const uint128 kOffset(UINT64_C(7809847782465536322),
                        UINT64_C(7113472399480571277));

  uint128 hash = IncrementalHash(kOffset, data1, len1);
  if (data2 == nullptr) {
    return hash;
  }
  return IncrementalHash(hash, data2, len2);
}

// static
void QuicUtils::SerializeUint128Short(uint128 v, uint8_t* out) {
  const uint64_t lo = Uint128Low64(v);
  const uint64_t hi = Uint128High64(v);
  // This assumes that the system is little-endian.
  memcpy(out, &lo, sizeof(lo));
  memcpy(out + sizeof(lo), &hi, sizeof(hi) / 2);
}

}  // namespace net
