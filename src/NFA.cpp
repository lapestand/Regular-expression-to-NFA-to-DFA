#include "../include/NFA.h"


NFA::NFA(std::string& rgx) {
    postfix_ = rgx;

    finalState_ = -1;
    stateOrder= 0;
    totalStateCount = (int)postfix_.size()*2;
    std::cout << "\nNFA creating...";
    currentNFA_ = postfix2nfa(postfix_);
    std::cout << "\nNFA created, table extracting...";
    nfaTable_ = createTransitionTable(&currentNFA_, totalStateCount);
    std::cout << "\nTable extracted.";
}

NFA::nfa_ NFA::postfix2nfa(std::string pfx) {
    if (pfx.empty()) {
        return *fromEpsilon();
    }

    std::stack<nfa_*> st;
    nfa_ *top, *first, *second;
    /** AAB|*.B. */
    for (char currentChar : pfx) {
        if (currentChar == STAR_SYMBOL) {
            top = st.top(); st.pop();
            st.push(star_(top));
        } else if (currentChar == CONCAT_SYMBOL) {
            first = st.top(); st.pop();
            second = st.top(); st.pop();
            st.push(concat_(first, second));
        } else if (currentChar == UNION_SYMBOL) {
            first = st.top(); st.pop();
            second = st.top(); st.pop();
            st.push(union_(first, second));
        } else {
            st.push(fromSymbol(currentChar));
        }
    }

    return *st.top();
}

void NFA::addTransition(NFA::state_* from, NFA::state_* to, char symbol) {
    if (symbol == EPSILON_SYMBOL) {
        for (auto & epsilonTransition : from->epsilonTransitions){
            if (epsilonTransition == nullptr){
                epsilonTransition = to;
                break;
            }
        }
    } else {
        from->transitions[symbol - 'A'] = to;
    }
}

NFA::nfa_* NFA::fromEpsilon() {
    nfa_* newNfa = createNfa_();
    addTransition(newNfa->start, newNfa->end, EPSILON_SYMBOL);
    return newNfa;
}

NFA::nfa_* NFA::fromSymbol(char symbol) {
    nfa_* newNfa = createNfa_();
    addTransition(newNfa->start, newNfa->end, symbol);
    return newNfa;
}

NFA::nfa_* NFA::createNfa_() {
    nfa_* newNfa = new nfa_;

    newNfa->start = new state_;
    newNfa->start->finalState = false;
    newNfa->start->stateId = stateOrder++;

    newNfa->end = new state_;
    newNfa->end->finalState = true;
    newNfa->end->stateId = stateOrder++;

    return newNfa;
}

NFA::nfa_* NFA::star_(NFA::nfa_* currentNfa) {
    nfa_* newNfa_ = createNfa_();

    addTransition(newNfa_->start, currentNfa->start, EPSILON_SYMBOL);
    addTransition(currentNfa->end, newNfa_->end, EPSILON_SYMBOL);

    addTransition(newNfa_->start, newNfa_->end, EPSILON_SYMBOL);
    addTransition(currentNfa->end, currentNfa->start, EPSILON_SYMBOL);


    currentNfa->end->finalState = false;

    return newNfa_;
}

NFA::nfa_* NFA::concat_(NFA::nfa_* second, NFA::nfa_* first) {
    nfa_* newNfa = createNfa_();

    addTransition(newNfa->start, first->start, EPSILON_SYMBOL);
    addTransition(first->end, second->start, EPSILON_SYMBOL);
    addTransition(second->end, newNfa->end, EPSILON_SYMBOL);

    first->end->finalState = false;
    second->end->finalState = false;

    return newNfa;
}

NFA::nfa_* NFA::union_(NFA::nfa_* second, NFA::nfa_* first) {
    nfa_* newNfa = createNfa_();

    addTransition(newNfa->start, first->start, EPSILON_SYMBOL);
    addTransition(first->end, newNfa->end, EPSILON_SYMBOL);
    first->end->finalState = false;

    addTransition(newNfa->start, second->start, EPSILON_SYMBOL);
    addTransition(second->end, newNfa->end, EPSILON_SYMBOL);
    second->end->finalState = false;

    return newNfa;
}

