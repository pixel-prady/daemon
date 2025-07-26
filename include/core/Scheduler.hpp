#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <chrono>
#include <functional>
#include <thread>
#include <atomic>

class Scheduler {
    public: 
        Scheduler(); 
        ~Scheduler(); 

        void StartMonitoring ( std :: function<void ()>task, std :: chrono :: milliseconds interval); 

        void StopMonitoring () ; 

        void StopTask() ; 

    private : 
        std :: thread monitoringThread ;
        std :: atomic <bool> isMonitoring; 
        bool taskRunning ; 

        void TaskLoop(std :: function <void() > task , std :: chrono:: milliseconds interval);
};

#endif 