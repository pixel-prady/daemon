#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include "LocalBackendServer.hpp"

class Logger
{

public:

    virtual void logInfo(const std::string &message);
    virtual void logError(const std::string &message);
    virtual void logWarning(const std::string &message);

    void setBackendServer(LocalBackendServer *server)
    {
        backendServer = server;
    }

private:
    LocalBackendServer *backendServer = nullptr;
};

#endif
