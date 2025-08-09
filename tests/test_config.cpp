#include <gtest/gtest.h>
#include "ConfigParser.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

// Mock file creation utility for testing

void createMockConfigFile(const std::string &filepath) {
    std::ofstream outFile(filepath);
    outFile << "cpu_idle_threshold: \"50\"\n";
    outFile << "watch_path: \"/home/user\"\n";
    outFile << "segregate_existing_files: true\n";
    outFile << "folders:\n";
    outFile << "  category1:\n";
    outFile << "    path: \"/path/to/folder1\"\n";
    outFile << "    extensions: [\".txt\", \".cpp\"]\n";
    outFile << "  category2:\n";
    outFile << "    path: \"/path/to/folder2\"\n";
    outFile << "    extensions: [\".jpg\", \".png\"]\n";
    outFile.close();
}

// Test for ConfigParser class //  Test Fixture Setup
class ConfigParserTest : public ::testing::Test {
protected:
    // Filepath for the mock config file
    const std::string mockFilePath = "test_config.yaml";

    // This is run before each test
    void SetUp() override {
        // Create a mock configuration file before each test
        createMockConfigFile(mockFilePath);
    }

    // This is run after each test
    void TearDown() override {
        // Clean up the mock config file after each test
        std::remove(mockFilePath.c_str());
    }
};

// Test loadConfig() - Valid case
TEST_F(ConfigParserTest, LoadConfig_ValidFile) {
    ConfigParser configParser(mockFilePath);
    ASSERT_TRUE(configParser.loadConfig());
}

// Test loadConfig() - Invalid file
TEST_F(ConfigParserTest, LoadConfig_InvalidFile) {
    ConfigParser configParser("invalid_config.yaml");
    ASSERT_FALSE(configParser.loadConfig());
}

// Test loadFolderConfig() - Valid folders
TEST_F(ConfigParserTest, LoadFolderConfig_Valid) {
    ConfigParser configParser(mockFilePath);
    auto folders = configParser.loadFolderConfig();
    ASSERT_EQ(folders.size(), 2);

    // Test folder "category1"
    ASSERT_EQ(folders["category1"].first, "/path/to/folder1");
    ASSERT_EQ(folders["category1"].second.size(), 2);
    ASSERT_EQ(folders["category1"].second[0], ".txt");
    ASSERT_EQ(folders["category1"].second[1], ".cpp");

    // Test folder "category2"
    ASSERT_EQ(folders["category2"].first, "/path/to/folder2");
    ASSERT_EQ(folders["category2"].second.size(), 2);
    ASSERT_EQ(folders["category2"].second[0], ".jpg");
    ASSERT_EQ(folders["category2"].second[1], ".png");
}

// Test parseYaml() - Valid config
TEST_F(ConfigParserTest, ParseYaml_Valid) {
    ConfigParser configParser(mockFilePath);
    ASSERT_TRUE(configParser.loadConfig());  // Ensure the file loads correctly

    // Test specific values
    ASSERT_EQ(configParser.getString("cpu_idle_threshold", ""), "50");
    ASSERT_EQ(configParser.getString("watch_path", ""), "/home/user");
    ASSERT_EQ(configParser.getBool("segregate_existing_files", false), true);
}

// Test parseYaml() - Invalid config
TEST_F(ConfigParserTest, ParseYaml_Invalid) {
    // Simulate a corrupted or missing config
    ConfigParser configParser("invalid_config.yaml");
    ASSERT_FALSE(configParser.loadConfig());
}

// Test getString()
TEST_F(ConfigParserTest, GetString_Valid) {
    ConfigParser configParser(mockFilePath);
    configParser.loadConfig();
    ASSERT_EQ(configParser.getString("cpu_idle_threshold", ""), "50");
    ASSERT_EQ(configParser.getString("non_existing_key", "default_value"), "default_value");
}

// Test getInt()
TEST_F(ConfigParserTest, GetInt_Valid) {
    ConfigParser configParser(mockFilePath);
    configParser.loadConfig();
    ASSERT_EQ(configParser.getInt("cpu_idle_threshold", 0), 50);  // "50" is converted to 50
    ASSERT_EQ(configParser.getInt("non_existing_key", 10), 10);
}

// Test getDouble()
TEST_F(ConfigParserTest, GetDouble_Valid) {
    ConfigParser configParser(mockFilePath);
    configParser.loadConfig();
    ASSERT_EQ(configParser.getDouble("cpu_idle_threshold", 0.0), 50.0);  // "50" is converted to 50.0
    ASSERT_EQ(configParser.getDouble("non_existing_key", 10.5), 10.5);
}

// Test getBool()
TEST_F(ConfigParserTest, GetBool_Valid) {
    ConfigParser configParser(mockFilePath);
    configParser.loadConfig();
    ASSERT_EQ(configParser.getBool("segregate_existing_files", false), true);
    ASSERT_EQ(configParser.getBool("non_existing_key", false), false);
}


