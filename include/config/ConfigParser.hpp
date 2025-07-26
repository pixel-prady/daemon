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
    std ::string getString(const std ::string &key, const std ::string default = "");
    int getInt(const std ::string &key, int default = 0);
    double getDouble(const std ::string &key, double default = 0.0);

private:
    std ::string filepath;
    std ::unordered_map<std ::string, std ::string> configData;

    bool parseYaml();
    bool parseJson(); // will do it later //
};

#endif;