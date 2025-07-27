#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <string>
#include <unordered_map>
#include <vector>

class ConfigParser
{
public:
    ConfigParser(const std ::string &filepath);

    bool loadConfig();
    std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> loadFolderConfig();
    std ::string getString(const std ::string &key, const std ::string &defaultValue = "");
    int getInt(const std ::string &key, int defaultValue = 0);
    double getDouble(const std ::string &key, double defaultValue = 0.0);

private:
    std ::string filepath;
    std ::unordered_map<std ::string, std ::string> configData;

    bool parseYaml();
    // bool parseJson(); // will do it later //
};

#endif