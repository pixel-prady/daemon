#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <chrono>
#include <functional>
#include <thread>
#include <atomic>
#include "core/CPUIdleMonitor.hpp"

class Scheduler {
    public: 
        Scheduler(double cpuIdleThreshold = 15.0, std::chrono::milliseconds interval = std::chrono::milliseconds(1000)); 
        ~Scheduler(); 

        void StartMonitoring ( std :: function<void ()>task); 

        void StopMonitoring () ; 

        // void StopTask() ; 

    private : 
        std :: thread monitoringThread ;
        std :: atomic <bool> isMonitoring; 
        bool taskRunning ; 

        CPUIdleMonitor cpuIdleMonitor ;
        std :: chrono::milliseconds interval ;

        void TaskLoop(std :: function <void() > task);
        friend class SchedulerTest ;
};

#endif 