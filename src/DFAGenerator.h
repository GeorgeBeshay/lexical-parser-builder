#ifndef LEXICAL_PARSER_BUILDER_DFAGENERATOR_H
#define LEXICAL_PARSER_BUILDER_DFAGENERATOR_H

#include <bits/stdc++.h>
#include <iostream>

using namespace std;

class DFAGenerator {

public:
    // type aliases for better readability.
    using State = int;
    using Symbol = char;
    using Class = string;

    DFAGenerator(
            const unordered_map<State, unordered_map<Symbol, unordered_set<State>>>& NFATransMap,
            const unordered_map<State, unordered_set<State>>& NFAEpsilonTransMap,
            const unordered_map<State, Class>& NFAAcceptingStates,
            const unordered_set<State>& NFAInitialStates,
            const unordered_set<Symbol>& symbols
            );

    // getters
    // marked with 'const' to indicate that this method should not modify any of the object fields.
    // Can be safely called on const DFAGenerator objects.
    unordered_map<State, unordered_map<Symbol, State>> getTransMap() const;
    unordered_map<State, Class> getAcceptingStates() const;
    State getInitialState() const;
    int getNumberOfStates() const;

    // T -> e-closure(T). All states reachable on any epsilon-transition on any state s in T.
    static unordered_set<State> computeEpsilonClosure(
        const unordered_set<State>& T,
        const unordered_map<State, unordered_set<State>>& NFAEpsilonTransMap
    );

    static unordered_set<State> moveNFA(
        const set<State>& T,
        Symbol a,
        const unordered_map<State, unordered_map<Symbol, unordered_set<State>>>& NFATransMap
    );

    template <typename T>
    static set<T> unorderedSetToOrderedSet(const unordered_set<T>& unorderedSet) {
        return set<T>(unorderedSet.begin(), unorderedSet.end());
    }

private:
    unordered_map<State, unordered_map<Symbol, State>> transMap;
    unordered_map<State, Class> acceptingStates;
    State initialState;
    int numberOfStates;

    // core algorithm
    void subsetConstruction(
            const unordered_map<State, unordered_map<Symbol, unordered_set<State>>>& NFATransMap,
            const unordered_map<State, unordered_set<State>>& NFAEpsilonTransMap,
            const unordered_map<State, Class>& NFAAcceptingStates,
            const unordered_set<State>& NFAInitialStates,
            const unordered_set<Symbol>& symbols
            );

    void computeAcceptingDFAStates(
            const unordered_map<DFAGenerator::State, set<DFAGenerator::State>>& DFAToNFAMapper,
            const unordered_map<State, Class>& NFAAcceptingStates
            );

};


#endif //LEXICAL_PARSER_BUILDER_DFAGENERATOR_H
