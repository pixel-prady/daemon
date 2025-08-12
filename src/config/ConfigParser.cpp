#include "ConfigParser.hpp"
#include <fstream>
#include <yaml-cpp/yaml.h>

ConfigParser::ConfigParser(const std::string &path, Logger* loggerInstance)
    : filepath(path), logger(loggerInstance) {}

bool ConfigParser::loadConfig()
{
    return parseYaml();
}

std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> ConfigParser::loadFolderConfig()
{
    std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> folders;
    try
    {
        YAML::Node config = YAML::LoadFile(filepath);
        if (config["folders"])
        {
            YAML::Node folderConfig = config["folders"];
            for (YAML::const_iterator itr = folderConfig.begin(); itr != folderConfig.end(); ++itr)
            {
                std::string category = itr->first.as<std::string>();
                std::string path = itr->second["path"].as<std::string>();
                std::vector<std::string> extensions = itr->second["extensions"].as<std::vector<std::string>>();

                folders[category] = {path, extensions};
            }
        }

        return folders;
    }
    catch (const YAML::Exception &e)
    {
        logger->logError(std::string("ERROR LOADING THE FOLDER CONFIGURATIONS: ") + e.what());
        return folders;
    }
}

bool ConfigParser::parseYaml()
{
    try
    {
        YAML::Node config = YAML::LoadFile(filepath);
        if (config["cpu_idle_threshold"])
        {
            configData["cpu_idle_threshold"] = config["cpu_idle_threshold"].as<std::string>();
        }
        if (config["watch_path"])
        {
            configData["watch_path"] = config["watch_path"].as<std::string>();
        }
        if (config["segregate_existing_files"])
        {
            configData["segregate_existing_files"] = config["segregate_existing_files"].as<bool>();
        }

        return true;
    }
    catch (const YAML::Exception &e)
    {
        logger->logError(std::string("Error loading YAML config: ") + e.what());
        return false;
    }
}

bool ConfigParser ::  updateConfigFile(const std::string& key, const std::string& value) {
    try {
        YAML::Node config = YAML::LoadFile(filepath);

        if (key == "cpu_idle_threshold") {
            config["cpu_idle_threshold"] = std::stoi(value);
        } else if (key == "watch_path") {
            config["watch_path"] = value;
        } else if (key == "segregate_existing_files") {
            config["segregate_existing_files"] = (value == "true" || value == "1");
        } else if(key == "monitoring_interval") {
            config["monitoring_interval"] = value; 
        }else {
            logger->logWarning("Updating complex keys is not supported right now.");
            return false;
        }

        std::ofstream outFile(filepath);
        outFile << YAML::Dump(config);
        outFile.close();
        logger->logInfo("Config updated successfully.");

        return true;
    } catch (const std::exception& e) {
        logger->logError(std::string("Failed to update config: ") + e.what());
        return false;
    }
}

std::string ConfigParser::getString(const std::string &key, const std::string &defaultValue)
{
    return configData.count(key) ? configData[key] : defaultValue;
}

int ConfigParser::getInt(const std::string &key, int defaultValue)
{
    return configData.count(key) ? std::stoi(configData[key]) : defaultValue;
}

double ConfigParser::getDouble(const std::string &key, double defaultValue)
{
    return configData.count(key) ? std::stod(configData[key]) : defaultValue;
}

bool ConfigParser::getBool(const std::string &key, bool defaultValue)
{
    return configData.count(key) ? (configData[key] == "true" || configData[key] == "1" ? true : false) : defaultValue;
}
