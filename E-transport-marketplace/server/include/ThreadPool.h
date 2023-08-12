//
// Created by Nathan Ellis on 04/04/2023.
//
/**
 * @file ThreadPool.hpp
 * @brief A ThreadPool implementation based on std::thread and std::mutex.
 */


#ifndef SERVER_THREADPOOL_H
#define SERVER_THREADPOOL_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iterator>
#include <stack>
#include <functional>

using namespace std;

/**
 * @struct Task
 * @brief A struct that represents a task to be executed by a ThreadPool.
 */
struct Task {
    std::function<void()> func; /**< The function to be executed. */
    bool isCompleted; /**< A flag indicating whether the task has been completed or not. */
};

/**
 * @class ThreadPool
 * @brief A ThreadPool implementation based on std::thread and std::mutex.
 */
class ThreadPool {
public:
    /**
     * @brief Returns the number of threads in the ThreadPool.
     * @return The number of threads in the ThreadPool.
     */
    size_t getNumThreads() const {
        return m_threads.size();
    }

    /**
     * @brief Returns a reference to the thread at the given index.
     * @param index The index of the thread.
     * @return A reference to the thread at the given index.
     */
    std::thread &getThread(size_t index) { return m_threads[index]; }

    /**
     * @brief Constructs a new ThreadPool with the given number of threads.
     * @param numThreads The number of threads in the ThreadPool.
     */
    ThreadPool(size_t numThreads) {
        for (size_t i = 0; i < numThreads; ++i) {
            m_threads.emplace_back([this] {
                while (true) {
                    std::unique_lock<std::mutex> lock(m_taskMutex);
                    m_taskCV.wait(lock, [this] {
                        return !m_tasks.empty() || m_isStopping;
                    });
                    if (m_isStopping && m_tasks.empty()) {
                        return;
                    }
                    Task task = std::move(m_tasks.top());
                    m_tasks.pop();
                    lock.unlock();
                    task.func();
                    task.isCompleted = true;
                }
            });
        }
    }

    /**
     * @brief Adds a new task to the ThreadPool.
     * @param taskFunc The function to be executed by the task.
     */
    void addTask(std::function<void()> taskFunc) {
        std::unique_lock<std::mutex> lock(m_taskMutex);
        m_tasks.emplace(Task{std::move(taskFunc), false});
        lock.unlock();
        m_taskCV.notify_one();
    }

    /**
     * @brief Waits until all tasks in the ThreadPool have been completed.
     */
    void wait() {
        std::unique_lock<std::mutex> lock(m_taskMutex);
        m_taskCV.wait(lock, [this] {
            bool allCompleted = true;
            auto tasksCopy = m_tasks;
            while (!tasksCopy.empty()) {
                const Task &task = tasksCopy.top();
                if (!task.isCompleted) {
                    allCompleted = false;
                    break;
                }
                tasksCopy.pop();
            }
            return allCompleted;
        });
    }

    /**
     * @brief Destroys the ThreadPool and all its threads.
     */
    ~ThreadPool() {
        std::unique_lock<std::mutex> lock(m_taskMutex);
        m_isStopping = true;
        lock.unlock();
        m_taskCV.notify_all();
        for (std::thread &t: m_threads) {
            t.join();
        }
    }

private:
    std::stack<Task> m_tasks; /**< A stack of tasks to be executed by the ThreadPool. */
    std::vector<std::thread> m_threads; /**< A vector of threads in the ThreadPool. */
    mutex m_taskMutex;
    condition_variable m_taskCV;
    bool m_isStopping = false;
};

#endif //SERVER_THREADPOOL_H
