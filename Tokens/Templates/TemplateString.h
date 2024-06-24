#pragma once

#include "../../header.h"

std::string makeString(std::string &&templ, std::map<std::string, std::string> vars) {
    bool isVar = false;
    std::string varName;
    std::string res;
    for (char c: templ) {
        if (c == '}') {
            isVar = false;
            res += vars[varName];
            varName.clear();
            continue;
        }
        if (isVar) {
            varName += c;
            continue;
        }
        if (c == '{') {
            isVar = true;
            continue;
        }
        res.push_back(c);
    }
    return res;
}
