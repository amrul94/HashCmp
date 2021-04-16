cmake_minimum_required(VERSION 3.17)
project(Thesis_work)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -maes")

set(MAIN src/main.cpp)
set(UTILITIES src/log_duration.h)
set(WRAPPERS src/hash_wrappers.h src/hash_wrappers.cpp src/hashes.h src/hashes.cpp)
set(TESTS src/tests.h src/tests.cpp src/london_crime_tests.h src/london_crime_tests.cpp src/random_world_tests.h
        src/random_world_tests.cpp src/tests_with_images.h src/tests_with_images.cpp)
set(PROJECT_FILES ${MAIN} ${UTILITIES} ${WRAPPERS} ${TESTS})

set(PARSERS csv_parser/csv.h)

set(CITY_HASH hashes/cityhash/city.h hashes/cityhash/city.cc hashes/cityhash/config.h)
set(FARM_HASH hashes/farmhash/farmhash.h hashes/farmhash/farmhash.cc)
set(FAST_HASH hashes/fasthash/fasthash.h hashes/fasthash/fasthash.c)
set(FNV_HASH hashes/fnv_hash_32/const_fvn_hash.h hashes/fnv_hash_64_128/fnv.h hashes/fnv_hash_64_128/fnv.cpp)
set(JENKINS_HASH hashes/spooky_hash/spooky.h hashes/spooky_hash/spooky.c hashes/jenkins_hash/lookup3.cpp
        hashes/jenkins_hash/lookup3.h)
set(METRO_HASH hashes/metro_hash/metrohash64.h hashes/metro_hash/metrohash64.cpp hashes/metro_hash/platform.h)
set(MURMUR_HASH hashes/murmur_hash/MurmurHash1.h hashes/murmur_hash/MurmurHash1.cpp
        hashes/murmur_hash/MurmurHash2.h hashes/murmur_hash/MurmurHash2.cpp
        hashes/murmur_hash/MurmurHash3.h hashes/murmur_hash/MurmurHash3.cpp)
set(PEARSON_HASH hashes/pearson/pearson.h hashes/pearson/pearson.cpp)
set(ROLLING_HASH hashes/rolling_hash/cyclichash.h hashes/rolling_hash/rabinkarphash.h hashes/rolling_hash/characterhash.h
        hashes/rolling_hash/mersennetwister.h hashes/rolling_hash/generalhash.h hashes/rolling_hash/threewisehash.h
        hashes/rolling_hash/adler32.h)
set(SUPER_FAST_HASH hashes/super_fast_hash/super_fast_hash.h hashes/super_fast_hash/super_fast_hash.cpp)
set(T1HA_HASH hashes/t1ha/t1ha.h hashes/t1ha/t1ha0.c hashes/t1ha/t1ha0_ia32aes_a.h hashes/t1ha/t1ha0_ia32aes_avx.c
        hashes/t1ha/t1ha0_ia32aes_avx2.c hashes/t1ha/t1ha0_ia32aes_b.h hashes/t1ha/t1ha0_ia32aes_noavx.c
        hashes/t1ha/t1ha0_selfcheck.c hashes/t1ha/t1ha1.c hashes/t1ha/t1ha1_selfcheck.c
        hashes/t1ha/t1ha2.c hashes/t1ha/t1ha2_selfcheck.c hashes/t1ha/t1ha_bits.h hashes/t1ha/t1ha_selfcheck.c
        hashes/t1ha/t1ha_selfcheck.h hashes/t1ha/t1ha_selfcheck_all.c)
set(XX_HASH hashes/xx_hash/xxhash.h hashes/xx_hash/xxhash.c hashes/xx_hash/xxh3.h)
set(OTHERS_HASH hashes/other_hash_functions.h hashes/other_hash_functions.cpp)

set(HASHES ${CITY_HASH} ${FARM_HASH} ${FAST_HASH} ${FNV_HASH} ${JENKINS_HASH} ${METRO_HASH} ${MURMUR_HASH}
        ${PEARSON_HASH} ${ROLLING_HASH} ${SUPER_FAST_HASH} ${T1HA_HASH} ${XX_HASH} ${OTHERS_HASH})

add_executable(Thesis_work "${PROJECT_FILES}" ${PARSERS} ${HASHES})
