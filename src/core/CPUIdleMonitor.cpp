#include "core/CPUIdleMonitor.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <utility>

CPUIdleMonitor ::CPUIdleMonitor(double idleThreshold, int intervalMs)
    : IdleThreshold(idleThreshold), IntervalMs(intervalMs) {}

std ::pair<unsigned long long, unsigned long long> CPUIdleMonitor::readCPUStats()
{
    std ::ifstream statFile("/proc/stat");

    if (!statFile.is_open())
    {
        throw std::runtime_error("Failed to open /proc/stat");
    }

    std ::string line;

    std ::getline(statFile, line);
    std ::istringstream ss(line);

    std ::string cpu;

    statFile.close();

    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;

    ss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    unsigned long long idleTime = idle + iowait;
    unsigned long long totalTime = user + nice + system + idle + iowait + irq + softirq + steal;

    return {idleTime, totalTime};
}

bool CPUIdleMonitor::isCPUIdle()
{
    auto [idleTime1, totalTime1] = readCPUStats(); // taking first snap of cpu readings
    std ::this_thread ::sleep_for(std ::chrono ::milliseconds(IntervalMs));

    auto [idleTime2, totalTime2] = readCPUStats(); // taking second snap of cpu readings ;

    unsigned long long idleTimeDiff = idleTime2 - idleTime1;
    unsigned long long totalTimeDiff = totalTime2 - totalTime1;


    if ( totalTimeDiff  == 0 ) return false;  

    double cpuUsagePercentage = ( 1.0-(double) idleTimeDiff / totalTimeDiff) * 100 ; 

    return cpuUsagePercentage < IdleThreshold; 
}