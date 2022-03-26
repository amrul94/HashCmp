#ifndef THESISWORK_CONCURRENCY_H
#define THESISWORK_CONCURRENCY_H

#include <future>
#include <vector>
#include <thread>

template<typename ResultType>
class ThreadTasks {
    using TaskFunction = std::function<ResultType(size_t, size_t)>;
    using MergeResultFunction = std::function<void(ResultType&, ResultType)>;
    using Future = std::future<ResultType>;
    using PackagedTask = std::packaged_task<ResultType(size_t, size_t)>;

public:
    ThreadTasks(const ThreadTasks&) = delete;
    ThreadTasks(TaskFunction task, MergeResultFunction merge_results, size_t num_threads, size_t num_iterations)
        : task_(task)
        , merge_results_(merge_results)
        , num_threads_(num_threads)
        , num_iterations_(num_iterations)
        , block_size_(num_iterations / num_threads)
        , futures_(num_threads - 1)
        , threads_(num_threads - 1) {
        StartThreads();
    }

    ResultType GetResult()  {
        for(size_t i = 0; i < (num_threads_ - 1); ++i) {
            auto current_result = futures_[i].get();
            merge_results_(result_, current_result);
        }
        return result_;
    }


private:
    void StartThreads()  {
        uint64_t start_block = 0;
        for (size_t i = 0; i < (num_threads_ - 1); ++i) {
            uint64_t end_block_ = start_block + block_size_;
            PackagedTask current_task(task_);
            futures_[i] = current_task.get_future();
            threads_[i] = std::jthread(std::move(current_task), start_block, end_block_);
            start_block = end_block_;
        }

        result_ = task_(start_block, num_iterations_);
    }

    TaskFunction task_;
    MergeResultFunction merge_results_;
    const size_t num_threads_;
    const size_t num_iterations_;
    const size_t block_size_;
    std::vector<Future> futures_;
    std::vector<std::jthread> threads_;
    ResultType result_;

};

#endif //THESISWORK_CONCURRENCY_H
