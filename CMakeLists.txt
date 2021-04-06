cmake_minimum_required(VERSION 3.17)
project(Thesis_work)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(Thesis_work main.cpp log_duration.h hash_wrappers.h hash_wrappers.cpp
        london_crime_tests.h london_crime_tests.cpp random_world_tests.h random_world_tests.cpp
        csv_parser/csv.h
        hashes/cityhash/city.h hashes/cityhash/city.cc hashes/cityhash/config.h
        hashes/farmhash/farmhash.h hashes/farmhash/farmhash.cc
        hashes/xx_hash/xxhash.h hashes/xx_hash/xxhash.c hashes/xx_hash/xxh3.h
        hashes/murmur_hash/MurmurHash1.h hashes/murmur_hash/MurmurHash1.cpp
        hashes/murmur_hash/MurmurHash2.h hashes/murmur_hash/MurmurHash2.cpp
        hashes/murmur_hash/MurmurHash3.h hashes/murmur_hash/MurmurHash3.cpp
        hashes/pearson/pearson.h hashes/pearson/pearson.cpp
        hashes/super_fast_hash/super_fast_hash.h hashes/super_fast_hash/super_fast_hash.cpp
        hashes/rolling_hash/cyclichash.h hashes/rolling_hash/rabinkarphash.h hashes/rolling_hash/characterhash.h
        hashes/rolling_hash/mersennetwister.h  hashes/rolling_hash/generalhash.h  hashes/rolling_hash/threewisehash.h
        hashes/rolling_hash/adler32.h hashes/various_hash_functions.h hashes/various_hash_functions.cpp hashes/t1ha_/t1ha_.h hashes/t1ha_/t1ha_.cpp

        hashes/t1ha/t1ha.h hashes/t1ha/t1ha0.c hashes/t1ha/t1ha0_ia32aes_a.h hashes/t1ha/t1ha0_ia32aes_avx.c
        hashes/t1ha/t1ha0_ia32aes_avx2.c hashes/t1ha/t1ha0_ia32aes_b.h hashes/t1ha/t1ha0_ia32aes_noavx.c
        hashes/t1ha/t1ha0_selfcheck.c hashes/t1ha/t1ha1.c hashes/t1ha/t1ha1_selfcheck.c
        hashes/t1ha/t1ha2.c hashes/t1ha/t1ha2_selfcheck.c hashes/t1ha/t1ha_bits.h hashes/t1ha/t1ha_selfcheck.c
        hashes/t1ha/t1ha_selfcheck.h hashes/t1ha/t1ha_selfcheck_all.c)
