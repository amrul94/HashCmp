cmake_minimum_required(VERSION 3.17)
project(Thesis_work)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(Thesis_work main.cpp log_duration.h hash_wrappers.h hash_wrappers.cpp
        london_crime_tests.h london_crime_tests.cpp
        csv_parser/csv.h
        hashes/cityhash/city.h hashes/cityhash/city.cc hashes/cityhash/config.h
        hashes/farmhash/farmhash.h hashes/farmhash/farmhash.cc
        hashes/xx_hash/xxhash.h hashes/xx_hash/xxhash.c hashes/xx_hash/xxh3.h
        hashes/murmur_hash/MurmurHash1.h hashes/murmur_hash/MurmurHash1.cpp
        hashes/murmur_hash/MurmurHash2.h hashes/murmur_hash/MurmurHash2.cpp
        hashes/murmur_hash/MurmurHash3.h hashes/murmur_hash/MurmurHash3.cpp)
