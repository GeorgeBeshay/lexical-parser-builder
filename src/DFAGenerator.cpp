#include "DFAGenerator.h"

DFAGenerator::DFAGenerator(
        const unordered_map<State, unordered_map<Symbol, unordered_set<State>>> &NFATransMap,
        const unordered_map<State, unordered_set<State>> &NFAEpsilonTransMap,
        const unordered_set<State> &NFAAcceptingStates, const unordered_set<State> &NFAInitialStates
) {
    transMap = {};
    acceptingStates = {};
    initialState = 0;
}

void
DFAGenerator::subsetConstruction(
        const unordered_map<State, unordered_map<Symbol, unordered_set<State>>> &NFATransMap,
        const unordered_map<State, unordered_set<State>> &NFAEpsilonTransMap,
        const unordered_set<State> &NFAAcceptingStates,
        const unordered_set<State> &NFAInitialStates
) {

}

unordered_set<DFAGenerator::State>
DFAGenerator::computeEpsilonClosure(
        const unordered_set<State> &T,
        const unordered_map<State, unordered_set<State>> &NFAEpsilonTransMap
) {
    return {};
}

// getters
unordered_map<DFAGenerator::State, unordered_map<DFAGenerator::Symbol, DFAGenerator::State>>
DFAGenerator::getTransMap() const {
    return transMap;
}

unordered_set<DFAGenerator::State>
DFAGenerator::getAcceptingStates() const {
    return acceptingStates;
}

DFAGenerator::State
DFAGenerator::getInitialState() const {
    return initialState;
}
