#ifndef MURMUR_HASH_INLINE_H
#define MURMUR_HASH_INLINE_H


//-----------------------------------------------------------------------------
// Platform-specific functions and macros

// Microsoft Visual Studio

#if defined(_MSC_VER) && (_MSC_VER < 1600)

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned __int64 uint64_t;

// Other compilers

#else	// defined(_MSC_VER)

#include <cstdint>

#endif // !defined(_MSC_VER)
//-----------------------------------------------------------------------------

#define BIG_CONSTANT(x) (x##LLU)

namespace murmur_inline {

    static inline uint32_t MurmurHash1(const void * key, int len, uint32_t seed)
    {
        const unsigned int m = 0xc6a4a793;

        const int r = 16;

        unsigned int h = seed ^ (len * m);

        //----------

        const unsigned char * data = (const unsigned char *)key;

        while(len >= 4)
        {
            unsigned int k = *(unsigned int *)data;

            h += k;
            h *= m;
            h ^= h >> 16;

            data += 4;
            len -= 4;
        }

        //----------

        switch(len)
        {
            case 3:
                h += data[2] << 16;
            case 2:
                h += data[1] << 8;
            case 1:
                h += data[0];
                h *= m;
                h ^= h >> r;
        };

        //----------

        h *= m;
        h ^= h >> 10;
        h *= m;
        h ^= h >> 17;

        return h;
    }

    static inline uint32_t MurmurHash2(const void * key, int len, uint32_t seed)
    {
        // 'm' and 'r' are mixing constants generated offline.
        // They're not really 'magic', they just happen to work well.

        const uint32_t m = 0x5bd1e995;
        const int r = 24;

        // Initialize the hash to a 'random' value

        uint32_t h = seed ^ len;

        // Mix 4 bytes at a time into the hash

        const unsigned char * data = (const unsigned char *)key;

        while(len >= 4)
        {
            uint32_t k = *(uint32_t*)data;

            k *= m;
            k ^= k >> r;
            k *= m;

            h *= m;
            h ^= k;

            data += 4;
            len -= 4;
        }

        // Handle the last few bytes of the input array

        switch(len)
        {
            case 3: h ^= data[2] << 16;
            case 2: h ^= data[1] << 8;
            case 1: h ^= data[0];
                h *= m;
        };

        // Do a few final mixes of the hash to ensure the last few
        // bytes are well-incorporated.

        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h;
    }

    //-----------------------------------------------------------------------------
    // MurmurHash2, 64-bit versions, by Austin Appleby

    // The same caveats as 32-bit MurmurHash2 apply here - beware of alignment
    // and endian-ness issues if used across multiple platforms.

    // 64-bit hash for 64-bit platforms

    static inline uint64_t MurmurHash64A(const void * key, int len, uint64_t seed)
    {
        const uint64_t m = BIG_CONSTANT(0xc6a4a7935bd1e995);
        const int r = 47;

        uint64_t h = seed ^ (len * m);

        const uint64_t * data = (const uint64_t *)key;
        const uint64_t * end = data + (len/8);

        while(data != end)
        {
            uint64_t k = *data++;

            k *= m;
            k ^= k >> r;
            k *= m;

            h ^= k;
            h *= m;
        }

        const unsigned char * data2 = (const unsigned char*)data;

        switch(len & 7)
        {
            case 7: h ^= uint64_t(data2[6]) << 48;
            case 6: h ^= uint64_t(data2[5]) << 40;
            case 5: h ^= uint64_t(data2[4]) << 32;
            case 4: h ^= uint64_t(data2[3]) << 24;
            case 3: h ^= uint64_t(data2[2]) << 16;
            case 2: h ^= uint64_t(data2[1]) << 8;
            case 1: h ^= uint64_t(data2[0]);
                h *= m;
        };

        h ^= h >> r;
        h *= m;
        h ^= h >> r;

        return h;
    }


    // This is a variant of MurmurHash2 modified to use the Merkle-Damgard
    // construction. Bulk speed should be identical to Murmur2, small-key speed
    // will be 10%-20% slower due to the added overhead at the end of the hash.

    // This variant fixes a minor issue where null keys were more likely to
    // collide with each other than expected, and also makes the function
    // more amenable to incremental implementations.

#define mmix(h,k) { k *= m; k ^= k >> r; k *= m; h *= m; h ^= k; }

    static inline uint32_t MurmurHash2A(const void * key, int len, uint32_t seed)
    {
        const uint32_t m = 0x5bd1e995;
        const int r = 24;
        uint32_t l = len;

        const unsigned char * data = (const unsigned char *)key;

        uint32_t h = seed;

        while(len >= 4)
        {
            uint32_t k = *(uint32_t*)data;

            mmix(h,k);

            data += 4;
            len -= 4;
        }

        uint32_t t = 0;

        switch(len)
        {
            case 3: t ^= data[2] << 16;
            case 2: t ^= data[1] << 8;
            case 1: t ^= data[0];
        };

        mmix(h,t);
        mmix(h,l);

        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h;
    }

#undef mmix

    inline uint32_t rotl32 ( uint32_t x, int8_t r )
    {
        return (x << r) | (x >> (32 - r));
    }

    inline uint64_t rotl64 ( uint64_t x, int8_t r )
    {
        return (x << r) | (x >> (64 - r));
    }

#define	ROTL32(x,y)	rotl32(x,y)
#define ROTL64(x,y)	rotl64(x,y)

    static inline uint32_t getblock32(const uint32_t * p, int i)
    {
        return p[i];
    }

    static inline uint64_t getblock64(const uint64_t * p, int i)
    {
        return p[i];
    }

    //-----------------------------------------------------------------------------
    // Finalization mix - force all bits of a hash block to avalanche

    static inline uint32_t fmix32 ( uint32_t h )
    {
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;

        return h;
    }

    static inline uint32_t MurmurHash3_x86_32 (const void * key, int len, uint32_t seed)
    {
        const uint8_t * data = (const uint8_t*)key;
        const int nblocks = len / 4;

        uint32_t h1 = seed;

        const uint32_t c1 = 0xcc9e2d51;
        const uint32_t c2 = 0x1b873593;

        //----------
        // body

        const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

        for(int i = -nblocks; i; i++)
        {
            uint32_t k1 = getblock32(blocks,i);

            k1 *= c1;
            k1 = ROTL32(k1,15);
            k1 *= c2;

            h1 ^= k1;
            h1 = ROTL32(h1,13);
            h1 = h1*5+0xe6546b64;
        }

        //----------
        // tail

        const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

        uint32_t k1 = 0;

        switch(len & 3)
        {
            case 3: k1 ^= tail[2] << 16;
            case 2: k1 ^= tail[1] << 8;
            case 1: k1 ^= tail[0];
                k1 *= c1; k1 = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
        };

        //----------
        // finalization

        h1 ^= len;

        h1 = fmix32(h1);

        return h1;
    }

#undef ROTL64
#undef ROTL32

}

//-----------------------------------------------------------------------------

#undef BIG_CONSTANT

#endif //MURMUR_HASH_INLINE_H
