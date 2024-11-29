#ifndef LEXICAL_PARSER_BUILDER_DFAGENERATOR_H
#define LEXICAL_PARSER_BUILDER_DFAGENERATOR_H

#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <stack>
#include <iostream>

using namespace std;

class DFAGenerator {

public:
    // type aliases for better readability.
    using state = int;
    using symbol = char;
    using clazz = string;

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

    template <typename T>
    static set<T> unorderedSetToOrderedSet(const unordered_set<T>& unorderedSet) {
        return set<T>(unorderedSet.begin(), unorderedSet.end());
    }

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
            const unordered_map<DFAGenerator::state, set<DFAGenerator::state>>& dfaToNfaMapper,
            const unordered_map<state, clazz>& nfaAcceptingStates
            );

};


#endif //LEXICAL_PARSER_BUILDER_DFAGENERATOR_H
