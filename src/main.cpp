#include "FileSegregator.hpp"
#include "ConfigParser.hpp"
#include "FileWatcher.hpp"
#include "Scheduler.hpp"
#include "Logger.hpp"
#include "DaemonIdManager.hpp"
#include "LocalBackendServer.hpp"

#include <iostream>
#include <set>
#include <mutex>
#include <atomic>

std::set<std::string> changedFiles;
std::mutex fileMutex;
std::atomic<bool> checkExistingFile{true};

void addChangedFile(const std::string &filePath)
{
    std::lock_guard<std::mutex> lock(fileMutex);
    changedFiles.insert(filePath);
}

void segregateFilesIfIdle(FileSegregator &fileSegregator)
{
    std::lock_guard<std::mutex> lock(fileMutex);
    for (const auto &filePath : changedFiles)
    {
        fileSegregator.SegregateFile(filePath);
    }
    changedFiles.clear();
}

void runAutoSortDaemon()
{
    Logger logger;

    DaemonIdManager daemonIdManager("config/daemon_id.txt");

    std::string daemonId = daemonIdManager.getDaemonId();
    logger.logInfo("Daemon ID: " + daemonId + "\n");

    LocalBackendServer server(daemonId);
    server.start();

    logger.setBackendServer(&server);

    ConfigParser config("config/settings.yaml", &logger);

    if (!config.loadConfig())
    {
        logger.logError("Failed to load configuration.");
        return;
    }

    auto folderConfig = config.loadFolderConfig();

    FileSegregator fileSegregator(folderConfig, &logger);

    std::string watchPath = config.getString("watch_path", "/home/zeus/Downloads");

    FileWatcher fileWatcher(watchPath, false, &logger);

    fileWatcher.StartWatching([&fileSegregator, &logger](const std::string &filePath, const std::string &event)
                              {
        logger.logInfo("File event detected: " + event + " - " + filePath);
        addChangedFile(filePath); });

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
        changedFiles.clear(); });

    std::cout << "AutoSort is running..." << std::endl;


    server.onCommand([&](const std::string& command, const std::string& data) {
        logger.logInfo("Received command: " + command + " with data: " + data);

        if (command == "update") {
            auto sep = data.find('=');
            if (sep != std::string::npos) {
                std::string key = data.substr(0, sep);
                std::string value = data.substr(sep + 1);

                if (config.updateConfigFile(key, value)) {
                    logger.logInfo("Config updated successfully for key: " + key);
                    server.broadcastMessage("SUCCESS: Config updated. Restarting daemon...");
                    std::exit(0);
                } else {
                    logger.logError("Failed to update config file for key: " + key);
                    server.broadcastMessage("ERROR: Failed to update config.");
                }
            } else {
                logger.logWarning("Malformed update command data: " + data);
                server.broadcastMessage("ERROR: Invalid update command format.");
            }
        }
        else if (command == "restart") {
            logger.logInfo("Restart command received.");
            server.broadcastMessage("INFO: Restarting daemon...");
            std::exit(0);
        }
        else {
            logger.logWarning("Unknown command received: " + command);
            server.broadcastMessage("ERROR: Unknown command.");
        }
    });

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

int main()
{
    try
    {
        runAutoSortDaemon();
    }
    catch (const std::exception &e)
    {
        Logger logger;
        logger.logError("Error: " + std::string(e.what()));
    }
}
