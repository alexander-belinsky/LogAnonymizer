#pragma once

#include "../header.h"
#include "../Config/ConfigManager.h"

class TokenChecker {

public:
    explicit TokenChecker (ConfigManager &config): config(config) {}


    void connectDB(std::string &path, std::string &tableName) {

    }

    virtual bool checkWord(const std::string &word) {
        return false;
    }

    virtual void checkStack(std::vector<std::string> &stack) {
    }

protected:
    ConfigManager &config;
};

