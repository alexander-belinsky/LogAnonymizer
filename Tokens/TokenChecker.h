#include <utility>

#pragma once

class TokenManager {

public:
    explicit TokenManager(std::string configPath): configPath(std::move(configPath)) {}

    bool checkWord(const std::string &word) {
        return false;
    }

    bool checkStack(std::vector<std::string> &stack) {
        return false;
    }

private:
    std::string configPath;
};

