#include "FileSegregator.hpp"
#include "Logger.hpp"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <fstream>

class SimpleLogger : public Logger {
public:
    void logInfo(const std::string& message) override {}
    void logWarning(const std::string& message) override {}
    void logError(const std::string& message) override {}
};

void createDummyFile(const std::string& path) {
    std::ofstream ofs(path);
    ofs << "test";
}

bool fileExists(const std::string& path) {
    return std::filesystem::exists(path);
}

bool dirExists(const std::string& path) {
    return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

void printResult(const std::string& testName, bool passed) {
    std::cout << testName << (passed ? "_passed" : "_failed") << std::endl;
}

int main() {
    SimpleLogger logger;

    std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> customFolders = {
        {"Images", {"./TestImages", {".jpg", ".png"}}},
        {"Documents", {"./TestDocs", {".txt", ".pdf"}}}
    };

    FileSegregator segregator(customFolders, &logger);

    // Test 1: Check if GetFileCategory returns correct categories based on file extension
    bool t1 = true;
    t1 &= (segregator.GetFileCategory("photo.jpg") == "Images");
    t1 &= (segregator.GetFileCategory("doc.txt") == "Documents");
    t1 &= (segregator.GetFileCategory("unknown.xyz") == "");
    printResult("test1", t1);

    // Prepare files for move tests
    createDummyFile("photo.jpg");
    createDummyFile("doc.txt");
    createDummyFile("unknown.xyz");

    // Test 2: Check if SegregateFile moves known files to the correct custom folders
    segregator.SegregateFile("photo.jpg");
    segregator.SegregateFile("doc.txt");
    bool t2 = fileExists("./TestImages/photo.jpg") && fileExists("./TestDocs/doc.txt");
    printResult("test2", t2);

    // Test 3: Check if SegregateFile moves unknown files to Uncategorized folder
    segregator.SegregateFile("unknown.xyz");
    bool t3 = fileExists("./Uncategorized/unknown.xyz");
    printResult("test3", t3);

    // Test 4: Check if CreateDirectoryIfNotExists creates directory when it doesn't exist
    std::string newDir = "./NewTestDir";
    if (dirExists(newDir)) std::filesystem::remove_all(newDir);
    segregator.CreateDirectoryIfNotExists(newDir);
    bool t4 = dirExists(newDir);
    printResult("test4", t4);

    // Test 5: Check that CreateDirectoryIfNotExists does nothing if directory already exists
    segregator.CreateDirectoryIfNotExists(newDir);
    bool t5 = dirExists(newDir);
    printResult("test5", t5);

    // Test 6: Check if IsDirectory correctly identifies directories and files
    bool t6 = segregator.IsDirectory("./TestImages") && !segregator.IsDirectory("./TestImages/photo.jpg");
    printResult("test6", t6);

    // Test 7: Check if SegregateExistingFiles processes all files in a directory properly
    std::string existingDir = "./ExistingFilesTest";
    if (dirExists(existingDir)) std::filesystem::remove_all(existingDir);
    std::filesystem::create_directory(existingDir);
    createDummyFile(existingDir + "/file1.jpg");
    createDummyFile(existingDir + "/file2.txt");
    createDummyFile(existingDir + "/file3.unknown");
    segregator.SegregateExistingFiles(existingDir, true);
    bool t7 = fileExists("./TestImages/file1.jpg") && fileExists("./TestDocs/file2.txt") && fileExists("./Uncategorized/file3.unknown");
    printResult("test7", t7);

    // Test 8: Check if SegregateFile handles non-existent file gracefully (no crash)
    segregator.SegregateFile("non_existent_file.xyz");
    printResult("test8", true);

    // Cleanup
    std::filesystem::remove_all("./TestImages");
    std::filesystem::remove_all("./TestDocs");
    std::filesystem::remove_all("./Uncategorized");
    std::filesystem::remove_all(newDir);
    std::filesystem::remove_all(existingDir);

    return 0;
}
