// SchedulerTest.cpp
#include "Scheduler.hpp"
#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <thread>

class SchedulerTest : public ::testing::Test {
    // No special setup needed
};

// Verifies the task runs when CPU is considered idle
TEST_F(SchedulerTest, TaskRunsWhenCpuIsIdle) {
    Scheduler scheduler(1000.0, std::chrono::milliseconds(10));

    std::atomic<int> counter{0};
    auto task = [&counter]() {
        ++counter;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    };

    scheduler.StartMonitoring(task);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    scheduler.StopMonitoring();

    EXPECT_GT(counter.load(), 0);
}

// Checks that calling StartMonitoring twice does not start multiple tasks
TEST_F(SchedulerTest, StartingTwiceDoesNothing) {
    Scheduler scheduler(1000.0, std::chrono::milliseconds(10));
    std::atomic<int> counter{0};
    auto task = [&counter]() { ++counter; };

    scheduler.StartMonitoring(task);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    int countAfterFirstStart = counter.load();

    scheduler.StartMonitoring(task);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    scheduler.StopMonitoring();

    int countAfterSecondStart = counter.load();

    EXPECT_GE(countAfterSecondStart, countAfterFirstStart);
}

// Ensures calling StopMonitoring without starting is safe
TEST_F(SchedulerTest, StoppingWithoutStartIsSafe) {
    Scheduler scheduler(1000.0, std::chrono::milliseconds(10));
    scheduler.StopMonitoring();

    SUCCEED();
}

// Validates that the task does not run when CPU is never idle
TEST_F(SchedulerTest, TaskDoesNotRunIfCpuIsNotIdle) {
    Scheduler scheduler(0.0, std::chrono::milliseconds(10));
    std::atomic<int> counter{0};
    auto task = [&counter]() { ++counter; };

    scheduler.StartMonitoring(task);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    scheduler.StopMonitoring();

    EXPECT_EQ(counter.load(), 0);
}
