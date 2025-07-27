#include "include/core/Scheduler.hpp"
#include <iostream>

Scheduler::Scheduler(double cpuIdleThreshold, std::chrono::milliseconds interval)
    : isMonitoring(false), taskRunning(false), cpuIdleMonitor(cpuIdleThreshold),interval(interval) {}

Scheduler :: ~Scheduler (){
    StopMonitoring(); 
}


void Scheduler :: StartMonitoring (std:: function <void()>task ){
    if ( isMonitoring.load()){
        return; 
    }

    isMonitoring.store(true) ; 
    monitoringThread = std :: thread( & Scheduler :: TaskLoop, this, task) ; 
}

void Scheduler :: StopMonitoring () { 
    if ( !isMonitoring.load()){ 
        return; 
    }

    isMonitoring.store (false) ; 

    if (monitoringThread.joinable () ){ 
        monitoringThread.join () ; 
    }
}

void Scheduler :: TaskLoop ( std :: function <void ()> task ) { 
    while ( isMonitoring.load () ) {
        if ( !taskRunning && cpuIdleMonitor.isCPUIdle()) { 
            taskRunning = true ;
            task(); 
            taskRunning = false; 
        }
        std :: this_thread::sleep_for (interval) ; 
    }
}