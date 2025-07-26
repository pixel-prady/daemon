#include "include/core/Scheduler.hpp"
#include <iostream>

Scheduler :: Scheduler()
    : isMonitoring(false),taskRunning(false) {}

Scheduler :: ~Scheduler (){
    StopMonitoring(); 
}


void Scheduler :: StartMonitoring (std:: function <void()>task , std :: chrono :: milliseconds interval){
    if ( isMonitoring.load()){
        return; 
    }

    isMonitoring.store(true) ; 
    monitoringThread = std :: thread( & Scheduler :: TaskLoop, this, task, interval) ; 
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

void Scheduler :: TaskLoop ( std :: function <void ()> task , std :: chrono :: milliseconds interval) { 
    while ( isMonitoring.load () ) {
        if ( !taskRunning) { 
            taskRunning = true ;
            task(); 
            taskRunning = false; 
        }
        std :: this_thread::sleep_for (interval) ; 
    }
}