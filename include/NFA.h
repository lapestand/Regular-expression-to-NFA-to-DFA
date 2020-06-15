#ifndef RE_FA_NFA_H
#define RE_FA_NFA_H


#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <memory>
#include "Properties.h"

class NFA {
public:
    NFA(std::string& rgx);
    ~NFA();

private:
    struct state_{
        bool finalState;
        int stateId;
        state_* transitions[ALPHABET_SIZE];
        state_* epsilonTransitions[MAX_TRANSITION_COUNT_PER_STATE];
    };

    struct nfa_{
        state_* start;
        state_* end;
    };

    std::string postfix_;
    nfa_ currentNFA_;
    int stateOrder;
    int totalStateCount;
    int finalState_;
    nfa_Table nfaTable_;

private:
    nfa_ postfix2nfa(std::string pfx);

    nfa_* createNfa_();
    void addTransition(state_* from, state_* to, char symbol);
    nfa_* fromEpsilon();
    nfa_* fromSymbol(char symbol);
    nfa_* star_(nfa_* nf);
    nfa_* concat_(nfa_* first, nfa_* second);
    nfa_* union_(nfa_* first, nfa_* second);

    nfa_Table createTransitionTable(nfa_* curNFA, int sC);  // current nfa & state count
    nfa_Table initTable(int sC);  // init table
    void walk(state_* curState, nfa_Table& curTable, std::vector <state_*>& visited, std::vector <state_*>& saved,std::vector <int>& rows, int curRow);
    int findIdx(std::vector<state_ *> &visited, std::vector<int> &rows, state_ *const pState);

public:
    nfa_ currentNfa();
    nfa_Table nfaTable();
    int finalState();
    void printTable();

};


#endif //RE_FA_NFA_H
