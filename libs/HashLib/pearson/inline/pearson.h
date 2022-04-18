#ifndef PEARSON_INLINE_H
#define PEARSON_INLINE_H

#include <cstddef>
#include <cstdint>

#if defined (__SSSE3__)
#include <immintrin.h>
#endif

namespace pearson_inline {

    // AES S-Box table -- allows for eventually supported hardware accelerated look-up
    static const uint8_t t[256] ={
            0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
            0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
            0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
            0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
            0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
            0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
            0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
            0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
            0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
            0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
            0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
            0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
            0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
            0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
            0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
            0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

#ifndef LOW_MEM_FOOTPRINT
    static uint16_t t16[65536];
#endif

    static inline uint64_t pearson_hash_64 (const uint8_t *in, size_t len, uint64_t hash_in) {

        size_t i;

        __m128i hash_mask = _mm_cvtsi64_si128(0x0706050403020100);

        __m128i hash = _mm_cvtsi64_si128 (hash_in);

        // tabel lookup preparation
#if defined (__AES__) // AES-NI -------------
        __m128i ZERO = _mm_setzero_si128();
        __m128i ISOLATE_SBOX_MASK = _mm_set_epi32(0x0306090C, 0x0F020508, 0x0B0E0104, 0x070A0D00);
#else // ------------------------------------
        __m128i const p16  = _mm_set1_epi8 (0x10);
	__m128i lut_result  = _mm_xor_si128 (lut_result, lut_result);
	__m128i selected_entries;
	__m128i table_line;
#endif // -----------------------------------

        for (i = 0; i < len; i++) {
            // broadcast the character, xor into hash, make them different permutations
            __m128i cc = _mm_set1_epi8 (in[i]);
            hash = _mm_xor_si128 (hash, cc);
            hash = _mm_xor_si128 (hash, hash_mask);

            // table lookup
#if defined (__AES__) // AES-NI -------------
            hash = _mm_shuffle_epi8(hash, ISOLATE_SBOX_MASK); // re-order along AES round
            hash = _mm_aesenclast_si128(hash, ZERO);
#else // ------------------------------------
            size_t j;
		__m128i lut_index = hash;
		lut_result = _mm_xor_si128 (lut_result, lut_result);
		for (j = 0; j < 256; j += 16) {
			table_line = _mm_load_si128 ((__m128i *)&t[j]);
			selected_entries = _mm_min_epu8 (lut_index, p16);
			selected_entries = _mm_cmpeq_epi8 (selected_entries, p16);
			selected_entries = _mm_or_si128 (selected_entries, lut_index);
			selected_entries = _mm_shuffle_epi8 (table_line, selected_entries);
			lut_result  = _mm_or_si128 (lut_result, selected_entries);
			lut_index = _mm_sub_epi8 (lut_index, p16);
		}
		hash = lut_result;
#endif // -----------------------------------
        }
        // store output
        return _mm_cvtsi128_si64 (hash);
    }

// 32-bit hash: the return value has to be interpreted as uint32_t and
// follows machine-specific endianess in memory
    static inline uint32_t pearson_hash_32 (const uint8_t *in, size_t len, uint32_t hash) {

        size_t i;
        uint32_t hash_mask = 0x03020100;

        for (i = 0; i < len; i++) {
            // broadcast the character, xor into hash, make them different permutations
            uint32_t c = (uint8_t)in[i];
            c |= c <<  8;
            c |= c << 16;
            hash ^= c ^ hash_mask;
            // table lookup
#ifdef LOW_MEM_FOOTPRINT // 256 byte look-up table ----------
            uint32_t h = 0;
		uint8_t x;
		x = hash; x = t[x]; hash >>= 8; h  = x; h = ROR32 (h, 8);
		x = hash; x = t[x]; hash >>= 8; h |= x; h = ROR32 (h, 8);
		x = hash; x = t[x]; hash >>= 8; h |= x; h = ROR32 (h, 8);
		x = hash; x = t[x];           ; h |= x; h = ROR32 (h, 8);
		hash = h;
#else // 16-bit look-up table -------------------------------
            hash = (t16[hash >> 16] << 16) + t16[(uint16_t)hash];
#endif // LOW_MEM_FOOTPRINT ---------------------------------
        }
        // output
        return hash;
    }

    static inline void pearson_hash_init () {

#ifndef LOW_MEM_FOOTPRINT // 16-bit look-up table -----------
        size_t i;

        for (i = 0; i < 65536; i++)
            t16[i] = (t[i >> 8] << 8) + t[(uint8_t)i];
#endif // LOW_MEM_FOOTPRINT ---------------------------------
    }
}

#endif //PEARSON_INLINE_H
