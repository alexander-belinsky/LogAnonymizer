#pragma once

#include "Config/ConfigManager.h"
#include "DB/DataBase.h"

#include "Tokens/TokenChecker.h"

class MainManager {

public:
    MainManager(): config(".config"), db(config["db-path"]),
    tokens({new IPToken(config, db)}) {

    }

    ~MainManager() {
        for (auto token: tokens)
            delete token;
    }

    void anonymize() {
        fileIn.open(config["log-input"]);
        fileOut.open(config["log-output"]);

        char c = -1;
        fileIn.get(c);
        while (!fileIn.eof()) {
            std::string word;
            while (!fileIn.eof() && !isspace(c)) {
                word += c;
                fileIn.get(c);
            }
            while (!fileIn.eof() && isspace(c)) {
                word += c;
                fileIn.get(c);
            }
            stack.push_back(word);
            updateStack();
        }
    }

private:
    void updateStack() {
        for (auto token: tokens) {
            if (token->checkWord(stack.back())) {
                token->checkStack(stack);
                break;
            }
        }
        spillStack();
    }

    void spillStack() {
        for (auto& str: stack) {
            fileOut.write(str.c_str(), str.size());
        }
        stack.clear();
    }

private:
    ConfigManager config;
    DataBase db;

    std::vector<std::string> stack;
    std::vector<TokenChecker*> tokens;

    std::ofstream fileOut;
    std::ifstream fileIn;
};
