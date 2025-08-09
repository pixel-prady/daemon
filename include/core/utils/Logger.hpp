#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string> 
class Logger { 

    public : 

        virtual void logInfo(const std :: string & message ) ; 
        virtual void logWarning ( const std :: string & message  ) ; 
        virtual void logError( const std :: string & message ) ; 
        
        virtual ~Logger() = default; 
} ; 

#endif 
