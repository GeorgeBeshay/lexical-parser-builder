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
    languageSymbols = symbols;
    subsetConstruction(nfaTransMap, nfaEpsilonTransMap, nfaAcceptingStates, nfaInitialStates, symbols);
    minimizeDfa();
    compactDfa();
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

            if (U_.empty()) {
                continue;   // to prevent creating a rejecting state.
            }

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
    statesPartition newPartition;
    while (true) {
        newPartition.clear();

        for (auto& grp: currentPartition) {
            statesPartition tempGrpPartition = LexicalUtility::partitionGroup(
                grp,
                currentPartition,
                this->transMap,
                this->languageSymbols
                );

            for (auto& subgroup: tempGrpPartition) {
                newPartition.push_back(subgroup);
            }
        }

        if (LexicalUtility::areEqualPartitions(currentPartition, newPartition)) {
            break;      // the final partition is currentPartition.
        } else {
            currentPartition = newPartition;
        }
    }

    unordered_map<state, state> representativeStatesMapper;
    unordered_set<state> representativeStates;      // will be the states of the new DFA.
    unordered_map<state, clazz> newAcceptingStates;
    unordered_map<state, unordered_map<symbol, state>> newTransMap;

    for (auto& grp: currentPartition) {
        if (grp.empty()) {
            throw runtime_error("Partition groups should never be empty.");
        }

        state grpRepresentative = *(grp.begin());
        representativeStates.insert(grpRepresentative);
        for (auto& grpElement: grp) {
            representativeStatesMapper[grpElement] = grpRepresentative;
            if (this->acceptingStates.find(grpElement) != this->acceptingStates.end()) {
                newAcceptingStates[grpRepresentative] = this->acceptingStates[grpElement];
            }
        }
    }

    // map the existing transMap to a new one based on the representative states.
    for (auto& outerMapping: transMap) {
        if (representativeStates.count(outerMapping.first) != 0) {
            unordered_map<symbol, state> newInnerMapping;
            for (auto& innerMapping: outerMapping.second) {
                newInnerMapping[innerMapping.first] = representativeStatesMapper[innerMapping.second];
            }
            newTransMap[outerMapping.first] = newInnerMapping;
        }
    }

    this->numberOfStates = (int)representativeStates.size();
    this->initialState = representativeStatesMapper[this->initialState];
    this->acceptingStates = std::move(newAcceptingStates);
    this->transMap = newTransMap;

}

statesPartition
DFAGenerator::getInitialPartition() {

    // the partition will be on the form: {G_accepting_c1, G_accepting_c2, ..., G_accepting_cN, G_rejecting}
    size_t initialPartitionSize = (
        (this->numberOfStates == this->acceptingStates.size())
        ? this->acceptingStates.size()
        : (this->acceptingStates.size()+1)
        );
    statesPartition p(initialPartitionSize);
    unordered_map<clazz, int> acceptingClassToPartitionGrpMapper;

    for (state i = 0; i < this->numberOfStates; i++) {
        if (this->acceptingStates.count(i) != 0) {      // 'i' is an accepting state
            clazz acceptedClass = this->acceptingStates[i];
            if (acceptingClassToPartitionGrpMapper.count(acceptedClass) == 0) {
                acceptingClassToPartitionGrpMapper[acceptedClass] = (int) acceptingClassToPartitionGrpMapper.size();
            }
            p[acceptingClassToPartitionGrpMapper[acceptedClass]].insert(i);
        } else {
            p[initialPartitionSize - 1].insert(i);
        }
    }

    return p;
}

/**
 * re-define each state and its transition to start counting from 0.
 * This is because the previous approach would result in a scattered states ids like having the states: {2, 5, 8, 3}
 * instead of {0, 1, 2, 3}.
 */
void
DFAGenerator::compactDfa() {
    unordered_map<state, state> newStateIds{{this->initialState, 0}};

    state nextStateId = 1;
    for (auto& transition: transMap) {
        if (newStateIds.count(transition.first) == 0) {
            newStateIds[transition.first] = nextStateId++;
        }
    }

    unordered_map<state, clazz> compactAcceptingStates;
    for (auto& acceptingStateMapping: this->acceptingStates) {
        compactAcceptingStates[newStateIds[acceptingStateMapping.first]] = acceptingStateMapping.second;
    }

    unordered_map<state, unordered_map<symbol, state>> compactTransMap;
    for (auto& transitionMapping: this->transMap) {
        unordered_map<symbol, state> tempCompactInnerMapping;
        for (auto& innerMapping: transitionMapping.second) {
            tempCompactInnerMapping[innerMapping.first] = newStateIds[innerMapping.second];
        }
        compactTransMap[newStateIds[transitionMapping.first]] = tempCompactInnerMapping;
    }

    this->initialState = newStateIds[this->initialState];
    this->acceptingStates = compactAcceptingStates;
    this->transMap = compactTransMap;
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

unordered_set<symbol>
DFAGenerator::getLanguageSymbols() const {
    return languageSymbols;
}

state
DFAGenerator::getInitialState() const {
    return initialState;
}

int
DFAGenerator::getNumberOfStates() const {
    return numberOfStates;
}
