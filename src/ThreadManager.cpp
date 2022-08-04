#include "ThreadManager.h"

using namespace sth;

std::unique_ptr<ThreadManager> ThreadManager::instance = nullptr;

ThreadManager::ThreadManager(size_t threadCount) {
	if (threadCount <= 0 || threadCount > std::thread::hardware_concurrency())
			throw std::runtime_error("The number of threads out of range");
	
	this->enabled = true;
	
	// Creating and launch all threads
	this->threads = new std::thread*[threadCount];
	this->threadCount = threadCount;
	for (int i = 0; i < threadCount; i++) {
		this->threads[i] = new std::thread(&ThreadManager::run, this);
	}
	
	printf("ThreadManager started\n");
}

ThreadManager::~ThreadManager() {
	this->enabled = false;
	
	// Clearing the queue
	this->mutex_pool.lock();
	this->taskPool = std::queue<std::function<void ()>>();
	this->mutex_pool.unlock();
	
	// Notify all threads and terminate their work
	for (size_t i = 0; i < this->threadCount; i++) {
		this->cv_pool.notify_all();
		this->threads[i]->join();
		delete this->threads[i];
	}
	delete[] this->threads;
	printf("ThreadManager stopped\n");
}

void ThreadManager::init(size_t threadCount) {
	if (instance != nullptr)
			throw std::runtime_error("Error initialize ThreadManager");
	
	instance.reset(new ThreadManager(threadCount));
}

ThreadManager* ThreadManager::getInstance() {
	if (instance == nullptr)
			throw std::runtime_error("ThreadManager not initialize");
	
	return instance.get();
}

void ThreadManager::free() {
	instance.reset();
}

void ThreadManager::addToQueue(std::function<void ()> func) {
	std::scoped_lock<std::mutex> lock(this->mutex_pool);
	this->taskPool.push(func);
	
	// Notify only one thread to start executing a task
	this->cv_pool.notify_one();
}

void ThreadManager::run() {
	while (enabled) {
		std::unique_lock<std::mutex> lock(this->mutex_pool);
		
		// Waiting for notification
		this->cv_pool.wait(lock, [this] () -> bool { return !this->taskPool.empty() || !enabled; });
		
		if (this->taskPool.empty())
				continue;
		
		auto func = std::move(this->taskPool.front());
		this->taskPool.pop();
		lock.unlock();
		
		func();
	}
}