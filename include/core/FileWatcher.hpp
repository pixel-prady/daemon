//
// Created by Pradhuman on 7/24/25.
//

#ifndef FILEWATCHER_HPP
#define FILEWATCHER_HPP

#include <string>
#include <functional>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <sys/inotify.h>
#include "Logger.hpp"

class FileWatcher
{
public:
    explicit FileWatcher(const std::string &watchDir, bool recursive,Logger*logger);

    ~FileWatcher(); // to destruct the thread and the inotify instance, created

    void StartWatching(std::function<void(const std::string &, const std::string &)> OnFileChange);

    void StopWatching();

private:
    std ::string watchDir;
    bool recursive;
    int inotifyFd;
    std ::atomic<bool> watching;
    std ::thread watchThread;
    std ::unordered_map<int, std ::string> wdToPath;

    void watchLoop(std ::function<void(const std ::string &, const std ::string &)> callback);
    void addWatchRecursively(const std ::string &path);
    Logger * logger ;
};
#endif