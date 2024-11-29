#include "DFAGenerator.h"

DFAGenerator::DFAGenerator(
        const unordered_map<State, unordered_map<Symbol, unordered_set<State>>> &NFATransMap,
        const unordered_map<State, unordered_set<State>> &NFAEpsilonTransMap,
        const unordered_map<State, Class> &NFAAcceptingStates,
        const unordered_set<State> &NFAInitialStates,
        const unordered_set<Symbol> &symbols
) {
    transMap = {};
    acceptingStates = {};
    initialState = 0;   // the initial DFA state is s0.
    numberOfStates = 1; // any DFA at least has 1 state.
    subsetConstruction(NFATransMap, NFAEpsilonTransMap, NFAAcceptingStates, NFAInitialStates, symbols);
}

void
DFAGenerator::subsetConstruction(
        const unordered_map<State, unordered_map<Symbol, unordered_set<State>>> &NFATransMap,
        const unordered_map<State, unordered_set<State>> &NFAEpsilonTransMap,
        const unordered_map<State, Class> &NFAAcceptingStates,
        const unordered_set<State> &NFAInitialStates,
        const unordered_set<Symbol> &symbols
) {

    map<set<State>, State> NFAToDFAMapper;
    unordered_map<State, set<State>> DFAToNFAMapper;

    // e-closure(s0)
    unordered_set<State> unorderedInitialStatesEpsilonClosure = computeEpsilonClosure(NFAInitialStates, NFAEpsilonTransMap);
    set<State> initialStatesEpsilonClosure = unorderedSetToOrderedSet(unorderedInitialStatesEpsilonClosure);
    NFAToDFAMapper[initialStatesEpsilonClosure] = this->initialState;
    DFAToNFAMapper[this->initialState] = initialStatesEpsilonClosure;

    stack<State> unmarkedStates;        // contains the DFA unmarked states.
    unmarkedStates.push(this->initialState);    // initially, the s0 state is only unmarked.
    unordered_set<State> markedStates;

    while (!unmarkedStates.empty()) {

        State tempDFAState = unmarkedStates.top();
        unmarkedStates.pop();
        if (markedStates.count(tempDFAState) != 0) continue;
        markedStates.insert(tempDFAState);

        for (Symbol a: symbols) {

            unordered_set<State> U = computeEpsilonClosure(moveNFA(DFAToNFAMapper[tempDFAState], a, NFATransMap),
                                                           NFAEpsilonTransMap);
            set<State> U_ = unorderedSetToOrderedSet(U);

            if (NFAToDFAMapper.count(U_) == 0) {
                State newState = (this->numberOfStates)++;
                NFAToDFAMapper[U_] = newState;
                DFAToNFAMapper[newState] = U_;
                unmarkedStates.push(newState);
            }

            this->transMap[tempDFAState][a] = NFAToDFAMapper[U_];

        }

    }

    computeAcceptingDFAStates(DFAToNFAMapper, NFAAcceptingStates);

}

void
DFAGenerator::computeAcceptingDFAStates(
        const unordered_map<DFAGenerator::State, set<DFAGenerator::State>>& DFAToNFAMapper,
        const unordered_map<State, Class>& NFAAcceptingStates
) {
    for (const auto& mapping: DFAToNFAMapper) {
        Class correspondingAcceptedClass;
        for (auto tempNFAState: mapping.second) {
            if (NFAAcceptingStates.find(tempNFAState) != NFAAcceptingStates.end()) {
                correspondingAcceptedClass = NFAAcceptingStates.at(tempNFAState);
                break;
            }
        }
        if (!correspondingAcceptedClass.empty()) {         // if not "", then it is accepting, and we know its class.
            this->acceptingStates[mapping.first] = correspondingAcceptedClass;
        }
    }
}

unordered_set<DFAGenerator::State>
DFAGenerator::computeEpsilonClosure(
        const unordered_set<State> &T,
        const unordered_map<State, unordered_set<State>> &NFAEpsilonTransMap
) {
    unordered_set<State> epsilonClosure;
    stack<State> remainingStates;

    // pushing all states into the stack.
    for (auto t: T) {
        remainingStates.push(t);
        epsilonClosure.insert(t);
    }

    while (!remainingStates.empty()) {
        State tempState = remainingStates.top();
        remainingStates.pop();

        if (NFAEpsilonTransMap.find(tempState) == NFAEpsilonTransMap.end()) {
            continue;
        }

        // for all states u reachable by an epsilon transition from tempState.
        for (auto u: NFAEpsilonTransMap.at(tempState)) {
            if (epsilonClosure.count(u) == 0) {
                epsilonClosure.insert(u);
                remainingStates.push(u);
            }
        }
    }

    return epsilonClosure;
}

unordered_set<DFAGenerator::State>
DFAGenerator::moveNFA(
        const set<State> &T,
        DFAGenerator::Symbol a,
        const unordered_map<State, unordered_map<Symbol, unordered_set<State>>> &NFATransMap
) {

    unordered_set<State> destinations;
    for (auto t: T) {
        if (
            NFATransMap.find(t) == NFATransMap.end() ||
            NFATransMap.at(t).find(a) == NFATransMap.at(t).end()
        ) {
            continue;
        }
        unordered_set<State> tempDestinations = NFATransMap.at(t).at(a);
        destinations.insert(tempDestinations.begin(), tempDestinations.end());
    }

    return destinations;
}

// getters
unordered_map<DFAGenerator::State, unordered_map<DFAGenerator::Symbol, DFAGenerator::State>>
DFAGenerator::getTransMap() const {
    return transMap;
}

unordered_map<DFAGenerator::State, DFAGenerator::Class>
DFAGenerator::getAcceptingStates() const {
    return acceptingStates;
}

DFAGenerator::State
DFAGenerator::getInitialState() const {
    return initialState;
}

int
DFAGenerator::getNumberOfStates() const {
    return numberOfStates;
}
