#include "DFAGenerator.h"

DFAGenerator::DFAGenerator(
        const unordered_map<state, unordered_map<symbol, unordered_set<state>>> &nfaTransMap,
        const unordered_map<state, unordered_set<state>> &nfaEpsilonTransMap,
        const unordered_map<state, clazz> &nfaAcceptingStates,
        const unordered_set<state> &nfaInitialStates,
        const unordered_set<symbol> &symbols
) {
    transMap = {};
    acceptingStates = {};
    initialState = 0;   // the initial DFA state is s0.
    numberOfStates = 1; // any DFA at least has 1 state.
    subsetConstruction(nfaTransMap, nfaEpsilonTransMap, nfaAcceptingStates, nfaInitialStates, symbols);
    minimizeDfa();
}

void
DFAGenerator::subsetConstruction(
        const unordered_map<state, unordered_map<symbol, unordered_set<state>>> &nfaTransMap,
        const unordered_map<state, unordered_set<state>> &nfaEpsilonTransMap,
        const unordered_map<state, clazz> &nfaAcceptingStates,
        const unordered_set<state> &nfaInitialStates,
        const unordered_set<symbol> &symbols
) {

    map<set<state>, state> nfaToDfaMapper;
    unordered_map<state, set<state>> dfaToNfaMapper;

    // e-closure(s0)
    unordered_set<state> unorderedInitialStatesEpsilonClosure = computeEpsilonClosure(nfaInitialStates, nfaEpsilonTransMap);
    set<state> initialStatesEpsilonClosure = LexicalUtility::unorderedSetToOrderedSet(unorderedInitialStatesEpsilonClosure);
    nfaToDfaMapper[initialStatesEpsilonClosure] = this->initialState;
    dfaToNfaMapper[this->initialState] = initialStatesEpsilonClosure;

    stack<state> unmarkedStates;        // contains the DFA unmarked states.
    unmarkedStates.push(this->initialState);    // initially, the s0 state is only unmarked.
    unordered_set<state> markedStates;

    while (!unmarkedStates.empty()) {

        state tempDfaState = unmarkedStates.top();
        unmarkedStates.pop();
        if (markedStates.count(tempDfaState) != 0) continue;
        markedStates.insert(tempDfaState);

        for (symbol a: symbols) {

            unordered_set<state> U = computeEpsilonClosure(moveNfa(dfaToNfaMapper[tempDfaState], a, nfaTransMap),
                                                           nfaEpsilonTransMap);
            set<state> U_ = LexicalUtility::unorderedSetToOrderedSet(U);

            if (nfaToDfaMapper.count(U_) == 0) {
                state newState = (this->numberOfStates)++;
                nfaToDfaMapper[U_] = newState;
                dfaToNfaMapper[newState] = U_;
                unmarkedStates.push(newState);
            }

            this->transMap[tempDfaState][a] = nfaToDfaMapper[U_];

        }

    }

    computeAcceptingDfaStates(dfaToNfaMapper, nfaAcceptingStates);

}

void
DFAGenerator::computeAcceptingDfaStates(
        const unordered_map<state, set<state>>& dfaToNfaMapper,
        const unordered_map<state, clazz>& nfaAcceptingStates
) {
    for (const auto& mapping: dfaToNfaMapper) {
        clazz correspondingAcceptedClass;
        for (auto tempNfaState: mapping.second) {
            if (nfaAcceptingStates.find(tempNfaState) != nfaAcceptingStates.end()) {
                correspondingAcceptedClass = nfaAcceptingStates.at(tempNfaState);
                break;
            }
        }
        if (!correspondingAcceptedClass.empty()) {         // if not "", then it is accepting, and we know its class.
            this->acceptingStates[mapping.first] = correspondingAcceptedClass;
        }
    }
}

unordered_set<state>
DFAGenerator::computeEpsilonClosure(
        const unordered_set<state> &originalStates,
        const unordered_map<state, unordered_set<state>> &nfaEpsilonTransMap
) {
    unordered_set<state> epsilonClosure;
    stack<state> remainingStates;

    // pushing all states into the stack.
    for (auto t: originalStates) {
        remainingStates.push(t);
        epsilonClosure.insert(t);
    }

    while (!remainingStates.empty()) {
        state tempState = remainingStates.top();
        remainingStates.pop();

        if (nfaEpsilonTransMap.find(tempState) == nfaEpsilonTransMap.end()) {
            continue;
        }

        // for all states u reachable by an epsilon transition from tempState.
        for (auto u: nfaEpsilonTransMap.at(tempState)) {
            if (epsilonClosure.count(u) == 0) {
                epsilonClosure.insert(u);
                remainingStates.push(u);
            }
        }
    }

    return epsilonClosure;
}

unordered_set<state>
DFAGenerator::moveNfa(
        const set<state> &T,
        symbol a,
        const unordered_map<state, unordered_map<symbol, unordered_set<state>>> &nfaTransMap
) {

    unordered_set<state> destinations;
    for (auto t: T) {
        if (
            nfaTransMap.find(t) == nfaTransMap.end() ||
            nfaTransMap.at(t).find(a) == nfaTransMap.at(t).end()
        ) {
            continue;
        }
        unordered_set<state> tempDestinations = nfaTransMap.at(t).at(a);
        destinations.insert(tempDestinations.begin(), tempDestinations.end());
    }

    return destinations;
}

void
DFAGenerator::minimizeDfa() {
    statesPartition currentPartition = getInitialPartition();
    statesPartition newPartition(currentPartition.size());      // the new partition size will always be >= parent partition size.
    while (true) {

        newPartition.clear();

        for (auto& grp: currentPartition) {
            statesPartition tempGrpPartition = LexicalUtility::partitionGroup(grp,currentPartition, this->transMap);
            newPartition.insert(newPartition.end(), tempGrpPartition.begin(), tempGrpPartition.end());
        }

        if (LexicalUtility::areEqualPartitions(currentPartition, newPartition)) {
            break;
        } else {
            currentPartition = newPartition;
        }
    }

    // the final partition is currentPartition.
    // TODO
    // create the representative states mapping

    // map the new starting state

    // map the accepting states

    // map the transitions from other states.
}

statesPartition
DFAGenerator::getInitialPartition() {

    // the accepting states as the first group
    group acceptingStatesGrp;
    for (auto& mapping: this->acceptingStates) {
        acceptingStatesGrp.insert(mapping.first);
    }

    // the rejecting states are the second group
    group rejectingStatesGrp;
    for (state i = 0; i < this->numberOfStates; i++) {
        if (acceptingStatesGrp.find(i) != acceptingStatesGrp.cend()) {
            rejectingStatesGrp.insert(i);
        }
    }

    statesPartition p {
        acceptingStatesGrp,
        rejectingStatesGrp
    };

    return p;
}

// getters
unordered_map<state, unordered_map<symbol , state>>
DFAGenerator::getTransMap() const {
    return transMap;
}

unordered_map<state , clazz>
DFAGenerator::getAcceptingStates() const {
    return acceptingStates;
}

state
DFAGenerator::getInitialState() const {
    return initialState;
}

int
DFAGenerator::getNumberOfStates() const {
    return numberOfStates;
}
