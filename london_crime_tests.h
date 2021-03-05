//
// Created by amrulla on 02.03.2021.
//

#ifndef THESIS_WORK_LONDON_CRIME_TESTS_H
#define THESIS_WORK_LONDON_CRIME_TESTS_H

#include <unordered_map>
#include <utility>

#include <boost/container_hash/hash.hpp>

#include "csv_parser/csv.h"

#include "hash_wrappers.h"
#include "log_duration.h"

struct LondonCrime {
    LondonCrime(std::string lsoa_code, std::string borough, std::string major_category, std::string minor_category,
                int value, int year, int month)
            : lsoa_code(std::move(lsoa_code))
            , borough(std::move(borough))
            , major_category(std::move(major_category))
            , minor_category(std::move(minor_category))
            , year(year)
            , month(month)
            , value(value){
    }

    std::string lsoa_code, borough, major_category, minor_category;
    int year, month, value;
};


template<typename Value, typename HashType>
using StringTable = std::unordered_map<std::string, Value, HashType>;

template<typename Value, typename HashType>
using IntTable = std::unordered_map<int, Value, HashType>;

template<typename HashType>
using LondonCrimeTable = StringTable<StringTable<StringTable<StringTable<IntTable<IntTable<int, std::hash<int>>, std::hash<int>>, HashType>, HashType>, HashType>, HashType>;

template <typename Table>
void buildTable(Table& table) {
    io::CSVReader<7> in("data/London Crime.csv");
    in.read_header(io::ignore_extra_column, "lsoa_code", "borough","major_category","minor_category","value","year","month");

    std::string lsoa_code, borough, major_category, minor_category;
    int value, year, month;

    //int count_value = 0;
    while(in.read_row(lsoa_code, borough, major_category, minor_category, value, year, month)){
        //std::cout << lsoa_code << std::endl;
        table[lsoa_code][borough][major_category][minor_category][year][month] = value;
        //count_value += table[lsoa_code][borough][major_category][minor_category][year][month];
    }
    //std::cout << "count_value = " << count_value << std::endl;
}

template <typename Table>
void test(Table& table, std::string_view hash_name, std::ostream& out = std::cout) {
    std::cout << hash_name << ":" << std::endl;
    size_t collisions = 0, empty = 0, max_bucket = 0;
    for (auto bucket = table.bucket_count(); bucket--;) {
        if (table.bucket_size(bucket) == 0)
            empty++;
        else {
            const size_t bucket_size = table.bucket_size(bucket);
            if (max_bucket < bucket_size) {
                max_bucket = bucket_size;
            }
            collisions += bucket_size - 1;
        }

    }

    out << "max_load_factor = " << table.max_load_factor() << std::endl
        << "load_factor = " << table.load_factor() << std::endl
        << "bucket_count = " << table.bucket_count() << std::endl
        << "collisions = " << collisions << std::endl
        << "max_bucket = " << max_bucket << std::endl
        << "empty buckets = " << empty << std::endl;
}

template<typename HashType>
void RunTest(std::string_view hash_name, std::ostream& out = std::cout) {
    LondonCrimeTable<HashType> table;
    buildTable(table);
    test(table, hash_name);
    std::cout << std::endl;
}

void runLondonCrimeTests();

#endif //THESIS_WORK_LONDON_CRIME_TESTS_H
