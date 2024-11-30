#ifndef LEXICAL_PARSER_BUILDER_LEXICALUTILITY_H
#define LEXICAL_PARSER_BUILDER_LEXICALUTILITY_H

#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include "types.h"

using namespace std;

namespace LexicalUtility {

    bool areEqualPartitions(const statesPartition& p1, const statesPartition& p2);
    statesPartition partitionGroup(
        const group& grp,
        const statesPartition& parentPartition,
        const unordered_map<state, unordered_map<symbol, state>>& transMap,
        const unordered_set<symbol>& languageSymbols
        );

    template <typename T> set<T> unorderedSetToOrderedSet(const unordered_set<T>& unorderedSet) {
        return set<T>(unorderedSet.begin(), unorderedSet.end());
    }
}

#endif //LEXICAL_PARSER_BUILDER_LEXICALUTILITY_H
