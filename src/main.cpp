#include "core/FileSegregator.hpp"
#include "config/ConfigParser.hpp"
#include "core/FileWatcher.hpp"
#include "core/Scheduler.hpp"
#include "core/utils/Logger.hpp"
#include <iostream>
#include <set>
#include <mutex>


std :: set <std :: string> changedFiles ;
std :: mutex filemutex ; 

void addChangedFile(const std :: string & filePath) { 
    std :: lock_guard<std :: mutex> lock(filemutex);
    changedFiles.insert(filePath);
}

void segregateFilesIfIdle(FileSegregator &fileSegregator)
{
    std :: lock_guard<std :: mutex> lock(filemutex);
    for (const auto &filePath : changedFiles)
    {
        fileSegregator.SegregateFile(filePath);
    }
    changedFiles.clear();
}

void runAutoSortDaemon()
{
    ConfigParser config("config/settings.yaml");

    if (!config.loadConfig())
    {
        Logger::logError("Failed to load configuration.");
        return;
    }
    auto folderConfig = config.loadFolderConfig();

    FileSegregator fileSegregator(folderConfig);

    std ::string watchPath = config.getString("watch_path", "/home/zeus/Downloads"); // todo : dynamic path

    FileWatcher fileWatcher(watchPath); // recursive watching disabled by default ( baadme test karunga ) ;

    fileWatcher.StartWatching([&fileSegregator](const std::string &filePath, const std::string &event)
        {
            Logger::logInfo("File event detected: " + event + " - " + filePath);
            addChangedFile(filePath);
        }
    );

    Scheduler scheduler(config.getDouble("cpu_idle_threshold", 15.0),std::chrono::milliseconds(config.getInt("monitoring_interval", 1000)));

    scheduler.StartMonitoring([&fileSegregator](){
        if (!changedFiles.empty()){
            std :: lock_guard<std :: mutex > lock (filemutex) ;

            for ( const auto &filePath : changedFiles){
                Logger::logInfo ( "Segregating file : " + filePath) ; 
                fileSegregator.SegregateFile(filePath) ; 
            }

            changedFiles.clear() ; 
        }
    }) ; 

    std :: cout << "AutoSort is running "<<std :: endl; 

    while ( true){
        std :: this_thread :: sleep_for(std :: chrono :: seconds(1)) ; 
    }
}

int main() { 
    try
    {
        runAutoSortDaemon() ;
    }
    catch(const std::exception& e)
    {
        Logger :: logError( "Error : " + std :: string(e.what())) ; 
    }
    
}