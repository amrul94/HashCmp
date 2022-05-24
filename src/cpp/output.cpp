#include "output.h"

namespace tests::out {
    Logger::Logger(const std::filesystem::path& log_dir_path)
            : log_file_(log_dir_path / "log.txt")
            , log_dir_path_(log_dir_path) {
        BOOST_ASSERT_MSG(log_file_, "Failed to create log file");
    };

    // Метод, для получения пути, где хранится папка,
    // в которую будут сохранены результаты тестов и лог
    const std::filesystem::path& Logger::GetLogDirPath() const {
        return log_dir_path_;
    }

    // Возвращает текущее время
    static inline std::string CurrentTime() {
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(tp);
        std::string ts = std::ctime(&t);
        ts.resize(ts.size() - 1);
        ts.replace(ts.find(':'), 1, ".");
        ts.replace(ts.find(':'), 1, ".");
        return ts;
    }

    // Конструирует класс Logger
    Logger CreateLogger() {
        const auto current_path = std::filesystem::current_path();
        const std::filesystem::path reports_dir {"reports"};
        const auto reports_path = current_path / reports_dir / CurrentTime() / "cpp";
        std::filesystem::create_directories(reports_path);
        return Logger{reports_path};
    }

    // Конструктор StartAndEndLogTest.
    // Выводит в лог, информацию о начале текущего текста
    StartAndEndLogTest::StartAndEndLogTest(Logger& logger, std::string_view test_name)
            : logger_(logger)
            , test_name_(test_name) {
        logger_ << "=== START " << test_name_ << " TEST ===\n\n";
    }

    // Деструктор StartAndEndLogTest.
    // Выводит в лог, информацию об окончании текущего текста
    StartAndEndLogTest::~StartAndEndLogTest() {
        logger_ << "=== END " << test_name_ << " TEST ===\n\n\n";
    }

    // Конструктор StartAndEndLogBitsTest.
    // Выводит в лог, информацию о начале текущего текста
    StartAndEndLogBitsTest::StartAndEndLogBitsTest(Logger& logger, uint16_t hash_bits)
            : logger_(logger)
            , hash_bits_(hash_bits) {
        logger_ << "--- START " << hash_bits_ << " BITS TEST ---\n";
    }

    // Деструктор StartAndEndLogBitsTest.
    // Выводит в лог, информацию об окончании текущего текста
    StartAndEndLogBitsTest::~StartAndEndLogBitsTest() {
        logger_ << "--- END " << hash_bits_ << " BITS TEST ---\n\n";
    }
}

