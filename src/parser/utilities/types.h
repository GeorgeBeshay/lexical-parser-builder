#ifndef LEXICAL_PARSER_BUILDER_TYPES_H
#define LEXICAL_PARSER_BUILDER_TYPES_H

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "ParsingToken.h"

using namespace std;

// type aliases for better readability.
using t_prodAlt = vector<ParsingToken>;
using t_prodRule = unordered_set<t_prodAlt, ParsingTokenVectorHash>;
using t_grammar = unordered_map<ParsingToken, t_prodRule, ParsingTokenHash>;

using t_nonTerminalSet = unordered_set<ParsingToken, ParsingTokenHash>; // To be used first set and follow set for each non-terminal
using t_nonTerminalSetMap = unordered_map<ParsingToken, t_nonTerminalSet, ParsingTokenHash>;

#endif //LEXICAL_PARSER_BUILDER_TYPES_H
