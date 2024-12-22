#ifndef LEXICAL_PARSER_BUILDER_TYPES_H
#define LEXICAL_PARSER_BUILDER_TYPES_H

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "ParsingToken.h"

using namespace std;

// type aliases for better readability.
using production_alternative = vector<ParsingToken>;
using production_rule = unordered_set<production_alternative, ParsingTokenVectorHash>;
using grammar = unordered_map<ParsingToken, production_rule, ParsingTokenHash>;

#endif //LEXICAL_PARSER_BUILDER_TYPES_H
