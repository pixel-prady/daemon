#include "../googletest/googletest/include/gtest/gtest.h"
#include "../googletest/googlemock/include/gmock/gmock.h"
#include "core/CPUIdleMonitor.hpp"

class MockCPUIdleMonitor : public CPUIdleMonitor {
public:
    MockCPUIdleMonitor(double idleThreshold, int intervalMs)
        : CPUIdleMonitor(idleThreshold, intervalMs) {}

    double getIdleThreshold() const {
        return IdleThreshold;
    }

    void setIdleThreshold(double threshold) {
        IdleThreshold = threshold;
    }

    int getIntervalMs() const {
        return IntervalMs;
    }

    void setIntervalMs(int interval) {
        IntervalMs = interval;
    }

    MOCK_METHOD((std::pair<unsigned long long, unsigned long long>), readCPUStats, (), (override));
};

// Constructor test
TEST(CPUIdleMonitorTest, ConstructorWorks) {
    MockCPUIdleMonitor monitor(10.0, 1000);
    EXPECT_DOUBLE_EQ(monitor.getIdleThreshold(), 10.0);
    EXPECT_EQ(monitor.getIntervalMs(), 1000);
}

// Test for readCPUStats with mocked behavior
TEST(CPUIdleMonitorTest, ReadCPUStatsReturnsCorrectValues) {
    MockCPUIdleMonitor monitor(10.0, 1000);
    EXPECT_CALL(monitor, readCPUStats())
        .Times(2)
        .WillOnce(testing::Return(std::make_pair(2000ULL, 10000ULL)))
        .WillOnce(testing::Return(std::make_pair(3000ULL, 12000ULL)));

    auto [idle1, total1] = monitor.readCPUStats();
    auto [idle2, total2] = monitor.readCPUStats();

    EXPECT_EQ(idle1, 2000ULL);
    EXPECT_EQ(total1, 10000ULL);
    EXPECT_EQ(idle2, 3000ULL);
    EXPECT_EQ(total2, 12000ULL);
}

// Test for setter methods
TEST(CPUIdleMonitorTest, SetterUpdatesValuesCorrectly) {
    MockCPUIdleMonitor monitor(10.0, 1000);
    monitor.setIdleThreshold(15.0);
    monitor.setIntervalMs(2000);
    EXPECT_DOUBLE_EQ(monitor.getIdleThreshold(), 15.0);
    EXPECT_EQ(monitor.getIntervalMs(), 2000);
}

// Test for CPU idle detection (idle CPU)
TEST(CPUIdleMonitorTest, IsCPUIdleReturnsTrueWhenCPUIdle) {
    MockCPUIdleMonitor monitor(60.0, 1000);
    EXPECT_CALL(monitor, readCPUStats())
        .Times(2)
        .WillOnce(testing::Return(std::make_pair(8000ULL, 10000ULL)))
        .WillOnce(testing::Return(std::make_pair(9000ULL, 12000ULL)));

    bool result = monitor.isCPUIdle();
    EXPECT_TRUE(result);
}

// Test for CPU idle detection (not idle CPU)
TEST(CPUIdleMonitorTest, IsCPUIdleReturnsFalseWhenCPUIsNotIdle) {
    MockCPUIdleMonitor monitor(20.0, 1000);
    EXPECT_CALL(monitor, readCPUStats())
        .Times(2)
        .WillOnce(testing::Return(std::make_pair(1000ULL, 10000ULL)))
        .WillOnce(testing::Return(std::make_pair(1500ULL, 12000ULL)));

    bool result = monitor.isCPUIdle();
    EXPECT_FALSE(result);
}

// Test to handle zero division scenario
TEST(CPUIdleMonitorTest, IsCPUIdleHandlesZeroDivision) {
    MockCPUIdleMonitor monitor(20.0, 1000);
    EXPECT_CALL(monitor, readCPUStats())
        .Times(2)
        .WillOnce(testing::Return(std::make_pair(2000ULL, 10000ULL)))
        .WillOnce(testing::Return(std::make_pair(2000ULL, 10000ULL)));

    bool result = monitor.isCPUIdle();
    EXPECT_FALSE(result);
}
