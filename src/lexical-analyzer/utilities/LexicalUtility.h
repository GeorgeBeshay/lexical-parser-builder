#ifndef LEXICAL_PARSER_BUILDER_LEXICALUTILITY_H
#define LEXICAL_PARSER_BUILDER_LEXICALUTILITY_H

#include <iostream>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <stack>
#include <fstream>
#include "lexicalTypes.h"

using namespace std;

namespace LexicalUtility {

    bool areEqualPartitions(const t_statesPartition& p1, const t_statesPartition& p2);

    t_statesPartition partitionGroup(
        const t_group& grp,
        const t_statesPartition& parentPartition,
        const unordered_map<t_state, unordered_map<t_symbol, t_state>>& transMap,
        const unordered_set<t_symbol>& languageSymbols
        );

    // originalStates -> e-closure(originalStates). All states reachable on any epsilon-transition on any t_state s in originalStates.
    unordered_set<t_state> computeEpsilonClosure(
        const unordered_set<t_state>& originalStates,
        const unordered_map<t_state, unordered_set<t_state>>& nfaEpsilonTransMap
    );

    unordered_set<t_state> makeNfaTransition(
        const set<t_state>& sourceStates,
        t_symbol symbol_,
        const unordered_map<t_state, unordered_map<t_symbol, unordered_set<t_state>>>& nfaTransMap
    );

    // install Graphviz first https://graphviz.org/download/
    bool visualizeDfa(
        t_state initialState,
        const unordered_set<t_symbol>& symbols,
        const unordered_map<t_state, unordered_map<t_symbol, t_state>>& transMap,
        const unordered_map<t_state, t_clazz>& acceptingStates,
        const int& numberOfStates,
        const string& imgFileNameWithExtension
        );

    // install Graphviz first https://graphviz.org/download/
    bool visualizeNfa(
        const unordered_set<t_state>& initialStates,
        const unordered_set<t_symbol>& symbols,
        const unordered_map<t_state, unordered_map<t_symbol, unordered_set<t_state>>>& transMap,
        const unordered_map<t_state, unordered_set<t_state>>& epsilonTransitions,
        const unordered_map<t_state, t_clazz>& acceptingStates,
        const int& numberOfStates,
        const string& imgFileNameWithExtension
    );

    template <typename T> set<T> unorderedSetToOrderedSet(const unordered_set<T>& unorderedSet) {
        return set<T>(unorderedSet.begin(), unorderedSet.end());
    }

}

#endif //LEXICAL_PARSER_BUILDER_LEXICALUTILITY_H
