#include "FileSegregator.hpp"
#include "Logger.hpp"

#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <algorithm>

FileSegregator::FileSegregator(const std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> &customFolders, Logger* loggerInstance)
    : categoryToFolder(customFolders), logger(loggerInstance)
{
    defaultFolders = {
        {"Images", "./Images"},
        {"Documents", "./Documents"},
        {"Archives", "./Archives"},
        {"Videos", "./Videos"},
        {"Audios", "./Audios"},
        {"PDFs", "./PDFs"},
        {"PPTs", "./PPTs"},
        {"Uncategorized", "./Uncategorized"},
    };
}

FileSegregator::~FileSegregator() {}

void FileSegregator::SegregateFile(const std::string &filePath)
{
    std::string category = GetFileCategory(filePath);

    if (category.empty())
    {
        category = "Uncategorized";
        logger->logWarning("No category found for file: " + filePath + ". Moving to Uncategorized.");
    }

    std::string destinationFolder = categoryToFolder.count(category) > 0 ? categoryToFolder[category].first : defaultFolders[category];
    MoveFileToCategory(filePath, destinationFolder);
}

void FileSegregator::SegregateExistingFiles(const std::string &dirPath, bool value)
{
    if (value)
    {
        try
        {
            for (const auto &file : std::filesystem::recursive_directory_iterator(dirPath))
            {
                if (file.is_regular_file())
                {
                    logger->logInfo("Processing existing file: " + file.path().string());
                    SegregateFile(file.path().string());
                }
            }
        }
        catch (const std::exception &e)
        {
            logger->logError("Error processing existing files: " + std::string(e.what()));
        }
    }
}

void FileSegregator::MoveFileToCategory(const std::string &filePath, const std::string &folderPath)
{
    CreateDirectoryIfNotExists(folderPath);

    std::filesystem::path destPath = folderPath / std::filesystem::path(filePath).filename();
    try
    {
        std::filesystem::rename(filePath, destPath);
        logger->logInfo("Moved file " + filePath + " to " + folderPath);
    }
    catch (const std::exception &e)
    {
        logger->logError("Failed to move file " + filePath + ": " + e.what());
    }
}

std::string FileSegregator::GetFileCategory(const std::string &filePath)
{
    std::string extension = std::filesystem::path(filePath).extension().string();

    for (const auto &category : categoryToFolder)
    {
        const auto &folderExtensions = category.second.second;

        if (std::find(folderExtensions.begin(), folderExtensions.end(), extension) != folderExtensions.end())
        {
            return category.first;
        }
    }

    return "";
}

void FileSegregator::CreateDirectoryIfNotExists(const std::string &dirPath)
{
    if (!std::filesystem::exists(dirPath))
    {
        try
        {
            std::filesystem::create_directory(dirPath);
            logger->logInfo("Created directory: " + dirPath);
        }
        catch (const std::exception &e)
        {
            logger->logError("Failed to create directory " + dirPath + ": " + e.what());
        }
    }
}

bool FileSegregator::IsDirectory(const std::string &path)
{
    return std::filesystem::is_directory(path);
}
