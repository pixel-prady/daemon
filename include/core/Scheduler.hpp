#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <chrono>
#include <fucntional>
#include <thread>
#include <atomic>


class Scheduler {
    public: 
        Scheduler(); 
        ~Scheduler(); 

        void StartMonitoring ( std :: fucntion<void ()>task, std :: chrono :: millisecond interval); 

        void StopMonitoring () ; 

        void StopTask() ; 

    private : 
        std :: thread monitoringThread ;
        std :: atomic <bool> isMonitoring; 
        bool isRunning ; 

        void TaskLoop(std :: function <void() > task , std :: chrono:: milliseconds interval);
};

#endif 