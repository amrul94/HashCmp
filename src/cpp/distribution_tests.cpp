#include "distribution_tests.h"

#include <span>

namespace tests {
    namespace {
        // Класс, в котором собирается и хранится информация о распределительных свойствах хеш функций
        class DistributionStatistics {
        public:
            static constexpr uint16_t num_bars = 16;
            static constexpr uint16_t shift = 4;

            DistributionStatistics(const std::vector<std::atomic<Bucket>>& buckets,
                                   const DistTestParameters& parameters, const std::string& hash_name);
            // Возвращает в json информацию о распределительных свойствах
            boost::json::object GetStatistics();

        private:
            // Запускает сбор информации в разных потоках
            void RunThreads();
            // Собирает информацию
            void CollectingStatistics(uint64_t first_bar, uint64_t last_bar);

            // Функции Set... записывают информацию в json
            void SetStatistics();

            void SetXRanges(uint64_t first, uint64_t last, size_t bar_index);
            void SetY(ptrdiff_t first, ptrdiff_t last, uint64_t step, size_t bar_index);

            template<typename Span>
            void SetYMean(const Span& y_block, uint64_t step, size_t bar_index);

            template<typename Span>
            void SetYMin(const Span& y_block, size_t bar_index);

            template<typename Span>
            void SetYMax(const Span& y_block, size_t bar_index);

            void SetYErrMin(size_t bar_index);
            void SetYErrMax(size_t bar_index);

            const DistTestParameters& parameters_;              // параметры тестирования
            const std::vector<std::atomic<Bucket>>& buckets_;   // вектор хеш-значений

            // Параметры ниже нужны для постройки гистограммы
            boost::json::array x_bars_;                         // индексы диапазонов
            boost::json::array x_ranges_;                       // диапазоны хеш-значений
            boost::json::array y_min_;                          // минимальные значения в диапазонах
            boost::json::array y_max_;                          // максимальные значения в диапазонах
            boost::json::array y_err_min_;                      // разность минимальных и средних значений
            boost::json::array y_err_max_;                      // разность максимальных и средних значений
            boost::json::array y_mean_;                         // средние значения в диапазонах

            boost::json::object distribution_statistics_;       // общая статистика
        };

        // Конструктор, который формирует массивы и json. В них будут записаны данные тестирования
        DistributionStatistics::DistributionStatistics(const std::vector<std::atomic<Bucket>>& buckets,
                               const DistTestParameters& parameters, const std::string& hash_name)
               : parameters_(parameters)
               , buckets_(buckets)
               , x_bars_(num_bars)
               , x_ranges_(num_bars)
               , y_min_(num_bars)
               , y_max_(num_bars)
               , y_err_min_(num_bars)
               , y_err_max_(num_bars)
               , y_mean_(num_bars) {
            distribution_statistics_["Test name"] = "Test Check Distribution";
            distribution_statistics_["Mode"] = TestFlagToString(parameters.mode);
            distribution_statistics_["Bits"] = parameters.hash_bits;
            distribution_statistics_["Hash name"] = hash_name;
            std::iota(x_bars_.begin(), x_bars_.end(), 1);
        }

        // Собирает информацию о распределительных свойствах хеш функции
        // и возвращает ее в json
        boost::json::object DistributionStatistics::GetStatistics() {
            RunThreads();
            SetStatistics();
            return distribution_statistics_;
        }

        // Запускает сбор информации в разных потоках
        void DistributionStatistics::RunThreads() {
            auto lambda = [this](uint64_t start_bar, uint64_t end_bar) {
                this->CollectingStatistics(start_bar, end_bar);
            };
            ThreadTasks<void> thread_tasks(lambda, parameters_.num_threads, num_bars);
        }

        // Собирает информацию
        void DistributionStatistics::CollectingStatistics(uint64_t first_bar, uint64_t last_bar) {
            uint64_t step = (buckets_.size() >> 4);
            for (uint32_t bar = first_bar; bar < last_bar; ++bar) {
                const auto begin = static_cast<ptrdiff_t>(bar * step);
                const auto end = begin + static_cast<ptrdiff_t>(step);

                SetXRanges(begin, end, bar);
                SetY(begin, end, step, bar);
            }
        }

