#ifndef LEXICAL_PARSER_BUILDER_TYPES_H
#define LEXICAL_PARSER_BUILDER_TYPES_H

#include <unordered_set>
#include <string>
#include <vector>

using namespace std;

#define REJECTING_STATE (-1)

// type aliases for better readability.
using state = int;
using symbol = char;
using clazz = string;
using group = unordered_set<state>;
using statesPartition = vector<group>;

#endif //LEXICAL_PARSER_BUILDER_TYPES_H