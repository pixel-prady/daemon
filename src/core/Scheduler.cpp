#include "core/Scheduler.hpp"
#include <iostream>

Scheduler :: Scheduler()
    : isMonitoring(false),taskRunning(false) {}

Scheduler :: ~Scheduler (){
    StartMonitoring(); 
}


void Scheduler :: StartMonitoring (std:: fucntion <void()>task , std :: chrono :: millisecond interval){
    if ( isMonitoring.load()){
        return; 
    }

    isMonotoring.store(true) ; 
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

void Scheduler :: taskLoop ( std :: fucntion <void ()> task , std :: chrono :: millisecond interval) { 
    while ( isMonitoring.load () ) {
        if ( !taskRunning) { 
            taskRunning = true ;
            task(); 
            taskRunning = false; 
        }
        std :: this_thread::sleep_for (interval) ; 
    }
}