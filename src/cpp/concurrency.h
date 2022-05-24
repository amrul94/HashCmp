#ifndef THESIS_WORK_CONCURRENCY_H
#define THESIS_WORK_CONCURRENCY_H

#include <future>
#include <thread>
#include <vector>

// Возвращает число потоков, которое можно запустить в системе
static inline uint16_t GetNumThreads() {
    const uint16_t hardware_threads = std::thread::hardware_concurrency();
    return hardware_threads != 0 ? hardware_threads : 1;
}

// Класс, в котором запускаются функции в разных потоках.
// ResultType - возвращаемое значение для функции, которая запускается в отдельном потоке
template<typename ResultType>
class ThreadTasks {
    // Функция запускаемая, в отдельном потоке
    using TaskFunction = std::function<ResultType(uint64_t, uint64_t)>;
    // Функция, которая объединяет информацию полученную из разных потоков
    using MergeResultFunction = std::function<void(ResultType&, const ResultType&)>;
    // Фьючерс, в который будет записан результат выполнения функции запущенной в отдельном потоке
    using Future = std::future<ResultType>;
    // Обертка над функцией запускаемой, в отдельном потоке.
    // Необходима для получения фьючерса (Future)
    using PackagedTask = std::packaged_task<ResultType(uint64_t, uint64_t)>;

public:
    ThreadTasks(const ThreadTasks&) = delete;
    ThreadTasks(ThreadTasks&&) = default;

    ThreadTasks& operator=(const ThreadTasks&) = delete;
    ThreadTasks& operator=(ThreadTasks&&) = default;

    // Конструктор, который запускает выполнение функций (task) в разных потоках
    ThreadTasks(TaskFunction task, MergeResultFunction merge_results, uint16_t num_threads, size_t num_iterations)
        : task_(task)
        , merge_results_(merge_results)
        , num_par_threads_(num_threads - 1)
        , num_iterations_(num_iterations)
        , block_size_(num_iterations / num_threads) {
        StartThreads();
    }

    // Возвращает объединенный результат вычисления из всех потокв
    ResultType GetResult()  {
        for(size_t i = 0; i < num_par_threads_; ++i) {
            auto current_result = futures_[i].get();
            merge_results_(result_, current_result);
        }
        return result_;
    }


private:
    // который запускает выполнение функций (task) в разных потоках
    void StartThreads()  {
        for (uint64_t i = 0; i < num_par_threads_; ++i) {
            PackagedTask current_task(task_);
            futures_.emplace_back(std::move(current_task.get_future()));
            threads_.emplace_back(std::move(current_task), 0, block_size_);
        }

        const uint64_t last_block_size = num_iterations_ - block_size_ * num_par_threads_;
        result_ = task_(0, last_block_size);
    }

    TaskFunction task_;                 // Функция, которая запускается в отдельном потоке
    MergeResultFunction merge_results_; // Функция, которая объединяет результаты вычисления из разных потоков
    uint16_t num_par_threads_;          // Число потоков
    size_t num_iterations_;             // Число итераций для всех потоков
    size_t block_size_;                 // Размер блока, запускаемого в отдельном потоке
    std::vector<Future> futures_;       // Фьючерсы, в которые будут записаны результаты выполнения потоков
    std::vector<std::jthread> threads_; // Потоки выполнения
    ResultType result_;                 // Общий результат выполнения всех потоков
};


// Специализация ThreadTasks, потоки в которой ничего не возвращают
template<>
class ThreadTasks<void> {
    using TaskFunction = std::function<void(uint64_t, uint64_t)>;

public:
    ThreadTasks(const ThreadTasks&) = delete;
    ThreadTasks(ThreadTasks&&) = default;

    ThreadTasks& operator=(const ThreadTasks&) = delete;
    ThreadTasks& operator=(ThreadTasks&&) = default;

    ThreadTasks(TaskFunction task, uint16_t num_threads, size_t num_iterations)
            : task_(task)
            , num_par_threads_(num_threads - 1)
            , num_iterations_(num_iterations)
            , block_size_(num_iterations / num_threads)
            , threads_(num_par_threads_){
        StartThreads();
        JoinThreads();
    }

private:
    void StartThreads()  {
        uint64_t start_block = 0;
        for (auto& t : threads_) {
            uint64_t end_block = start_block + block_size_;
            t = std::thread(task_, start_block, end_block);
            start_block = end_block;
        }
        task_(start_block, num_iterations_);
    }

    void JoinThreads() {
        for (auto& t : threads_) {
            t.join();
        }
    }

    TaskFunction task_;
    uint16_t num_par_threads_;
    size_t num_iterations_;
    size_t block_size_;
    std::vector<std::thread> threads_;
};

#endif //THESIS_WORK_CONCURRENCY_H
