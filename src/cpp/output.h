#ifndef THESIS_WORK_OUTPUT_H
#define THESIS_WORK_OUTPUT_H

#include <filesystem>
#include <fstream>
#include <iostream>

#include <boost/json.hpp>

namespace tests::out {
    // Структура, которая хранит json-объект и
    // объект класса ofstream, для вывода json в файл
    struct OutputJson {
        boost::json::object obj;
        std::ofstream out;
    };

    // Класс для логирования. Осуществляет одновременный вывод лога
    // в консоль и в файл
    class Logger
    {
    public:
        explicit Logger(const std::filesystem::path& log_dir_path);

        // Операторы вывода
        template<typename T> Logger& operator<<(T t);
        Logger& operator<<(std::ostream& (*fun)(std::ostream& ));

        // Метод, для получения пути, где хранится папка,
        // в которую будут сохранены результаты тестов и лог
        const std::filesystem::path& GetLogDirPath() const;

    private:
        std::ofstream log_file_;
        const std::filesystem::path log_dir_path_;
    };

    // Оператор вывода
    template<typename T> inline Logger& Logger::operator<<(T t) {
        log_file_ << t;
        std::cout << t;
        return *this;
    }

    // Оператор вывода. Необходим для std::endl
    inline Logger& Logger::operator<<(std::ostream& (*fun)(std::ostream&)) {
        log_file_ << std::endl;
        std::cout << std::endl;
        return *this;
    }

    // Конструирует класс Logger
    Logger CreateLogger();

    // Класс для вывода лога о начале и конце тестов из одной группы
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

    // Класс для вывода лога о начале и конце тестов одной битности
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
