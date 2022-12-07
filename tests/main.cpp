#include "simple-threads/ThreadManager.h"

#include <gtest/gtest.h>
#include <exception>
#include <stdexcept>

struct ThreadManagerTest : public ::testing::Test {
	sth::ThreadManager* tmanager = nullptr;

	void SetUp() override {
		if (!sth::ThreadManager::is_init())
			sth::ThreadManager::init(2);
		this->tmanager = sth::ThreadManager::get_instance();
	}

	void TearDown() override {
		if (tmanager) {
			this->tmanager->clear_queue();
			this->tmanager->wait_all();
			if (sth::ThreadManager::is_init())
				sth::ThreadManager::release();
		}
	}
};

int add(int a, int b) {
	return a + b;
}

TEST_F(ThreadManagerTest, add_task) {
	int a = 5, b = 8;
	auto future = tmanager->add_task(sth::Priority::HIGHEST, &add, a, b);
	EXPECT_EQ(future.get(), a + b);
}

TEST_F(ThreadManagerTest, test_exception) {
	auto l = []() {
		throw std::exception();
	};

	auto f = tmanager->add_task(sth::Priority::HIGHEST, l);

	try {
		f.get();
		FAIL() << "The exception failed";
	} catch (const std::exception& ex) {
		SUCCEED();
	}
}

TEST(ThreadManagerTestExceptions, test_exception1) {
	try {
		sth::ThreadManager::get_instance();
		FAIL() << "The exception failed";
	} catch (const std::logic_error& ex) {
		SUCCEED();
	}
}

/* WARNING: THIS CODE CALLED SEGFAULT
 * but outside google test it behaves normally (wtf??)
TEST(ThreadManagerTestExceptions, test_exception2) {
	try {
		if(!sth::ThreadManager::is_init()) sth::ThreadManager::init(256);
		FAIL() << "The exception failed";
	} catch (const std::invalid_argument& ex) {
		if (sth::ThreadManager::is_init()) sth::ThreadManager::release();
		SUCCEED();
	}
}*/

TEST(ThreadManagerTestExceptions, test_exception3) {
	try {
		sth::ThreadManager::init(2);
		sth::ThreadManager::init(3);
		FAIL() << "The exception failed";
	} catch (const std::logic_error& ex) {
		sth::ThreadManager::release();
		SUCCEED();
	}
}

TEST(ThreadManagerTestExceptions, test_exception4) {
	try {
		sth::ThreadManager::release();
		FAIL() << "The exception failed";
	} catch (const std::logic_error& ex) {
		SUCCEED();
	}
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
