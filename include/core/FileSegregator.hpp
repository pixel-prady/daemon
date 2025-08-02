#ifndef FILESEGREGATOR_HPP
#define FILESEGREGATOR_HPP

#include <string>
#include <unordered_map>
#include <vector>

class FileSegregator
{
public:
    FileSegregator(const std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> &customFolders);

    ~FileSegregator();

    void SegregateFile(const std::string &filePath);

    void SegregateExistingFiles (const std::string &dirPath,bool value) ; 

private:
    void MoveFileToCategory(const std::string &filePath, const std::string &category);

    std::string GetFileCategory(const std::string &filePath);

    bool IsDirectory(const std::string &path);

    void CreateDirectoryIfNotExists(const std::string &dirPath);

    std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> categoryToFolder;

    std::unordered_map<std::string, std::string> defaultFolders;
};

#endif 
