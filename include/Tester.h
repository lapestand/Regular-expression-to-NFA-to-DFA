//
// Created by izztylmaz on 4.06.2020.
//

#ifndef RE_FA_TESTER_H
#define RE_FA_TESTER_H

#include <iostream>
#include "RegexParser.h"
#include "NFA.h"

class Tester {
public:
    Tester();
    ~Tester();
    static void test_isRegexValid(const std::string& str);
};


#endif //RE_FA_TESTER_H
