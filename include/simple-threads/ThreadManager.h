/**
 * This file is the manager of all threads, which follows from the name of the class.
 * Just include it in to take full advantage of multithreading!
 * 
 * @author N3WC0DER
 */

#pragma once
#include <queue>
#include <vector>
#include <thread>
#include <future>
#include <functional>

namespace sth {
/** Pattern: Singleton */
class ThreadManager {
private:
	/** 
	 * The constructor is needed to initialize threads 
	 * @param size_t threadCount number of threads to be created
	 */
	ThreadManager(size_t threadCount);
	ThreadManager(const ThreadManager&) = delete;
	ThreadManager& operator=(const ThreadManager&) = delete;
	// void operator delete(void*);
	/** PLEASE DON'T USE OPERATOR DELETE!! */
	
	/** Flag that specifies the state of the ThreadManager */
	std::atomic<bool> enabled;
	
	/** The queue from which threads take tasks */
	std::queue<std::function<void ()>> taskPool;
	/** This mutex synchronizes the task queue */
	std::mutex mutex_pool;
	/** condition_variable makes threads wait for a new task to be added if the queue is empty */
	std::condition_variable cv_pool;
	
	/** Array of threads that execute tasks */
	std::thread **threads = nullptr;
	
	std::atomic<size_t> threadCount;
	
	static std::unique_ptr<ThreadManager> instance;
	
	/** Adding a Function Task to the Queue */
	void addToQueue(std::function<void ()> func);
	
	/** A function that threads perform to execute tasks from a queue */
	void run();
	
public:
	/** Destructor where threads end and are freed */
	~ThreadManager();
	
	/** 
	 * @param size_t threadCount The number of threads to be used. By default - the maximum possible number at which parallelism will be maintained
	 */
	static void init(size_t threadCount = std::thread::hardware_concurrency());
	static ThreadManager* getInstance();
	static void free();
	
	/**
	 * Function to convert tasks to std::packaged_task 
	 * @return std::future<returnType> future to get task result
	 */
	template <typename Func, typename... Args>
	auto addTask (Func&& func, Args&&... args) -> std::future<std::invoke_result_t<Func, Args...>> {
        using returnType = std::invoke_result_t<Func, Args...>;
        auto task = std::make_shared<std::packaged_task<returnType()>>(
                std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

        this->addToQueue([task] () { (*task)(); });
        return task->get_future();
	}
};

} // namespace sth