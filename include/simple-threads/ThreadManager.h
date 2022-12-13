/**
 * This file is the manager of all threads, which follows from the name of the class.
 * Just include it in to take full advantage of multithreading!
 * 
 * @author N3WC0DER
 */

#pragma once
#include <type_traits>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <queue>
#include <stdexcept>

namespace sth {

enum class Priority : int {
	LOWEST,
	MEDIUM,
	HIGHEST,
};

/** Pattern: Singleton */
class ThreadManager {
private:
	/** The Task class contains std::function and execution priority. Required for comparing priority in std::priority_queue */
	struct Task {
		std::function<void()> func;
		Priority priority;

		/* for std::priority_queue */
		friend bool operator<(const ThreadManager::Task& l, const ThreadManager::Task& r) {
			return l.priority < r.priority;
		}
	};

	/** Maximum number of possible active threads (may increase this number in the future) */
	static constexpr size_t MAXIMUM_THREAD_COUNT = 64;

	/** 
	 * The constructor is needed to initialize threads 
	 * @param size_t threadCount number of threads to be created
	 * @throw std::invalid_argument
	 */
	explicit ThreadManager(size_t threadCount);
	// void operator delete(void*);
	/* PLEASE DON'T USE OPERATOR DELETE!! */

	/** Flag that specifies the state of the ThreadManager */
	std::atomic<bool> enabled;

	/** The queue from which threads take tasks */
	std::priority_queue<Task> taskPool;
	/** This mutex synchronizes the task queue */
	std::mutex mutex_pool;
	/** condition_variable makes threads wait for a new task to be added if the queue is empty */
	std::condition_variable cv_pool;

	/** Array of threads that execute tasks */
	std::vector<std::thread> threadPool;

	/** Number of active threads */
	std::atomic<size_t> threadCount = 0;

	/** Number of threads waiting for tasks */
	std::atomic<size_t> threadReady = 0;

	static std::unique_ptr<ThreadManager> instance;

	/** Adding a function task to the queue */
	void addToQueue(const ThreadManager::Task& task);

	/** A function that threads perform to execute tasks from a queue */
	void run();

	/** Pulling a task from the queue */
	ThreadManager::Task getTask();
	
public:
	ThreadManager(const ThreadManager&) = delete;
	ThreadManager& operator=(const ThreadManager&) = delete;
	/** Destructor where threads end and are freed */
	~ThreadManager();

	/** 
	 * @param size_t threadCount The number of threads to be used. 
	 * By default - the maximum possible number at which parallelism will be maintained.
	 */
	static void init(size_t threadCount = std::thread::hardware_concurrency());
	/**
	 * @throw std::logic_error
	 */
	static ThreadManager* getInstance();
	/**
	 * @throw std::logic_error
	 */
	static void release();

	static bool isInit() noexcept;

	/** Blocking wait for all threads to finish */
	void waitAll();

	/** Clearing the queue of tasks */
	void clearQueue();

	/**
	 * Function to convert tasks to std::packaged_task 
	 * @return std::future<return_type_t> future to get task result
	 */
	template<typename Func, typename... Args>
	auto addTask(Priority priority, Func&& func, Args&&... args) -> std::future<std::invoke_result_t<Func, Args...>> {
		using return_type_t = std::invoke_result_t<Func, Args...>;
		auto task = std::make_shared<std::packaged_task<return_type_t()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

		this->addToQueue(Task {[task]() {(*task)();}, priority});
		return task->get_future();
	}
};

}  // namespace sth
