//
// Created by lapestand on 4.06.2020.
//

#include "../include/RegexParser.h"


RegexParser::RegexParser(std::string r) {
    rgx_ = std::move(r);
    if (isRegexValid(rgx_)) {
        isValid_ = true;
        concattedVersion_ = insertConcat(rgx_);
//        std::cout << concattedVersion_;
        postfixNorm_ = infix2postfix(concattedVersion_);
//        std::cout << '\n' << postfixNorm_;
    } else {
        isValid_ = false;
    }
}

bool RegexParser::isRegexValid(const std::string &r) {
    int openParenthesisCount = 0;
    int closeParenthesisCount = 0;
    char currentElement;

    if (r.empty()) return false;
    for (int i = 0; i < r.size(); ++i) {
        currentElement = r[i];
        if (currentElement == OPEN_PARENTHESIS_SYMBOL) { ++openParenthesisCount; }
        else if (currentElement == CLOSE_PARENTHESIS_SYMBOL) {
            if (i == 0) return false;
            if (r[i - 1] == OPEN_PARENTHESIS_SYMBOL) return false;
            ++closeParenthesisCount;
        } else if (currentElement == STAR_SYMBOL) {
            if (i == 0) return false;
            if (r[i - 1] == STAR_SYMBOL || r[i - 1] == UNION_SYMBOL || r[i - 1] == OPEN_PARENTHESIS_SYMBOL)
                return false;
        } else if (currentElement == UNION_SYMBOL) {
            if (i == 0) return false;
            if (r[i - 1] == UNION_SYMBOL || r[i - 1] == OPEN_PARENTHESIS_SYMBOL) return false;
        } else if (!isUpperCaseLetter(currentElement)) { return false; }
        if (closeParenthesisCount > openParenthesisCount) return false;
    }
    return true;
}

std::string RegexParser::insertConcat(std::string rgxWithoutConcat) {
    char currentChar, nextChar, lastChar = rgxWithoutConcat.back();
    std::string tmp;
    for (int i = 0; i < rgxWithoutConcat.size() - 1; ++i) {
        currentChar = rgxWithoutConcat[i];
        nextChar = rgxWithoutConcat[i + 1];
        tmp.push_back(currentChar);
//        if (isUpperCaseLetter(currentChar) && (isUpperCaseLetter(nextChar) || nextChar == OPEN_PARENTHESIS_SYMBOL)) {
        if (currentChar != OPEN_PARENTHESIS_SYMBOL && currentChar != UNION_SYMBOL){
            if (nextChar != CLOSE_PARENTHESIS_SYMBOL && nextChar != UNION_SYMBOL && nextChar != STAR_SYMBOL){
                tmp.push_back(CONCAT_SYMBOL);
            }
        }
    }
    tmp.push_back(lastChar);
    return tmp;
}

bool RegexParser::isUpperCaseLetter(char letter) {
    return letter >= 'A' && letter <= 'Z';
}

int RegexParser::priority(char c){
    if (c == STAR_SYMBOL) return STAR_PRIORITY;
    if (c == CONCAT_SYMBOL) return CONCAT_PRIORITY;
    if (c== UNION_SYMBOL) return UNION_PRIORITY;
    return MIN_PRIORITY;
}

std::string RegexParser::infix2postfix(const std::string &infix) {
    std::string tmpPost;
    std::vector<int> symbols = {STAR_SYMBOL, CONCAT_SYMBOL, UNION_SYMBOL};
    std::stack<char> st;
    std::string currentChar;
    for (char c : infix) {
        if (std::find(symbols.begin(), symbols.end(), c) != symbols.end()) {
            while (!st.empty()){
                if (priority(st.top()) >= priority(c)){
                    tmpPost.push_back(st.top());
                    st.pop();
                }else break;
            }
            st.push(c);
        } else if (c == OPEN_PARENTHESIS_SYMBOL) {
            st.push(c);
        } else if (c == CLOSE_PARENTHESIS_SYMBOL){
            while (st.top() != OPEN_PARENTHESIS_SYMBOL){
                tmpPost.push_back(st.top());
                st.pop();
            }
            st.pop();
        }else{
            tmpPost.push_back(c);
        }
    }
    while (!st.empty()){
        tmpPost.push_back(st.top());
        st.pop();
    }
    return tmpPost;
}


/** GETTERS **/
bool RegexParser::isValid() const { return isValid_; }

std::string RegexParser::concattedVersion() const { return concattedVersion_; }

std::string RegexParser::postfixNorm() const { return postfixNorm_; }

std::string RegexParser::rgx() const { return rgx_; }


/** DESTRUCTOR **/
RegexParser::~RegexParser() = default;
