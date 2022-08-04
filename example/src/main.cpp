/**
 * This file contains examples of using ThreadManager.
 * They use most of the variants of functions, lambda functions and functors.
 */

#include <iostream>
#include <unistd.h>

#include "ThreadManager.h"

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
	void operator()(const std::string str) {
		std::scoped_lock<std::mutex> lock(mutex);
		std::cout << str << std::endl;
	}
};

class B {
private:
	int b[10];
public:
	B() {
		for (int i = 0; i < 10; i++)
				b[i] = rand() % 100;
	}
	
	int& operator()(const int i) {
		return b[i];
	}
	
	void print() const {
		std::scoped_lock<std::mutex> lock(mutex);
		for (int i = 0; i < 10; i++)
				std::cout << b[i] << " ";
		std::cout << std::endl;
	}
};

int main() {
	ThreadManager::init();
	ThreadManager* thm = ThreadManager::getInstance();
	
	// Void function without parameters
	thm->addTask(&voidFuncWithoutParam);
	
	// Lambda without parameters
	thm->addTask([] {
		std::scoped_lock<std::mutex> lock(mutex);
		std::cout << "lambda function" << std::endl;
	});
	
	// A function with two parameters that returns their sum
	auto future = thm->addTask(&add, 5, 10);
	std::cout << future.get() << std::endl;
	
	// A lambda with two parameters that return their sum in the third parameter
	int res;
	auto future2 = thm->addTask([] (int a, int b, int& result) {
		result = a + b;
	}, 10, 37, std::ref(res));
	
	// Can be used to wait for a task to complete
	future2.get();
	std::cout << "10 + 37 = " << res << std::endl;
	
	// Unary functor
	A a;
	thm->addTask(a, "Functor");
	
	// Unary functor and member function
	B b;
	thm->addTask(&B::print, &b); // Do not forget that a non-static member function takes as its first argument a reference to the object from which it is called.
	auto future3 = thm->addTask(b, 3);
	std::cout << future3.get() << std::endl;
	thm->addTask(&B::print, &b);
	
	sleep(1); // To complete all tasks
	thm->free();
	std::cout << "Main" << std::endl;
	return 0;
}
