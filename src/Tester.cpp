//
// Created by lapestand on 4.06.2020.
//

#include "../include/Tester.h"

void Tester::test_isRegexValid(const std::string& str) {
    RegexParser tempParser(str);
    std::cout << '\n' << str;
    if (tempParser.isValid()) std::cout << " - Valid!\n";
    else std::cout << " - UnValid!\n";
}

Tester::Tester() = default;

Tester::~Tester() = default;
