#ifndef LEXICAL_PARSER_BUILDER_DFAGENERATOR_H
#define LEXICAL_PARSER_BUILDER_DFAGENERATOR_H

#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <stack>
#include <iostream>
#include "types.h"
#include "LexicalUtility.h"

using namespace std;

class DFAGenerator {

public:

    DFAGenerator(
            const unordered_map<state, unordered_map<symbol, unordered_set<state>>>& nfaTransMap,
            const unordered_map<state, unordered_set<state>>& nfaEpsilonTransMap,
            const unordered_map<state, clazz>& nfaAcceptingStates,
            const unordered_set<state>& nfaInitialStates,
            const unordered_set<symbol>& symbols
            );

    // getters
    // marked with 'const' to indicate that this method should not modify any of the object fields.
    // Can be safely called on const DFAGenerator objects.
    unordered_map<state, unordered_map<symbol, state>> getTransMap() const;
    unordered_map<state, clazz> getAcceptingStates() const;
    state getInitialState() const;
    int getNumberOfStates() const;

    // originalStates -> e-closure(originalStates). All states reachable on any epsilon-transition on any state s in originalStates.
    static unordered_set<state> computeEpsilonClosure(
        const unordered_set<state>& originalStates,
        const unordered_map<state, unordered_set<state>>& nfaEpsilonTransMap
    );

    static unordered_set<state> moveNfa(
        const set<state>& T,
        symbol a,
        const unordered_map<state, unordered_map<symbol, unordered_set<state>>>& nfaTransMap
    );

private:
    unordered_map<state, unordered_map<symbol, state>> transMap;
    unordered_map<state, clazz> acceptingStates;
    state initialState;
    int numberOfStates;

    // core algorithm
    void subsetConstruction(
            const unordered_map<state, unordered_map<symbol, unordered_set<state>>>& nfaTransMap,
            const unordered_map<state, unordered_set<state>>& nfaEpsilonTransMap,
            const unordered_map<state, clazz>& nfaAcceptingStates,
            const unordered_set<state>& nfaInitialStates,
            const unordered_set<symbol>& symbols
            );

    void computeAcceptingDfaStates(
            const unordered_map<state, set<state>>& dfaToNfaMapper,
            const unordered_map<state, clazz>& nfaAcceptingStates
            );

    void minimizeDfa();

    statesPartition getInitialPartition();

};


#endif //LEXICAL_PARSER_BUILDER_DFAGENERATOR_H