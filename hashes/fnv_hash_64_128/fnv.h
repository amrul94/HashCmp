// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Some helpers for quic.

#ifndef NET_QUIC_QUIC_UTILS_H_
#define NET_QUIC_QUIC_UTILS_H_

#include <stddef.h>
#include <stdint.h>

#include <string>

#include "int128.h"

#ifdef _MSC_VER
// MSVC 2013 and prior don't have alignof or aligned(); they have __alignof and
// a __declspec instead.
#define QUIC_ALIGN_OF __alignof
#define QUIC_ALIGNED(X) __declspec(align(X))
#else
#define QUIC_ALIGN_OF alignof
#define QUIC_ALIGNED(X) __attribute__((aligned(X)))
#endif  // _MSC_VER

namespace net {

class QuicUtils {
 public:
  enum Priority {
    LOCAL_PRIORITY,
    PEER_PRIORITY,
  };

  // Returns the 64 bit FNV1a hash of the data.  See
  // http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-param
  static uint64_t FNV1a_64_Hash(const char* data, int len);

  // returns the 128 bit FNV1a hash of the data.  See
  // http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-param
  static uint128 FNV1a_128_Hash(const char* data1, int len1);

  // returns the 128 bit FNV1a hash of the two sequences of data.  See
  // http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-param
  static uint128 FNV1a_128_Hash_Two(const char* data1,
                                    int len1,
                                    const char* data2,
                                    int len2);

  // SerializeUint128 writes the first 96 bits of |v| in little-endian form
  // to |out|.
  static void SerializeUint128Short(uint128 v, uint8_t* out);
};

}  // namespace net

#endif  // NET_QUIC_QUIC_UTILS_H_
