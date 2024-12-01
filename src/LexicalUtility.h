#ifndef LEXICAL_PARSER_BUILDER_LEXICALUTILITY_H
#define LEXICAL_PARSER_BUILDER_LEXICALUTILITY_H

#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <stack>
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

    // originalStates -> e-closure(originalStates). All states reachable on any epsilon-transition on any state s in originalStates.
    unordered_set<state> computeEpsilonClosure(
        const unordered_set<state>& originalStates,
        const unordered_map<state, unordered_set<state>>& nfaEpsilonTransMap
    );

    unordered_set<state> makeNfaTransition(
        const set<state>& sourceStates,
        symbol symbol_,
        const unordered_map<state, unordered_map<symbol, unordered_set<state>>>& nfaTransMap
    );

    template <typename T> set<T> unorderedSetToOrderedSet(const unordered_set<T>& unorderedSet) {
        return set<T>(unorderedSet.begin(), unorderedSet.end());
    }

}

#endif //LEXICAL_PARSER_BUILDER_LEXICALUTILITY_H
