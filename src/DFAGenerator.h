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

    DFAGenerator(
            const unordered_map<State, unordered_map<Symbol, unordered_set<State>>>& NFATransMap,
            const unordered_map<State, unordered_set<State>>& NFAEpsilonTransMap,
            const unordered_set<State>& NFAAcceptingStates,
            const unordered_set<State>& NFAInitialStates
            );

    // getters
    // marked with 'const' to indicate that this method should not modify any of the object fields.
    // Can be safely called on const DFAGenerator objects.
    unordered_map<State, unordered_map<Symbol, State>> getTransMap() const;
    unordered_set<State> getAcceptingStates() const;
    State getInitialState() const;

private:
    unordered_map<State, unordered_map<Symbol, State>> transMap;
    unordered_set<State> acceptingStates;
    State initialState;

    // core algorithm
    void subsetConstruction(
            const unordered_map<State, unordered_map<Symbol, unordered_set<State>>>& NFATransMap,
            const unordered_map<State, unordered_set<State>>& NFAEpsilonTransMap,
            const unordered_set<State>& NFAAcceptingStates,
            const unordered_set<State>& NFAInitialStates
            );

    // T -> e-closure(T). All states reachable on any epsilon-transition on any state s in T.
    static unordered_set<State> computeEpsilonClosure(
            const unordered_set<State>& T,
            const unordered_map<State, unordered_set<State>>& NFAEpsilonTransMap
            );

};


#endif //LEXICAL_PARSER_BUILDER_DFAGENERATOR_H
