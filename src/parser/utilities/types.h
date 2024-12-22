#ifndef LEXICAL_PARSER_BUILDER_TYPES_H
#define LEXICAL_PARSER_BUILDER_TYPES_H

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "ParsingToken.h"

using namespace std;

// type aliases for better readability.
using t_productionAlternative = vector<ParsingToken>;
using t_productionRule = unordered_set<t_productionAlternative, ParsingTokenVectorHash>;
using t_grammar = unordered_map<ParsingToken, t_productionRule, ParsingTokenHash>;

#endif //LEXICAL_PARSER_BUILDER_TYPES_H
