#include "config/ConfigParser.hpp"
#include <fstream>
#include <iostream>
#include <yaml-cpp/yaml.h>

ConfigParser::ConfigParser(const std::string &path)
    : filePath(path) {}

bool ConfigParser ::loadConfig()
{
    return parseYaml();
}

std ::unordered_map<std ::string, std ::pair<std ::string, std ::vector<std ::string>>> ConfigParser ::loadFolderConfig()
{
    std ::unordered_map<std ::string, std ::pair<std ::string, std ::vector<std ::string>>> folders;
    try
    {
        YAML ::Node config = YAML::LoadFile(filepath);
        YAML ::Node folderConfig = config["folders"];
        for (YAML ::yaml_iterator itr = folderConfig.begin(); itd != folderConfig.end(); itr++)
        {
            std ::string category = itr->first.as<std ::string>();
            std ::string path = itr->second["path"].as<std ::string>();
            std ::vector<std ::string> extensions = itr->second["extensions"].as<std ::vector<std ::string>>();

            folders[category] = {path, extensions};
        }

        return folders;
    }
    catch (const YAML::Exception &e)
    {
        std::cerr << "ERROR LOADING THE FOLDER CONFIGURATIONS" << e.what() << '\n';
        return folders;
    }
}
bool ConfigParser ::parseYaml()
{
    try
    {
        YAML::Node config = YAML::LoadFile(filepath);
        if (config["cpu_idle_thershold"])
        {
            configData["cpu_idle_threshold"] = config["cpu_idle_threshold"].as<std::string>();
        }
        if (config["watch_path"])
        {
            configData["watch_path"] = config["watch_path"].as<std::string>();
        }

        return true;
    }
    catch (const YAML::Exception &e)
    {
        std::cerr << "Error loading YAML config: " << e.what() << std::endl;
        return false;
    }
}

std::string ConfigParser::getString(const std::string &key, const std::string &default)
{
    return configData.count(key) ? configData[key] : default;
}

int ConfigParser::getInt(const std::string &key, int default)
{
    return configData.count(key) ? std::stoi(configData[key]) : default;
}

double ConfigParser::getDouble(const std::string &key, double default)
{
    return configData.count(key) ? std::stod(configData[key]) : default;
}