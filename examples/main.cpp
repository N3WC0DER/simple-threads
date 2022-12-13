/**
 * This file contains examples of using ThreadManager.
 * They use most of the variants of functions, lambda functions and functors.
 * 
 * @author N3WC0DER
 */

#include "simple-threads/ThreadManager.h"

#include <chrono>
#include <cstdio>
#include <exception>
#include <iostream>
#include <mutex>

using namespace sth;

std::mutex mutex;

void voidFuncWithoutParam() {
	std::scoped_lock<std::mutex> lock(mutex);
	std::cout << "voidFuncWithoutParam()" << std::endl;
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
	if (!ThreadManager::isInit())
		ThreadManager::init(4);

	ThreadManager* tmanager = ThreadManager::getInstance();
	
	// Void function without parameters
	tmanager->addTask(Priority::HIGHEST, &voidFuncWithoutParam);

	// Lambda without parameters
	tmanager->addTask(Priority::HIGHEST, [] {
		std::scoped_lock<std::mutex> lock(mutex);
		std::cout << "lambda function" << std::endl;
	});

	// A function with two parameters that returns their sum
	auto future = tmanager->addTask(Priority::HIGHEST, &add, 5, 10);
	std::cout << future.get() << std::endl;

	// A lambda with two parameters that return their sum in the third parameter
	int res;
	auto future2 = tmanager->addTask(
			Priority::HIGHEST, [](int a, int b, int& result) {
				result = a + b;
			},
			10, 37, std::ref(res));

	// Can be used to wait for a task to complete
	future2.get();
	std::cout << "10 + 37 = " << res << std::endl;

	// Unary functor
	A a;
	tmanager->addTask(Priority::HIGHEST, a, "Functor");

	// Unary functor and member function
	B b;
	tmanager->addTask(Priority::HIGHEST, &B::print, &b);  // Do not forget that a non-static member function
														   // takes as its first argument a reference to the object from which it is called.

	auto future3 = tmanager->addTask(Priority::HIGHEST, b, 3);

	std::cout << future3.get() << std::endl;

	tmanager->addTask(Priority::HIGHEST, &B::print, &b);

	auto l = []() {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	};

	// The following examples are not always executed in the order in which they were intended.
	// Use synchronization tools to organize the correct order.

	tmanager->addTask(Priority::HIGHEST, l);
	tmanager->addTask(Priority::HIGHEST, l);
	tmanager->addTask(Priority::HIGHEST, l);
	tmanager->addTask(Priority::HIGHEST, l);

	tmanager->addTask(Priority::MEDIUM, []() {
		std::scoped_lock<std::mutex> lock(mutex);
		std::cout << "Second output" << std::endl;
	});

	tmanager->addTask(Priority::HIGHEST, []() {
		std::scoped_lock<std::mutex> lock(mutex);
		std::cout << "First output" << std::endl;
	});

	// Waiting completing tasks and shutdown ThreadManager
	// (This member is called in the destructor, so it won't have any effect
	tmanager->waitAll();

	// In fact, the following code is completely unnecessary,
	// since, after the completion of the main () function,
	// the ThreadManager destructor is automatically called
	if (ThreadManager::isInit())
		ThreadManager::release();

	return 0;
}
