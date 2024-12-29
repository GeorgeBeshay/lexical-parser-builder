#ifndef LEXICAL_PARSER_BUILDER_LEXICALTYPES_H
#define LEXICAL_PARSER_BUILDER_LEXICALTYPES_H

#include <unordered_set>
#include <string>
#include <vector>

using namespace std;

#define REJECTING_STATE (-1)
#define IDENTIFIER_CLASS "id"

// type aliases for better readability.
using t_state = int;
using t_symbol = char;
using t_clazz = string;
using t_group = unordered_set<t_state>;
using t_statesPartition = vector<t_group>;
using t_lexem = string;

#endif //LEXICAL_PARSER_BUILDER_LEXICALTYPES_H
