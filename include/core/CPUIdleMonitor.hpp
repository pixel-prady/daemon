# ifndef CPU_IDLE_MONITOR_HPP
# define CPU_IDLE_MONITOR_HPP

# include <chrono>
# include <thread>

class CPUIdleMonitor{
    public : 
        explicit CPUIdleMonitor (double idleThreshold  = 15.0, int intervalMs = 1000); 

        bool isCPUIdle () ; 

    private : 
        double IdleThreshold ;
        int IntervalMs ; 

        std :: pair <unsigned long long  , unsigned long long > readCPUStats() ; 

};

# endif 