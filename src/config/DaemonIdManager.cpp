#include "DaemonIdManager.hpp"

#include <fstream>
#include <random>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <ctime>

namespace fs = std::filesystem;

DaemonIdManager::DaemonIdManager(const std::string &relativePath)
{
    fs::path cwd = fs::current_path();
    filePath = (cwd / relativePath).string();
}

std::string DaemonIdManager::getDaemonId()
{
    std::string id = readIdFromFile();
    if (id.empty())
    {
        id = generateRandomId();
        writeIdToFile(id);
    }
    return id;
}

std::string DaemonIdManager::readIdFromFile()
{
    std::ifstream inFile(filePath);
    if (!inFile.is_open())
        return "";

    std::string id;
    std::getline(inFile, id);
    inFile.close();
    return id;
}

bool DaemonIdManager::writeIdToFile(const std::string &id)
{
    fs::path dir = fs::path(filePath).parent_path();
    if (!fs::exists(dir))
    {
        fs::create_directories(dir);
    }

    std::ofstream outFile(filePath, std::ios::trunc);
    if (!outFile.is_open())
        return false;

    outFile << id;
    outFile.close();
    return true;
}

std::string DaemonIdManager::generateRandomId()
{
    std :: string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int length = 15; 
    std :: string id = "";

    srand(time(0)); 

    for (int i = 0; i < length; i++) {
        id += chars[rand() % chars.size()];
    }
    return id ; 
}
