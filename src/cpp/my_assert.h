#ifndef THESIS_WORK_MY_ASSERT_H
#define THESIS_WORK_MY_ASSERT_H

#include <execution>
#include <iostream>

// Функция для проверки выполнения условий равенства
template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const std::string& t_str, const std::string& u_str, const std::string& file,
                     const std::string& func, unsigned line, const std::string& hint) {
    using std::cerr;
    using namespace std::literals;
    if (t != u) {
        cerr << std::boolalpha;
        cerr << file << "("s << line << "): " << func << ": ";
        cerr << "ASSERT_EQUAL("s << t_str << ", " << u_str << ") failed: ";
        cerr << t << " != " << u << ".";
        if (!hint.empty()) {
            cerr << " Hint: " << hint;
        }
        cerr << std::endl;
        abort();
    }
}

// Макросы для проверки выполнения условий равенства
#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, "")
#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

#endif //THESIS_WORK_MY_ASSERT_H
