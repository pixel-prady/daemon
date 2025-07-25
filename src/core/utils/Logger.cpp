#include "../include/core/utils/Logger.hpp"

#include <iostream>

void Logger::logInfo(const std::string &message)
{
    std::cout << "[INFO] " << message << std::endl;
}

void Logger::logError(const std::string &message)
{
    std::cerr << "[ERROR] " << message << std::endl;
}

void Logger::logWarning(const std::string &message)
{
    std::cout << "[WARNING] " << message << std::endl;
}
