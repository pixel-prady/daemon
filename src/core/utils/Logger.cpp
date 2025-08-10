#include "Logger.hpp"
#include <iostream>

void Logger::logInfo(const std::string &message)
{
    std::string fullMsg = "[INFO] " + message + "\n";
    std::cout << fullMsg << std::endl;
    if (backendServer) {
        backendServer->broadcastMessage(fullMsg);
    }
}

void Logger::logError(const std::string &message)
{
    std::string fullMsg = "[ERROR] " + message +  "\n";
    std::cerr << fullMsg << std::endl;
    if (backendServer) {
        backendServer->broadcastMessage(fullMsg);
    }
}

void Logger::logWarning(const std::string &message)
{
    std::string fullMsg = "[WARNING] " + message = "\n";
    std::cout << fullMsg << std::endl;
    if (backendServer) {
        backendServer->broadcastMessage(fullMsg);
    }
}