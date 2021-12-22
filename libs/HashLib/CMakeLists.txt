cmake_minimum_required(VERSION 3.17)

project(HashLib)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-pthread -Wno-deprecated-enum-enum-conversion -Woverflow -Wdeprecated-enum-enum-conversion")
set(CMAKE_C_FLAGS "-pthread -maes")

set(CITY_HASH cityhash/city.h cityhash/city.cc cityhash/config.h)
set(FARM_HASH farmhash/farmhash.h farmhash/farmhash.cc)
set(FAST_HASH fasthash/fasthash.h fasthash/fasthash.c)
set(JENKINS_HASH spooky_hash/spooky.h spooky_hash/spooky.c jenkins_hash/lookup3.cpp
        jenkins_hash/lookup3.h)
set(METRO_HASH metro_hash/metrohash64.h metro_hash/metrohash64.cpp metro_hash/platform.h)
set(MURMUR_HASH murmur_hash/MurmurHash1.h murmur_hash/MurmurHash1.cpp
        murmur_hash/MurmurHash2.h murmur_hash/MurmurHash2.cpp
        murmur_hash/MurmurHash3.h murmur_hash/MurmurHash3.cpp)
set(PEARSON_HASH pearson/pearson.h pearson/pearson.cpp)
set(ROLLING_HASH rolling_hash/cyclichash.h rolling_hash/rabinkarphash.h rolling_hash/characterhash.h
        rolling_hash/mersennetwister.h rolling_hash/generalhash.h rolling_hash/threewisehash.h
        rolling_hash/adler32.h)
set(SUPER_FAST_HASH super_fast_hash/super_fast_hash.h super_fast_hash/super_fast_hash.cpp)
set(T1HA_HASH t1ha/t1ha.h t1ha/t1ha0.c t1ha/t1ha0_ia32aes_a.h t1ha/t1ha0_ia32aes_avx.c
        t1ha/t1ha0_ia32aes_avx2.c t1ha/t1ha0_ia32aes_b.h t1ha/t1ha0_ia32aes_noavx.c
        t1ha/t1ha0_selfcheck.c t1ha/t1ha1.c t1ha/t1ha1_selfcheck.c
        t1ha/t1ha2.c t1ha/t1ha2_selfcheck.c t1ha/t1ha_bits.h t1ha/t1ha_selfcheck.c
        t1ha/t1ha_selfcheck.h t1ha/t1ha_selfcheck_all.c)
set(XX_HASH xx_hash/xxhash.h xx_hash/xxhash.c xx_hash/xxh3.h)
set(OTHERS_HASH hash_functions.h hash_functions.cpp)

add_library(HashLib STATIC ${CITY_HASH} ${FARM_HASH} ${FAST_HASH} ${JENKINS_HASH} ${METRO_HASH} ${MURMUR_HASH}
        ${PEARSON_HASH} ${ROLLING_HASH} ${SUPER_FAST_HASH} ${T1HA_HASH} ${XX_HASH} ${OTHERS_HASH})
