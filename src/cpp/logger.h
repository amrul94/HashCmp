#ifndef THESISWORK_LOGGER_H
#define THESISWORK_LOGGER_H

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

class Logger
{
public:
    explicit Logger(const std::filesystem::path& log_file_path)
        : log_file_(log_file_path) {
        assert(log_file_);
    };

    // overload the << operator to log all standard data types
    template<typename T> Logger& operator<<(T t);
    // this is needed so << std::endl works
    Logger& operator<<(std::ostream& (*fun)(std::ostream& ) );

private:
    std::ofstream log_file_;
};

template<typename T> inline Logger& Logger::operator<<(T t) {
    log_file_ << t;
    std::cout << t;
    return *this;
}

inline Logger& Logger::operator<<(std::ostream& (*fun)(std::ostream&)) {
    log_file_ << std::endl;
    std::cout << std::endl;
    return *this;
}

#endif //THESISWORK_LOGGER_H
