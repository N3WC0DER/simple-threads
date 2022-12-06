/**
 * This file contains examples of using ThreadManager.
 * They use most of the variants of functions, lambda functions and functors.
 * 
 * @author N3WC0DER
 */

#include "simple-threads/ThreadManager.h"

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

using namespace sth;

std::mutex mutex;

void void_func_without_param() {
	std::scoped_lock<std::mutex> lock(mutex);
	std::cout << "void_func_without_param()" << std::endl;
}

int add(int a, int b) {
	return a + b;
}

struct A {
	void operator()(const std::string& str) {
		std::scoped_lock<std::mutex> lock(mutex);
		std::cout << str << std::endl;
	}
};

class B {
private:
	int b[10] {};

public:
	B() {
		for (int& i: b)
			i = rand() % 100;
	}

	int& operator()(const int i) {
		std::scoped_lock<std::mutex> lock(mutex);
		return b[i];
	}

	void print() const {
		std::scoped_lock<std::mutex> lock(mutex);
		for (const int i: b)
			std::cout << i << " ";
		std::cout << std::endl;
	}
};

int main() {
	if (!ThreadManager::is_init())
		ThreadManager::init(4);

	ThreadManager* tmanager = ThreadManager::get_instance();

	// Void function without parameters
	tmanager->add_task(Priority::HIGHEST, &void_func_without_param);

	// Lambda without parameters
	tmanager->add_task(Priority::HIGHEST, [] {
		std::scoped_lock<std::mutex> lock(mutex);
		std::cout << "lambda function" << std::endl;
	});

	// A function with two parameters that returns their sum
	auto future = tmanager->add_task(Priority::HIGHEST, &add, 5, 10);
	std::cout << future.get() << std::endl;

	// A lambda with two parameters that return their sum in the third parameter
	int res;
	auto future2 = tmanager->add_task(
			Priority::HIGHEST, [](int a, int b, int& result) {
				result = a + b;
			},
			10, 37, std::ref(res));

	// Can be used to wait for a task to complete
	future2.get();
	std::cout << "10 + 37 = " << res << std::endl;

	// Unary functor
	A a;
	tmanager->add_task(Priority::HIGHEST, a, "Functor");

	// Unary functor and member function
	B b;
	tmanager->add_task(Priority::HIGHEST, &B::print, &b);  // Do not forget that a non-static member function
													  // takes as its first argument a reference to the object from which it is called.

	auto future3 = tmanager->add_task(Priority::HIGHEST, b, 3);

	std::cout << future3.get() << std::endl;

	tmanager->add_task(Priority::HIGHEST, &B::print, &b);

	auto l = []() {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	};

	// The following examples are not always executed in the order in which they were intended.
	// Use synchronization tools to organize the correct order.

	tmanager->add_task(Priority::HIGHEST, l);
	tmanager->add_task(Priority::HIGHEST, l);
	tmanager->add_task(Priority::HIGHEST, l);
	tmanager->add_task(Priority::HIGHEST, l);

	tmanager->add_task(Priority::MEDIUM, []() {
		std::scoped_lock<std::mutex> lock(mutex);
		std::cout << "Second output" << std::endl;
	});

	tmanager->add_task(Priority::HIGHEST, []() {
		std::scoped_lock<std::mutex> lock(mutex);
		std::cout << "First output" << std::endl;
	});
	
	// Waiting completing tasks and shutdown ThreadManager
	tmanager->wait_all();
	if (ThreadManager::is_init())
		ThreadManager::free();

	return 0;
}
