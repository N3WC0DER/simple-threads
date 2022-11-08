#include "ThreadManager.h"
#include <iostream>

using namespace sth;

std::unique_ptr<ThreadManager> ThreadManager::instance = nullptr;

ThreadManager::ThreadManager(size_t thread_count) {
	if (thread_count <= 0 || thread_count > MAXIMUM_THREAD_COUNT)
			throw std::out_of_range("The number of threads out of range");
	
	this->enabled.store(true);
	
	// Creating and launch all threads
	this->thread_count.store(thread_count);

	for (size_t i = 0; i < thread_count; i++) {
		auto temp = std::thread(&ThreadManager::run, this);
		this->thread_pool.push_back( std::move(temp));
	}
	
	printf("ThreadManager started\n");
}

ThreadManager::~ThreadManager() {
	this->enabled.store(false);

	this->clear_queue();

	while (true) {
		if (this->task_pool.size() == 0) break;
	}
	
	// Notify all threads and wait end of their work
	for (auto& thread : this->thread_pool) {
		this->cv_pool.notify_all();
		thread.join();
	}
	printf("ThreadManager stopped\n");
}

void ThreadManager::init(size_t thread_count) {
	if (instance != nullptr)
			throw std::runtime_error("Error initialize ThreadManager");
	
	instance.reset(new ThreadManager(thread_count));
}

ThreadManager* ThreadManager::get_instance() {
	if (instance == nullptr)
			throw std::runtime_error("ThreadManager not initialize");
	
	return instance.get();
}

void ThreadManager::free() {
	instance.reset();
}

void ThreadManager::wait_all() {
	while (!this->task_pool.empty() && this->thread_ready.load() != this->thread_count.load()) {
		std::this_thread::yield();
	}
}

void ThreadManager::add_to_queue(Task& task) {
	std::scoped_lock<std::mutex> lock(this->mutex_pool);
	this->task_pool.push(std::move(task));

	// Notify only one thread to start executing a task
	this->cv_pool.notify_one();
}

void ThreadManager::run() {
	while (this->enabled.load()) {
		std::unique_lock<std::mutex> lock(this->mutex_pool);
		
		this->thread_ready.fetch_add(1);

		// Waiting for notification
		this->cv_pool.wait(lock, [this] () -> bool { return !this->task_pool.empty() || !this->enabled.load(); });
		
		this->thread_ready.fetch_sub(1);

		if (this->task_pool.empty())
				continue;
		
		auto task = std::move(this->task_pool.top());
		this->task_pool.pop();
		lock.unlock();
		
		task.func();
	}
}

Task ThreadManager::get_task() {
	std::unique_lock<std::mutex> lock(this->mutex_pool);
		
	// Waiting for notification
	if (this->task_pool.empty())
		this->cv_pool.wait(lock, [this] () -> bool { return !this->task_pool.empty() || !this->enabled.load(); });
	
	auto task = std::move(this->task_pool.top());
	this->task_pool.pop();
	lock.unlock();
	return task;
}

void ThreadManager::clear_queue() {
	std::queue<Task> temp_pool;

	this->mutex_pool.lock();
	this->task_pool = std::priority_queue<Task> ();
	this->mutex_pool.unlock();
}
