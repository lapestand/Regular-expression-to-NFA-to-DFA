#include "../include/DFA.h"

#include <utility>
#include <iostream>


DFA::~DFA() = default;

DFA::DFA(nfa_Table tb, int f) {
    nfaTable_ = tb;
    finalState_ = f;
    std::vector <int> visited;
    for (int i = 0; i < tb.size(); ++i) {
        epsilonClosures_.push_back(epsilonClosure(nfaTable_[i], i, visited));
        std::sort(epsilonClosures_.back().begin(), epsilonClosures_.back().end());
        if (DEBUG){
            std::cout << "\nEpsilon Closures for " << i << ": { ";
            for (int j : epsilonClosures_.back()) {
                std::cout << j << " ";
            }
            std::cout << '}';
        }
        visited.clear();
    }
    extractDFA();
}

std::vector<int>
DFA::epsilonClosure(std::vector<std::vector<int>> nfaTable, int currentState, std::vector<int>& visited) {
    std::vector<int> eCs, newEpsilons;

    for (int nextState : nfaTable[EPSILON_TRANSITIONS]) {
        if (std::find(visited.begin(), visited.end(), nextState) == visited.end()){
            newEpsilons = epsilonClosure(nfaTable_[nextState], nextState, visited);
            eCs.insert(std::end(eCs), std::begin(newEpsilons), std::end(newEpsilons));
        }
    }

    eCs.push_back(currentState);
    return eCs;
}

dfa_Table DFA::extractDFA() {
    dfa_Table newTable;
    addRow(newTable, epsilonClosures_[0]);
    std::vector <std::vector <int>> saved, visited;
    std::vector <int> currentUnitedStates;
    saved.push_back(epsilonClosures_[0]);
    visited.push_back(epsilonClosures_[0]);
    for (int i = 0; i < saved.size(); ++i) {
        currentUnitedStates = saved[i];
        /*for (auto & currentState : currentUnitedStates) {
            symbolTransitions(currentState)
        }*/
    }
    return newTable;
}

void DFA::addRow(dfa_Table &tb, const std::vector<int>& eClosures) {
    tb.resize(tb.size()+1);
    tb.back().resize(ALPHABET_SIZE);
    for (int key = 0; key < ALPHABET_SIZE; ++key) {
        for (auto & j : eClosures) {
            if (nfaTable_[j][SYMBOL_TRANSITIONS][key] != -1){
                if (std::find(tb.back()[key].begin(), tb.back()[key].end(), key) == tb.back()[key].end()){
                    tb.back()[key].push_back(key);
                }
            }
        }
    }
}
