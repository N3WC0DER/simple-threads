#include "simple-threads/ThreadManager.h"

#include <gtest/gtest.h>

struct ThreadManagerTest : public ::testing::Test {
	sth::ThreadManager* tmanager = nullptr;

	void SetUp() override {
		if (!sth::ThreadManager::is_init())
			sth::ThreadManager::init(2);
		this->tmanager = sth::ThreadManager::get_instance();
	}

	void TearDown() override {
		this->tmanager->clear_queue();
		this->tmanager->wait_all();
		sth::ThreadManager::free();
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

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
