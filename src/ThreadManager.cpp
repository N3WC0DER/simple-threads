#include "simple-threads/ThreadManager.h"

namespace sth {

std::unique_ptr<ThreadManager> ThreadManager::instance = nullptr;

ThreadManager::ThreadManager(size_t threadCount) {
	if (threadCount <= 0 || threadCount > MAXIMUM_THREAD_COUNT)
		throw std::invalid_argument("The number of threads is invalid");

	this->enabled.store(true);

	// Creating and launch all threads
	this->threadCount.store(threadCount);
	for (size_t i = 0; i < threadCount; i++) {
		this->threadPool.emplace_back(&ThreadManager::run, this);
	}
}

ThreadManager::~ThreadManager() {
	this->enabled.store(false);
	
	this->waitAll();
	this->clearQueue();

	// Notify all threads and wait end of their work
	for (auto& thread: this->threadPool) {
		this->cv_pool.notify_all();
		thread.join();
	}
}

void ThreadManager::addToQueue(const ThreadManager::Task& task) {
	std::scoped_lock<std::mutex> lock(this->mutex_pool);
	this->taskPool.push(task);

	// Notify only one thread to start executing a task
	this->cv_pool.notify_one();
}

void ThreadManager::run() {
	while (this->enabled.load()) {
		std::unique_lock<std::mutex> lock(this->mutex_pool);

		this->threadReady.fetch_add(1);

		// Waiting for notification
		this->cv_pool.wait(lock, [this]() -> bool {
			return !this->taskPool.empty() || !this->enabled.load();
		});

		this->threadReady.fetch_sub(1);

		if (this->taskPool.empty())
			continue;

		auto task = this->getTask();
		lock.unlock();

		task.func();
	}
}

ThreadManager::Task ThreadManager::getTask() {
	auto task = this->taskPool.top();
	this->taskPool.pop();

	return task;
}

void ThreadManager::init(size_t threadCount) {
	if (instance)
		throw std::logic_error("Error initialize ThreadManager: instance is init");

	instance.reset(new ThreadManager(threadCount));
}

ThreadManager* ThreadManager::getInstance() {
	if (!instance)
		throw std::logic_error("Error getting instance ThreadManager: instance is not init");

	return instance.get();
}

void ThreadManager::release() {
	if (!instance)
		throw std::logic_error("Instance release error: nothing to free");
	instance.reset();
}

bool ThreadManager::isInit() noexcept {
	return instance ? true : false;
}

void ThreadManager::waitAll() {
	while (!this->taskPool.empty() || this->threadReady.load() != this->threadCount.load()) {
		std::this_thread::yield();
	}
}

void ThreadManager::clearQueue() {
	this->mutex_pool.lock();
	this->taskPool = std::priority_queue<Task>();
	this->mutex_pool.unlock();
}

} // namespace sth
