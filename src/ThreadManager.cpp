#include "simple-threads/ThreadManager.h"

namespace sth {

std::unique_ptr<ThreadManager> ThreadManager::instance = nullptr;

ThreadManager::ThreadManager(size_t thread_count) {
	if (thread_count <= 0 || thread_count > MAXIMUM_THREAD_COUNT)
		throw std::invalid_argument("The number of threads is invalid");

	this->enabled.store(true);

	// Creating and launch all threads
	this->thread_count.store(thread_count);
	for (size_t i = 0; i < thread_count; i++) {
		this->thread_pool.emplace_back(&ThreadManager::run, this);
	}
}

ThreadManager::~ThreadManager() {
	this->enabled.store(false);

	this->clear_queue();

	// Notify all threads and wait end of their work
	for (auto& thread: this->thread_pool) {
		this->cv_pool.notify_all();
		thread.join();
	}
}

void ThreadManager::add_to_queue(const ThreadManager::Task& task) {
	std::scoped_lock<std::mutex> lock(this->mutex_pool);
	this->task_pool.push(task);

	// Notify only one thread to start executing a task
	this->cv_pool.notify_one();
}

void ThreadManager::run() {
	while (this->enabled.load()) {
		std::unique_lock<std::mutex> lock(this->mutex_pool);

		this->thread_ready.fetch_add(1);

		// Waiting for notification
		this->cv_pool.wait(lock, [this]() -> bool {
			return !this->task_pool.empty() || !this->enabled.load();
		});

		this->thread_ready.fetch_sub(1);

		if (this->task_pool.empty())
			continue;

		auto task = this->get_task();
		lock.unlock();

		task.func();
	}
}

ThreadManager::Task ThreadManager::get_task() {
	auto task = this->task_pool.top();
	this->task_pool.pop();

	return task;
}

void ThreadManager::init(size_t thread_count) {
	if (instance)
		throw std::logic_error("Error initialize ThreadManager: instance is init");
	auto temp = new ThreadManager(thread_count);
	instance.reset(temp);
}

ThreadManager* ThreadManager::get_instance() {
	if (!instance)
		throw std::logic_error("Error getting instance ThreadManager: instance is not init");

	return instance.get();
} 

void ThreadManager::release() {
	if (!instance)
		throw std::logic_error("Instance release error: nothing to free");
	instance.reset();
}

bool ThreadManager::is_init() noexcept {
	return instance ? true : false;
}

void ThreadManager::wait_all() {
	while (!this->task_pool.empty() || this->thread_ready.load() != this->thread_count.load()) {
		std::this_thread::yield();
	}
}

void ThreadManager::clear_queue() {
	this->mutex_pool.lock();
	this->task_pool = std::priority_queue<Task>();
	this->mutex_pool.unlock();
}

} // namespace sth
