#include "FileSegregator.hpp"
#include "ConfigParser.hpp"
#include "FileWatcher.hpp"
#include "Scheduler.hpp"
#include "Logger.hpp"

#include <iostream>
#include <set>
#include <mutex>
#include <atomic>

std::set<std::string> changedFiles;
std::mutex fileMutex;
std::atomic<bool> checkExistingFile{true};

void addChangedFile(const std::string& filePath)
{
    std::lock_guard<std::mutex> lock(fileMutex);
    changedFiles.insert(filePath);
}

void segregateFilesIfIdle(FileSegregator& fileSegregator)
{
    std::lock_guard<std::mutex> lock(fileMutex);
    for (const auto& filePath : changedFiles)
    {
        fileSegregator.SegregateFile(filePath);
    }
    changedFiles.clear();
}

void runAutoSortDaemon()
{
    Logger logger;  

    ConfigParser config("config/settings.yaml");

    if (!config.loadConfig())
    {
        logger.logError("Failed to load configuration.");
        return;
    }

    auto folderConfig = config.loadFolderConfig();

    FileSegregator fileSegregator(folderConfig, &logger);

    std::string watchPath = config.getString("watch_path", "/home/zeus/Downloads");

    FileWatcher fileWatcher(watchPath, false, &logger);

    fileWatcher.StartWatching([&fileSegregator, &logger](const std::string& filePath, const std::string& event)
    {
        logger.logInfo("File event detected: " + event + " - " + filePath);
        addChangedFile(filePath);
    });

    Scheduler scheduler(config.getDouble("cpu_idle_threshold", 15.0), std::chrono::milliseconds(config.getInt("monitoring_interval", 1000)));

    scheduler.StartMonitoring([&fileSegregator, &watchPath, &config, &logger]()
    {
        if (checkExistingFile.load())
        {
            fileSegregator.SegregateExistingFiles(watchPath, config.getBool("segregate_existing_files", false));
            checkExistingFile.store(false);
        }

        std::lock_guard<std::mutex> lock(fileMutex);
        for (const auto& filePath : changedFiles)
        {
            logger.logInfo("Segregating file: " + filePath);
            fileSegregator.SegregateFile(filePath);
        }
        changedFiles.clear();
    });

    std::cout << "AutoSort is running..." << std::endl;

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main()
{
    try
    {
        runAutoSortDaemon();
    }
    catch (const std::exception& e)
    {
        Logger logger;
        logger.logError("Error: " + std::string(e.what()));
    }
}
