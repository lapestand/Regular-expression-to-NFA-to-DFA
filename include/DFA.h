//
// Created by izztylmaz on 4.06.2020.
//

#ifndef RE_FA_DFA_H
#define RE_FA_DFA_H

#include <iostream>
#include <vector>
#include "Properties.h"

class DFA {
public:
    DFA(nfa_Table tb, int f);
    ~DFA();

private:
    nfa_Table nfaTable_;
    int finalState_;
    std::vector <std::vector <int>> epsilonClosures_;

private:
    std::vector<int> epsilonClosure(std::vector<std::vector<int>> nfaTable, int currentState, std::vector<int>& visited);
    dfa_Table extractDFA();

    void addRow(dfa_Table &tb, const std::vector<int>& vector);
};


#endif //RE_FA_DFA_H