        // Объединяет всю собранную информацию в одном json
        void DistributionStatistics::SetStatistics() {
            distribution_statistics_["Bar count"] = num_bars;
            distribution_statistics_["Bin size"] = parameters_.divisor;
            distribution_statistics_["X ranges"] = std::move(x_ranges_);
            distribution_statistics_["Y mean"] = std::move(y_mean_);
            distribution_statistics_["Y err min"] = std::move(y_err_min_);
            distribution_statistics_["Y err max"] = std::move(y_err_max_);
            distribution_statistics_["Y min"] = std::move(y_min_);
            distribution_statistics_["Y max"] = std::move(y_max_);
        }

        // Устанавливает значение диапазона в x_ranges_[bar_index]
        void DistributionStatistics::SetXRanges(uint64_t first, uint64_t last, size_t bar_index) {
            uint64_t median_index = (last + first) >> 1;
            uint64_t median = median_index * parameters_.divisor;
            x_ranges_[bar_index] = median;
        }

        // Устанавливает значения гистограмм по оси Y
        void DistributionStatistics::SetY(ptrdiff_t first, ptrdiff_t last, uint64_t step, size_t bar_index) {
            const std::span y_block(buckets_.begin() + first, buckets_.begin() + last);
            SetYMean(y_block, step, bar_index);
            SetYMin(y_block, bar_index);
            SetYMax(y_block, bar_index);
            SetYErrMin(bar_index);
            SetYErrMax(bar_index);
        }

        // Вычисляет среднее значение для одного столбца гистограммы (индекс столбца = bar_index)
        template<typename Span>
        void DistributionStatistics::SetYMean(const Span& y_block, uint64_t step, size_t bar_index) {
            const long double sum_value = std::accumulate(y_block.begin(), y_block.end(),
                                                          static_cast<long double>(0));
            const long double avg_value = sum_value / static_cast<long double>(step);
            y_mean_[bar_index] = static_cast<double>(avg_value);
        }
        // Вычисляет минимальное значение для одного столбца гистограммы (индекс столбца = bar_index)
        template<typename Span>
        void DistributionStatistics::SetYMin(const Span& y_block, size_t bar_index) {
            const auto min_value = std::ranges::min_element(y_block);
            y_min_[bar_index] = *min_value;
        }

        // Вычисляет максимальное значение для одного столбца гистограммы (индекс столбца = bar_index)
        template<typename Span>
        void DistributionStatistics::SetYMax(const Span& y_block, size_t bar_index) {
            const auto max_value = std::ranges::max_element(y_block);
            y_max_[bar_index] = *max_value;
        }

        // Вычисляет ошибку для минимального значения (индекс столбца = bar_index)
        void DistributionStatistics::SetYErrMin(size_t bar_index) {
            const auto min_value = static_cast<double>(y_min_[bar_index].get_uint64());
            const double avg_value = y_mean_[bar_index].get_double();
            const auto min_diff = std::abs(avg_value - min_value);
            y_err_min_[bar_index] = min_diff;
        }

        // Вычисляет ошибку для максимального значения (индекс столбца = bar_index)
        void DistributionStatistics::SetYErrMax(size_t bar_index) {
            const auto max_value = static_cast<double>(y_max_[bar_index].get_uint64());
            const double avg_value = y_mean_[bar_index].get_double();
            const auto max_diff = std::abs(avg_value - max_value);
            y_err_max_[bar_index] = max_diff;
        }

        // Формирует json-файл, в который будет сохранена информация с теста распределительных свойств
        out::OutputJson GetDistTestJson(const DistTestParameters& parameters, const std::string& hash_name,
                                        out::Logger& logger) {
            using namespace std::literals;
            const std::filesystem::path check_dist_dir = "Distribution tests";
            const std::filesystem::path hash_bits_dir = std::to_string(parameters.hash_bits);
            const std::filesystem::path report_name = hash_name + " with "s + TestFlagToString(parameters.mode) + " mode.json";
            const auto hash_bits_path = logger.GetLogDirPath() / check_dist_dir / hash_bits_dir;
            std::filesystem::create_directories(hash_bits_path);

            const std::filesystem::path hash_out_path = hash_bits_path / report_name;
            std::ofstream out(hash_out_path);
            return out::OutputJson{boost::json::object{}, std::move(out)};
        }

