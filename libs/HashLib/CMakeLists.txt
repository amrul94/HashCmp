cmake_minimum_required(VERSION 3.17)

project(HashLib)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_FLAGS "-march=native -O3 -pipe -Werror -pthread -maes")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")


set(CITY_HASH cityhash/citycrc.h cityhash/config.h cityhash/city.h cityhash/city.cc)
set(FARM_HASH farmhash/farmhash.h farmhash/farmhash.cc)
set(FAST_HASH fasthash/fasthash.h fasthash/fasthash.c)
file(GLOB HIGHWAYHASH "*.h" "*.cpp")
set(JENKINS_HASH spooky_hash/spooky.h spooky_hash/spooky.c)
set(METRO_HASH metro_hash/metrohash64.h metro_hash/metrohash64.cpp metro_hash/platform.h)
set(MURMUR_HASH murmur_hash/MurmurHash1.h murmur_hash/MurmurHash1.cpp
        murmur_hash/MurmurHash2.h murmur_hash/MurmurHash2.cpp
        murmur_hash/MurmurHash3.h murmur_hash/MurmurHash3.cpp)
set(MX3 mx3/mx3.h)
set(NMHASH nmhash/nmhash.h)
set(PEARSON_HASH pearson/pearson.h pearson/pearson.cpp)
set(PENGY_HASH pengyhash/pengyhash.h)
set(ROLLING_HASH rolling_hash/cyclichash.h rolling_hash/rabinkarphash.h rolling_hash/characterhash.h
        rolling_hash/mersennetwister.h rolling_hash/generalhash.h rolling_hash/threewisehash.h
        rolling_hash/adler32.h)
set(SIPHASH siphash/siphash_impl.h siphash/siphash.h siphash/siphash.c siphash/siphash_sse2.c siphash/siphash_ssse3.c
        siphash/siphash_ssse3.c siphash siphash/halfsiphash.c)
set(SUPER_FAST_HASH super_fast_hash/super_fast_hash.h super_fast_hash/super_fast_hash.cpp)
set(T1HA_HASH t1ha/t1ha.h t1ha/t1ha0.c t1ha/t1ha0_ia32aes_a.h t1ha/t1ha0_ia32aes_avx.c
        t1ha/t1ha0_ia32aes_avx2.c t1ha/t1ha0_ia32aes_b.h t1ha/t1ha0_ia32aes_noavx.c
        t1ha/t1ha0_selfcheck.c t1ha/t1ha1.c t1ha/t1ha1_selfcheck.c
        t1ha/t1ha2.c t1ha/t1ha2_selfcheck.c t1ha/t1ha_bits.h t1ha/t1ha_selfcheck.c
        t1ha/t1ha_selfcheck.h t1ha/t1ha_selfcheck_all.c)
set(WY_HASH wyhash/wyhash.h wyhash/wyhash32.h)
set(XX_HASH xx_hash/xxhash.h xx_hash/xxhash.c xx_hash/xxh3.h)
set(OTHERS_HASH hash_functions.h hash_functions.cpp)

add_library(HashLib STATIC ${CITY_HASH} ${FARM_HASH} ${FAST_HASH} ${HIGHWAYHASH} ${JENKINS_HASH} ${METRO_HASH} ${MURMUR_HASH}
        ${MX3} ${NMHASH} ${PEARSON_HASH} ${PENGY_HASH} ${ROLLING_HASH} ${SIPHASH} ${SUPER_FAST_HASH} ${T1HA_HASH}
        ${XX_HASH} ${WY_HASH}
        ${OTHERS_HASH})
