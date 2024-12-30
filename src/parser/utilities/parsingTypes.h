#ifndef LEXICAL_PARSER_BUILDER_PARSINGTYPES_H
#define LEXICAL_PARSER_BUILDER_PARSINGTYPES_H

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "ParsingToken.h"

using namespace std;

const ParsingToken EPSILON_TOKEN ("\\L", true);
const ParsingToken END_TOKEN ("$", true);
const ParsingToken SYNC_TOKEN ("_SYNC_", true);

// type aliases for better readability.
using t_prodAlt = vector<ParsingToken>;
using t_prodRule = unordered_set<t_prodAlt, ParsingTokenVectorHash>;
using t_grammar = unordered_map<ParsingToken, t_prodRule, ParsingTokenHash>;

using t_parsingTokenSet = unordered_set<ParsingToken, ParsingTokenHash>; // To be used first set and follow set for each non-terminal
using t_parsingTokenSetMap = unordered_map<ParsingToken, t_parsingTokenSet, ParsingTokenHash>;

#endif //LEXICAL_PARSER_BUILDER_PARSINGTYPES_H
