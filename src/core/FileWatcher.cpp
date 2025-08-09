#include "Logger.hpp"
#include "FileWatcher.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <dirent.h>
#include <vector>
#include <iostream>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUFFER_LEN (1024 * (EVENT_SIZE + 16))

FileWatcher::FileWatcher(const std::string &dir, bool recursive, Logger *loggerInstance)
    : watchDir(dir), recursive(recursive), inotifyFd(-1), watching(false), logger(loggerInstance) {}

FileWatcher::~FileWatcher()
{
    StopWatching();
}

void FileWatcher::StartWatching(std::function<void(const std::string &, const std::string &)> onFileChange)
{
    if (watching.load())
        return;

    inotifyFd = inotify_init1(IN_NONBLOCK);
    if (inotifyFd < 0)
    {
        logger->logError("FAILED TO INITIALIZE INOTIFY");
        return;
    }

    addWatchRecursively(watchDir);
    watching.store(true);

    watchThread = std::thread(&FileWatcher::watchLoop, this, onFileChange);
    logger->logInfo("Started file watching on: " + watchDir);
}

void FileWatcher::StopWatching()
{
    if (!watching.load())
        return;
    watching.store(false);
    if (watchThread.joinable())
        watchThread.join();

    if (inotifyFd >= 0)
    {
        close(inotifyFd);
        logger->logInfo("STOPPED WATCHING FILE " + watchDir);
    }
}

void FileWatcher::watchLoop(std::function<void(const std::string &, const std::string &)> callback)
{
    char buffer[EVENT_BUFFER_LEN];

    while (watching.load())
    {
        int length = read(inotifyFd, buffer, EVENT_BUFFER_LEN);
        if (length < 0)
            continue;

        int i = 0;
        while (i < length)
        {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];

            std::string dirPath = wdToPath[event->wd];
            std::string fullPath = dirPath + "/" + std::string(event->name);

            if (event->mask & IN_CREATE)
            {
                logger->logInfo("File created: " + fullPath);
                if (event->mask & IN_ISDIR && recursive)
                {
                    addWatchRecursively(fullPath);
                }
                callback(fullPath, "created");
            }
            else if (event->mask & IN_MODIFY)
            {
                logger->logInfo("File modified: " + fullPath);
                callback(fullPath, "modified");
            }
            else if (event->mask & IN_DELETE)
            {
                logger->logWarning("File deleted: " + fullPath);
                callback(fullPath, "deleted");
            }

            i += EVENT_SIZE + event->len;
        }
    }
}

void FileWatcher::addWatchRecursively(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) != 0 || !S_ISDIR(st.st_mode))
    {
        logger->logError("Invalid directory: " + path);
        return;
    }

    int wd = inotify_add_watch(inotifyFd, path.c_str(), IN_CREATE | IN_MODIFY | IN_DELETE);
    if (wd < 0)
    {
        logger->logError("Failed to add inotify watch for: " + path);
        return;
    }
    wdToPath[wd] = path;
    logger->logInfo("Watching directory: " + path);

    if (!recursive)
        return;

    DIR *dir = opendir(path.c_str());
    if (!dir)
        return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        std::string name = entry->d_name;
        if (name == "." || name == "..")
            continue;

        std::string fullPath = path + "/" + name;
        if (entry->d_type == DT_DIR)
        {
            addWatchRecursively(fullPath);
        }
    }
    closedir(dir);
}
