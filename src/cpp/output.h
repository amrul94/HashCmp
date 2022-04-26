#ifndef THESIS_WORK_OUTPUT_H
#define THESIS_WORK_OUTPUT_H

#include <filesystem>
#include <fstream>
#include <iostream>

#include <boost/json.hpp>

namespace tests::out {
    struct OutputJson {
        boost::json::object obj;
        std::ofstream out;
    };

    class Logger
    {
    public:
        explicit Logger(const std::filesystem::path& log_dir_path);

        template<typename T> Logger& operator<<(T t);
        Logger& operator<<(std::ostream& (*fun)(std::ostream& ));

        const std::filesystem::path& GetLogDirPath() const;

    private:
        std::ofstream log_file_;
        const std::filesystem::path log_dir_path_;
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

    Logger CreateLogger();

    class StartAndEndLogTest {
    public:
        StartAndEndLogTest() = delete;
        StartAndEndLogTest(const StartAndEndLogTest&) = default;
        StartAndEndLogTest(StartAndEndLogTest&&) = default;
        StartAndEndLogTest(Logger& logger, std::string_view test_name);

        StartAndEndLogTest& operator=(const StartAndEndLogTest&) = delete;
        StartAndEndLogTest& operator=(StartAndEndLogTest&&) = delete;

        ~StartAndEndLogTest();

    private:
        Logger& logger_;
        std::string_view test_name_;
    };

    class StartAndEndLogBitsTest {
    public:
        StartAndEndLogBitsTest() = delete;
        StartAndEndLogBitsTest(const StartAndEndLogBitsTest&) = default;
        StartAndEndLogBitsTest(StartAndEndLogBitsTest&&) = default;
        StartAndEndLogBitsTest(Logger& logger, uint16_t hash_bits);

        StartAndEndLogBitsTest& operator=(const StartAndEndLogBitsTest&) = delete;
        StartAndEndLogBitsTest& operator=(StartAndEndLogBitsTest&&) = delete;

        ~StartAndEndLogBitsTest();

    private:
        Logger& logger_;
        uint16_t hash_bits_;
    };
}




#endif //THESIS_WORK_OUTPUT_H
