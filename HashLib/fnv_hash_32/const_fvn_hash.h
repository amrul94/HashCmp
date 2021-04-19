// C++ 32bit FNV-1a string hashing.
// Support both compile-time and runtime calulations.
// This implementation also HashLib '\0' terminator.
// Requires a compiler with C++11 support.
// See main for examples.

#ifndef THESIS_WORK_CONST_FVN_HASH_H
#define THESIS_WORK_CONST_FVN_HASH_H

#include <cstring>
#include <iostream>

namespace hardrock
{
    class FnvHash
    {
        static const unsigned int FNV_PRIME = 16777619u;
        static const unsigned int OFFSET_BASIS = 2166136261u;
        template <unsigned int N>
        static constexpr unsigned int fnvHashConst(const char (&str)[N], unsigned int I = N)
        {
            return I == 1 ? (OFFSET_BASIS ^ str[0]) * FNV_PRIME : (fnvHashConst(str, I - 1) ^ str[I - 1]) * FNV_PRIME;
        }
        static unsigned int fnvHash(const char* str)
        {
            const size_t length = strlen(str) + 1;
            unsigned int hash = OFFSET_BASIS;
            for (size_t i = 0; i < length; ++i)
            {
                hash ^= *str++;
                hash *= FNV_PRIME;
            }
            return hash;
        }
        struct Wrapper
        {
            Wrapper(const char* str) : str (str) { }
            const char* str;
        };
        unsigned int hash_value;
    public:
        // calulate in run-time
        FnvHash(Wrapper wrapper) : hash_value(fnvHash(wrapper.str)) { }
        // calulate in compile-time
        template <unsigned int N>
        constexpr FnvHash(const char (&str)[N]) : hash_value(fnvHashConst(str)) { }
        // output result
        constexpr operator unsigned int() const { return this->hash_value; }
    };
}

// output function that requires a compile-time constant, for testing
template <unsigned int N>
struct constN
{
    constN() { std::cout << N << std::endl; }
};
/*
int main(int argc, char* args[])
{
    // compile-time calulation
    constN<hardrock::FnvHash("Good")> out1;

    // runtime calulation
    const char* test_str = "Good";
    std::cout << hardrock::FnvHash(test_str) << std::endl;

    return 0;
}
*/
#endif //THESIS_WORK_CONST_FVN_HASH_H
