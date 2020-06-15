/**
 * REGEX VALIDATION - DONE
 * REGEX TO NFA
 * NFA TO DFA
 * PRINT DFA
 * */

#include <iostream>
#include <string>
#include "include/Properties.h"
#include "include/NFA.h"
#include "include/RegexParser.h"
#include "include/DFA.h"

void test(std::string str);

/** Driver Code **/
int main() {
    /** Test for regex validation **/
    /*
    test_isRegexValid(std::string("deneme"));
    test_isRegexValid(std::string("(/)"));
    test_isRegexValid(std::string("A*"));
    test_isRegexValid(std::string("AB"));
    test_isRegexValid(std::string("A|B"));
    test_isRegexValid(std::string("(*XY*)|Z"));
    test_isRegexValid(std::string("*XY*|Z"));
    test_isRegexValid(std::string("XY*|*Z"));
    test_isRegexValid(std::string("((A|B)|CD)"));
    test_isRegexValid(std::string("(A|B)|CD"));
     */
//    std::string rgx = std::string("(A|B)*|CDD");
    std::string rgx = std::string("A(A|B)*B");

    if (DEBUG){
        std::cout << "\nParsing started..";
    }
    if (INFO){
        std::cout << "\nShunting Yard Algorithm used for transiton from infix to postfix";
    }
    RegexParser regexParser(rgx);

    if (!regexParser.isValid()){
        if (DEBUG){
            system("clear");
            std::cout << "Wrong syntax in regex! Exiting!";
        }
        return -1;
    }
    std::cout << '\n' << regexParser.concattedVersion();
    std::cout << '\n' << regexParser.postfixNorm();


    std::string postfix = regexParser.postfixNorm();
    NFA nfa(postfix);
    nfa_Table table = nfa.nfaTable();
    if (DEBUG){
        nfa.printTable();
    }
    DFA dfa(table, nfa.finalState());

    std::cout << "\nProgram Closing...";
    return 0;
}
