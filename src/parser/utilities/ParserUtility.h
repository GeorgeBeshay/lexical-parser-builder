#ifndef LEXICAL_PARSER_BUILDER_PARSERUTILITY_H
#define LEXICAL_PARSER_BUILDER_PARSERUTILITY_H

#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <iostream>

#include "types.h"
#include "ParsingToken.h"

using namespace std;

namespace ParserUtility {
    t_parsingTokenSetMap computeFirstSets(t_grammar grammar);

    t_parsingTokenSetMap computeFollowSets(t_grammar grammar,
                                           ParsingToken startSymbol,
                                           t_parsingTokenSetMap firstSetsMap);
};


#endif //LEXICAL_PARSER_BUILDER_PARSERUTILITY_H