        // Собирает статистику тестирования распределительных свойств
        inline boost::json::object ProcessingStatistics(const std::vector<std::atomic<Bucket>>& buckets,
                                                 const DistTestParameters& parameters, const std::string& hash_name) {
            DistributionStatistics statistics{buckets, parameters, hash_name};
            return statistics.GetStatistics();
        }

        // Выводит лог результатов тестирования
        void PrintDistributionTestLog(const boost::json::object& object, out::Logger& logger) {
            logger << "\t\tX ranges: " << object.at("X ranges") << '\n';
            logger << "\t\tY mean: " << object.at("Y mean") << '\n';
            logger << "\t\tY min: " << object.at("Y min") << '\n';
            logger << "\t\tY max: " << object.at("Y max") << '\n';
            logger << "\t\tY err min: " << object.at("Y err min") << '\n';
            logger << "\t\tY err max: " << object.at("Y err max") << std::endl;
        }
    }

    namespace out {
        // Сохраняет результаты тестов в json файл
        void SaveReport(const std::vector<std::atomic<Bucket>>& buckets, const DistTestParameters& parameters,
                        const std::string& hash_name, out::Logger& logger) {
            auto out_json = GetDistTestJson(parameters, hash_name, logger);
            out_json.obj = ProcessingStatistics(buckets, parameters, hash_name);
            PrintDistributionTestLog(out_json.obj, logger);
            out_json.out << out_json.obj;
        }
    }

    DistributionHashes::DistributionHashes(size_t num_buckets)
        : buckets_(num_buckets) {
    }

    // Добавляет хеш-значение
    void DistributionHashes::AddHash(uint64_t hash_value) {
        // Добавление нового значения в вектор осуществляется с помощью CAS
        std::atomic<Bucket>& current_bucket = buckets_[hash_value];
        Bucket old_bucket = 0, new_bucket{};
        do {
            old_bucket = current_bucket.load();
            Bucket increment = (old_bucket != max_bucket) ? 1 : 0;
            new_bucket = old_bucket + increment;
        } while (!current_bucket.compare_exchange_weak(old_bucket, new_bucket));
    }

    // Возвращает вектор хеш-значений
    const std::vector<std::atomic<Bucket>>& DistributionHashes::GetBuckets() const {
        return buckets_;
    }

    // Запускает тестирование распределительных свойств
    // всех хеш функций одной битности
    template<hfl::UnsignedIntegral UintT>
    static void RunDistributionTestsImpl(uint16_t num_threads, out::Logger& logger) {
        constexpr uint16_t bits = std::numeric_limits<UintT>::digits;
        const auto hashes = hfl::BuildHashes<UintT>();

        if constexpr(std::is_same_v<UintT, uint64_t>) {
            const uint64_t num_keys_and_buckets = 1ull << (bits - 32);
            const DistTestParameters parameters{bits, num_threads, num_keys_and_buckets, num_keys_and_buckets,
                                                TestFlag::BINS};
            DistributionTest(hashes, parameters, logger);
        } else {
            const uint64_t num_keys_and_buckets = 1ull << bits;
            const DistTestParameters parameters{bits, num_threads, num_keys_and_buckets, num_keys_and_buckets,
                                                TestFlag::NORMAL};
            DistributionTest(hashes, parameters, logger);
        }
    }

    // Запускает тестирование распределительных свойств
    // всех хеш функций всех битностей
    void RunDistributionTests(out::Logger& logger) {
        out::StartAndEndLogTest start_and_end_log(logger, "DISTRIBUTION");
        const uint16_t num_threads = GetNumThreads();
        logger << boost::format("\tnum_threads = %1%\n\n") % num_threads;

        RunDistributionTestsImpl<uint16_t>(num_threads, logger);
        RunDistributionTestsImpl<uint32_t>(num_threads, logger);
        RunDistributionTestsImpl<uint64_t>(num_threads, logger);
    }
}
