//
// Created by lapestand on 4.06.2020.
//

#ifndef RE_FA_REGEXPARSER_H
#define RE_FA_REGEXPARSER_H

#include <iostream>
#include <utility>
#include <stack>
#include <vector>
#include "Properties.h"

class RegexParser {
public:
    RegexParser(std::string r);

    ~RegexParser();

private:
    std::string rgx_;
    std::string concattedVersion_;
    std::string postfixNorm_;
    bool isValid_;

private:
    bool isUpperCaseLetter(char letter);

    bool isRegexValid(const std::string &r);

    std::string insertConcat(std::string rgxWithoutConcat);

    int priority(char c);

    std::string infix2postfix(const std::string &infix);

public:
    bool isValid() const;

    std::string rgx() const;

    std::string concattedVersion() const;

    std::string postfixNorm() const;
};


#endif //RE_FA_REGEXPARSER_H
