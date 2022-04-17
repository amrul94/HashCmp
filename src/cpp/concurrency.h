#ifndef THESISWORK_CONCURRENCY_H
#define THESISWORK_CONCURRENCY_H

#include <future>
#include <vector>
#include <thread>

static inline size_t GetNumThreads() {
    static const size_t hardware_threads = std::thread::hardware_concurrency();
    return hardware_threads != 0 ? hardware_threads : 1;
}

template<typename ResultType>
class ThreadTasks {
    using TaskFunction = std::function<ResultType(uint64_t, uint64_t)>;
    using MergeResultFunction = std::function<void(ResultType&, const ResultType&)>;
    using Future = std::future<ResultType>;
    using PackagedTask = std::packaged_task<ResultType(uint64_t, uint64_t)>;

public:
    ThreadTasks(const ThreadTasks&) = delete;
    ThreadTasks(ThreadTasks&&) = default;

    ThreadTasks& operator=(const ThreadTasks&) = delete;
    ThreadTasks& operator=(ThreadTasks&&) = default;

    ThreadTasks(TaskFunction task, MergeResultFunction merge_results, uint16_t num_threads, size_t num_iterations)
        : task_(task)
        , merge_results_(merge_results)
        , num_par_threads_(num_threads - 1)
        , num_iterations_(num_iterations)
        , block_size_(num_iterations / num_threads) {
        StartThreads();
    }

    ResultType GetResult()  {
        for(size_t i = 0; i < num_par_threads_; ++i) {
            auto current_result = futures_[i].get();
            merge_results_(result_, current_result);
        }
        return result_;
    }


private:
    void StartThreads()  {
        for (uint64_t i = 0; i < num_par_threads_; ++i) {
            PackagedTask current_task(task_);
            futures_.emplace_back(std::move(current_task.get_future()));
            threads_.emplace_back(std::move(current_task), 0, block_size_);
        }

        const uint64_t last_block_size = num_iterations_ - block_size_ * num_par_threads_;
        result_ = task_(0, last_block_size);
    }

    TaskFunction task_;
    MergeResultFunction merge_results_;
    uint16_t num_par_threads_;
    size_t num_iterations_;
    size_t block_size_;
    std::vector<Future> futures_;
    std::vector<std::jthread> threads_;
    ResultType result_;

};

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

#endif //THESISWORK_CONCURRENCY_H
