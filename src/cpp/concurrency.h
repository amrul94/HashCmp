#ifndef THESISWORK_CONCURRENCY_H
#define THESISWORK_CONCURRENCY_H

#include <future>
#include <vector>
#include <thread>

template<typename ResultType>
class ThreadTasks {
    using TaskFunction = std::function<ResultType(size_t, size_t)>;
    using MergeResultFunction = std::function<void(ResultType&, const ResultType&)>;
    using Future = std::future<ResultType>;
    using PackagedTask = std::packaged_task<ResultType(size_t, size_t)>;

public:
    ThreadTasks(const ThreadTasks&) = delete;
    ThreadTasks(TaskFunction task, MergeResultFunction merge_results, uint16_t num_threads, size_t num_iterations)
        : task_(task)
        , merge_results_(merge_results)
        , num_par_threads_(num_threads - 1)
        , num_iterations_(num_iterations)
        , block_size_(num_iterations / num_threads)
        , futures_(num_par_threads_)
        , threads_(num_par_threads_) {
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
        for (size_t i = 0; i < num_par_threads_; ++i) {
            PackagedTask current_task(task_);
            futures_[i] = current_task.get_future();
            threads_[i] = std::jthread(std::move(current_task), 0, block_size_);
        }

        const uint64_t last_block_size = num_iterations_ - block_size_ * num_par_threads_;
        result_ = task_(0, last_block_size);
    }

    TaskFunction task_;
    MergeResultFunction merge_results_;
    const uint16_t num_par_threads_;
    const size_t num_iterations_;
    const size_t block_size_;
    std::vector<Future> futures_;
    std::vector<std::jthread> threads_;
    ResultType result_;

};

#endif //THESISWORK_CONCURRENCY_H
