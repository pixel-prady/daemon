#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "Logger.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class ConfigParser
{
public:
    ConfigParser(const std ::string &filepath,Logger * logger);

    bool loadConfig();
    bool updateConfigFile(const std::string& key, const std::string& value);
    std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> loadFolderConfig();
    std ::string getString(const std ::string &key, const std ::string &defaultValue = "");
    int getInt(const std ::string &key, int defaultValue = 0);
    double getDouble(const std ::string &key, double defaultValue = 0.0);
    bool getBool( const std :: string &key, bool defaultValue = false) ; 

private:
    std ::string filepath;
    std ::unordered_map<std ::string, std ::string> configData;
    Logger * logger ; 

    bool parseYaml();
    // bool parseJson(); // will do it later //
};

#endif