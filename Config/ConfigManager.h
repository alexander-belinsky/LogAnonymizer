#pragma once

#include "../header.h"

class ConfigManager {

public:
    ConfigManager(std::string &&configPath) {
        fillMap(configPath);
    }

    std::string operator [] (std::string &&index) {
        return configMap[index];
    }

private:

    void removeSpaces(std::string &str) {
        std::string res;
        for (char c: str) {
            if (!isspace(c))
                res += c;
        }
        str = res;
    }

    void fillMap(std::string &configPath) {
        std::ifstream fin(configPath);
        if (!fin.is_open())
            throw std::runtime_error("[CONFIG]: Error while opening file!");

        std::string line;
        while (std::getline(fin, line)) {
            removeSpaces(line);
            std::stringstream lineStream(line);

            std::string var, value;
            std::getline(lineStream, var, ':');
            std::getline(lineStream, value);
            if (var.empty() || value.empty())
                throw std::runtime_error("[CONFIG]: Wrong structure of config file!");

            configMap[var] = value;
        }
    }

    std::map<std::string, std::string> configMap;
};