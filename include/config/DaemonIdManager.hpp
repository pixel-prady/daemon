#ifndef IDMANAGER_HPP
#define IDMANAGER_HPP

#include <string>

class DaemonIdManager
{
public:
    explicit DaemonIdManager(const std::string &filePath = "config/daemon_id.txt");
    std::string getDaemonId();

private:
    std::string filePath;

    std::string generateRandomId();

    std::string readIdFromFile();

    bool writeIdToFile(const std::string &id);
};

#endif