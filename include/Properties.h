//
// Created by izztylmaz on 29.05.2020.
//

#ifndef RE_FA_PROPERTIES_H
#define RE_FA_PROPERTIES_H

/** MODE **/
#include <vector>

#define DEBUG true
#define INFO false

/** SYMBOLS **/
#define STAR_SYMBOL '*'
#define OPEN_PARENTHESIS_SYMBOL '('
#define CLOSE_PARENTHESIS_SYMBOL ')'
#define UNION_SYMBOL '|'
#define CONCAT_SYMBOL '.'
#define EPSILON_SYMBOL 26


#define ALPHABET_SIZE 26  // Alphabet [A-Z]
#define MAX_TRANSITION_COUNT_PER_STATE 2

/** PRIORITIES **/
#define STAR_PRIORITY 3
#define CONCAT_PRIORITY 2
#define UNION_PRIORITY 1
#define MIN_PRIORITY 0

/** TRANSLATION TABLE **/
#define STATE_ID 0
#define EPSILON_TRANSITIONS 1
#define SYMBOL_TRANSITIONS 2

#define COLUMN_COUNT 3
#define UNUSED_TRANSITION -1

#define SEPERATOR_COUNT 50

typedef std::vector <std::vector <std::vector <int>>> nfa_Table;
typedef nfa_Table dfa_Table;

#endif //RE_FA_PROPERTIES_H