nfa_Table NFA::createTransitionTable(NFA::nfa_* curNFA, int sC) {
    nfa_Table newTable = initTable(sC);
    std::vector <state_*> visited, saved;
    std::vector <int> rows;
    rows.push_back(0);
    state_* initialState = curNFA->start;
    saved.push_back(initialState);
    walk(initialState, newTable, visited, saved, rows, 0);
    return newTable;
}

nfa_Table NFA::initTable(int sC) {
    nfa_Table newTable;
    newTable.resize(sC);
    for ( int row = 0; row < sC; ++row){
        newTable[row].resize(COLUMN_COUNT);
        newTable[row][STATE_ID] = {row};
        newTable[row][SYMBOL_TRANSITIONS].assign(ALPHABET_SIZE, UNUSED_TRANSITION);
    }
    return newTable;
}

void NFA::walk(NFA::state_ *curState, nfa_Table& curTable, std::vector<state_*>& visited, std::vector <state_*>& saved, std::vector<int>& rows, int curRow) {
    if (std::find(visited.begin(), visited.end(), curState) != visited.end()) { return; }

    int idx = findIdx(saved, rows, curState);
    if (curState->finalState) { finalState_ = idx; return;}

    std::vector<state_*> rowStates;
    for (const auto & nextState : curState->epsilonTransitions) {
        if (nextState != nullptr){
            idx = findIdx(saved, rows, nextState);
            curTable[curRow][EPSILON_TRANSITIONS].push_back(idx);
            rowStates.push_back(nextState);
        }
    }

    for (int key = 0; key < ALPHABET_SIZE; ++key){
        if (curState->transitions[key] != nullptr){
            idx = findIdx(saved, rows, curState->transitions[key]);
            curTable[curRow][SYMBOL_TRANSITIONS][key] = idx;
            rowStates.push_back(curState->transitions[key]);
        }
    }

    visited.push_back(curState);

    for (auto & rowState : rowStates) {
        idx = findIdx(saved, rows, rowState);
        walk(rowState, curTable, visited, saved, rows, idx);
    }
}

int NFA::findIdx(std::vector<state_ *> &saved, std::vector<int> &rows, state_ *const pState) {
    auto it = std::find(saved.begin(), saved.end(), pState);
    if (it != saved.end()){
        return std::distance(saved.begin(), it);
    }else{
            saved.push_back(pState);
            rows.push_back(rows.back()+1);
            return rows.back();
    }
}

void NFA::printTable() {
    std::cout << std::endl;
    std::cout << std::string(SEPERATOR_COUNT/2-6, ' ') << "_____________";
    std::cout << '\n' << std::string(SEPERATOR_COUNT/2-6, ' ') << "| NFA TABLE |\n";
    std::cout << std::string(SEPERATOR_COUNT, '-');
    std::cout << std::endl;
    char key;
    for (auto & row : nfaTable_) {
        std::cout << row[STATE_ID].back();
        std::cout << "\t-->\t\t (€) -> { ";
        for (int i : row[EPSILON_TRANSITIONS]) {
            std::cout << i << ", ";
        }
        std::cout << " } (SYMBOL) -> { ";
        for (int j = 0; j < row[SYMBOL_TRANSITIONS].size(); ++j) {
            key = static_cast<char>(j+'A');
            if (row[SYMBOL_TRANSITIONS][j] != UNUSED_TRANSITION){
                std::cout << row[SYMBOL_TRANSITIONS][j] << "(" << key << "), ";
            }
        }
        std::cout << " } ";
        if (row[STATE_ID].back() == 0) std::cout << "  <-- INITIAL_STATE";
        if (row[STATE_ID].back() == finalState_) std::cout << "  <-- FINAL_STATE!";
        std::cout << std::endl;
    }
    std::cout << std::string(SEPERATOR_COUNT, '-');
}

NFA::nfa_ NFA::currentNfa() { return currentNFA_; }

nfa_Table NFA::nfaTable() { return nfaTable_; }

int NFA::finalState() { return finalState_; }


NFA::~NFA() = default;
