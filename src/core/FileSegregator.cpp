#include "core/FileSegregator.hpp"
#include "core/utils/Logger.hpp"

#include <filesystem>
#include <iostream>
#include <unordered_map>
#include "FileSegregator.hpp"

FileSegregator::FileSegregator(const std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> &customFolders)
    : categoryToFolder(customFolders)
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

void FileSegregator ::SegregateFile(const std ::string &filePath)
{
    std ::string category = GetFileCategory(filePath);

    if (category.empty())
    {
        category = "Uncategorized";
        Logger::logWarning("No category found for file: " + filePath + ". Moving to Uncategorized.");
    }

    std::string destinationFolder = categoryToFolder.count(category) > 0 ? categoryToFolder[category].first : defaultFolders[category];
    MoveFileToCategory(filePath, destinationFolder);
}

void FileSegregator::MoveFileToCategory(const std::string &filePath, const std::string &folderPath)
{

    CreateDirectoryIfNotExists(folderPath);

    std::filesystem::path destPath = folderPath / std::filesystem::path(filePath).filename();
    try
    {
        std::filesystem::rename(filePath, destPath);
        Logger::logInfo("Moved file " + filePath + " to " + folderPath);
    }
    catch (const std::exception &e)
    {
        Logger::logError("Failed to move file " + filePath + ": " + e.what());
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
            Logger::logInfo("Created directory: " + dirPath);
        }
        catch (const std::exception &e)
        {
            Logger::logError("Failed to create directory " + dirPath + ": " + e.what());
        }
    }
}

bool FileSegregator::IsDirectory(const std::string &path)
{
    return std::filesystem::is_directory(path);
}