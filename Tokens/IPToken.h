#pragma once

#include "../header.h"
#include "TokenChecker.h"
#include "../DB/DataBase.h"

#include "Templates/TemplateString.h"

class IPToken : public TokenChecker {

public:
    explicit IPToken(ConfigManager &config, DataBase &db) : TokenChecker(config), db(db) {
        db.createTable("ip");
    }

    static std::pair<bool, std::pair<std::string, std::string>> getIp(const std::string &word) {
        int dotsCnt = 0;
        std::string digitsStr;

        std::string result;
        std::string suffix;

        bool flag = false;

        for (char c: word) {
            if (flag) {
                suffix += c;
                continue;
            }
            if (digitsStr.size() == 3 && dotsCnt == 3) {
                result += digitsStr;
                flag = true;
                continue;
            }
            if (c == '.') {
                if (digitsStr.empty())
                    return {false, {"", word}};
                result += digitsStr;
                digitsStr.clear();
                dotsCnt++;
            } else if ('0' <= c && c <= '9') {
                digitsStr.push_back(c);
            } else {
                result += digitsStr;
                if (!digitsStr.empty() && dotsCnt == 3) {
                    flag = true;
                    continue;
                }
                return {false, {"", word}};
            }
        }

        if (flag) {
            return {true, {result, suffix}};
        }

        result += digitsStr;
        if (!digitsStr.empty() && dotsCnt == 3) {
            return {true, {result, suffix}};
        }
        return {false, {"", word}};
    }

    bool checkWord(const std::string &word) override {
        return getIp(word).first;
    }

    void checkStack(std::vector<std::string> &stack) override {
        while (checkWord(stack.back())) {
            auto result = getIp(stack.back());
            stack.pop_back();

            std::string name;

            if (db.checkValueExists("ip", result.second.first)) {
                name = db.getCode("ip", result.second.first);
            }
            else {
                std::map<std::string, std::string> templateVars;
                templateVars["id"] = std::to_string(db.getNextId("ip"));

                name = makeString(config["ip-template"], templateVars);
                db.insertElement("ip", name, result.second.first);
            }
            stack.push_back(name);
            stack.push_back(result.second.second);
        }
    }

private:
    DataBase &db;

};