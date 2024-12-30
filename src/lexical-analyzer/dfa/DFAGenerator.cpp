#include "DFAGenerator.h"

DFAGenerator::DFAGenerator(
        const unordered_map<t_state, unordered_map<t_symbol, unordered_set<t_state>>> &nfaTransMap,
        const unordered_map<t_state, unordered_set<t_state>> &nfaEpsilonTransMap,
        const unordered_map<t_state, t_clazz> &nfaAcceptingStates,
        const unordered_set<t_state> &nfaInitialStates,
        const unordered_set<t_symbol> &symbols
) {
    transMap = {};
    acceptingStates = {};
    initialState = 0;   // the initial DFA t_state is s0.
    numberOfStates = 1; // any DFA at least has 1 t_state.
    languageSymbols = symbols;
    subsetConstruction(nfaTransMap, nfaEpsilonTransMap, nfaAcceptingStates, nfaInitialStates, symbols);
    minimizeDfa();
    compactDfa();
}

void
DFAGenerator::subsetConstruction(
        const unordered_map<t_state, unordered_map<t_symbol, unordered_set<t_state>>> &nfaTransMap,
        const unordered_map<t_state, unordered_set<t_state>> &nfaEpsilonTransMap,
        const unordered_map<t_state, t_clazz> &nfaAcceptingStates,
        const unordered_set<t_state> &nfaInitialStates,
        const unordered_set<t_symbol> &symbols
) {

    map<set<t_state>, t_state> nfaToDfaMapper;
    unordered_map<t_state, set<t_state>> dfaToNfaMapper;

    // e-closure(s0)
    unordered_set<t_state> unorderedInitialStatesEpsilonClosure = LexicalUtility::computeEpsilonClosure(nfaInitialStates, nfaEpsilonTransMap);
    set<t_state> initialStatesEpsilonClosure = LexicalUtility::unorderedSetToOrderedSet(unorderedInitialStatesEpsilonClosure);
    nfaToDfaMapper[initialStatesEpsilonClosure] = this->initialState;
    dfaToNfaMapper[this->initialState] = initialStatesEpsilonClosure;

    stack<t_state> unmarkedStates;        // contains the DFA unmarked states.
    unmarkedStates.push(this->initialState);    // initially, the s0 t_state is only unmarked.
    unordered_set<t_state> markedStates;

    while (!unmarkedStates.empty()) {

        t_state tempDfaState = unmarkedStates.top();
        unmarkedStates.pop();
        if (markedStates.count(tempDfaState) != 0) continue;
        markedStates.insert(tempDfaState);

        for (t_symbol a: symbols) {

            unordered_set<t_state> U = LexicalUtility::computeEpsilonClosure(
                LexicalUtility::makeNfaTransition(dfaToNfaMapper[tempDfaState], a, nfaTransMap),
                nfaEpsilonTransMap
                );
            set<t_state> U_ = LexicalUtility::unorderedSetToOrderedSet(U);

            if (U_.empty()) {
                continue;   // to prevent creating a rejecting t_state.
            }

            if (nfaToDfaMapper.count(U_) == 0) {
                t_state newState = (this->numberOfStates)++;
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
        const unordered_map<t_state, set<t_state>>& dfaToNfaMapper,
        const unordered_map<t_state, t_clazz>& nfaAcceptingStates
) {
    for (const auto& mapping: dfaToNfaMapper) {
        t_clazz correspondingAcceptedClass;
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

void
DFAGenerator::minimizeDfa() {

    t_statesPartition currentPartition = getInitialPartition();
    t_statesPartition newPartition;
    while (true) {
        newPartition.clear();

        for (auto& grp: currentPartition) {
            t_statesPartition tempGrpPartition = LexicalUtility::partitionGroup(
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

    unordered_map<t_state, t_state> representativeStatesMapper;
    unordered_set<t_state> representativeStates;      // will be the states of the new DFA.
    unordered_map<t_state, t_clazz> newAcceptingStates;
    unordered_map<t_state, unordered_map<t_symbol, t_state>> newTransMap;

    for (auto& grp: currentPartition) {
        if (grp.empty()) {
            throw runtime_error("Partition groups should never be empty.");
        }

        t_state grpRepresentative = *(grp.begin());
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
            unordered_map<t_symbol, t_state> newInnerMapping;
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

t_statesPartition
DFAGenerator::getInitialPartition() {

    // the partition will be on the form: {G_accepting_c1, G_accepting_c2, ..., G_accepting_cN, G_rejecting}
    size_t initialPartitionSize = (
        (this->numberOfStates == this->acceptingStates.size())
        ? this->acceptingStates.size()
        : (this->acceptingStates.size()+1)
        );
    t_statesPartition p(initialPartitionSize);
    unordered_map<t_clazz, int> acceptingClassToPartitionGrpMapper;

    for (t_state i = 0; i < this->numberOfStates; i++) {
        if (this->acceptingStates.count(i) != 0) {      // 'i' is an accepting t_state
            t_clazz acceptedClass = this->acceptingStates[i];
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
 * re-define each t_state and its transition to start counting from 0.
 * This is because the previous approach would result in a scattered states ids like having the states: {2, 5, 8, 3}
 * instead of {0, 1, 2, 3}.
 */
void
DFAGenerator::compactDfa() {
    unordered_map<t_state, t_state> newStateIds{{this->initialState, 0}};

    t_state nextStateId = 1;
    for (auto& transition: transMap) {
        if (newStateIds.count(transition.first) == 0) {        // exploring states that have an outgoing transition.
            newStateIds[transition.first] = nextStateId++;
        }
        for (auto& innerTransition: transition.second) {
            if (newStateIds.count(innerTransition.second) == 0) {   // exploring states that have an incoming transition.
                newStateIds[innerTransition.second] = nextStateId++;
            }
        }
    }

    unordered_map<t_state, t_clazz> compactAcceptingStates;
    for (auto& acceptingStateMapping: this->acceptingStates) {
        compactAcceptingStates[newStateIds[acceptingStateMapping.first]] = acceptingStateMapping.second;
    }

    unordered_map<t_state, unordered_map<t_symbol, t_state>> compactTransMap;
    for (auto& transitionMapping: this->transMap) {
        unordered_map<t_symbol, t_state> tempCompactInnerMapping;
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
unordered_map<t_state, unordered_map<t_symbol , t_state>>
DFAGenerator::getTransMap() const {
    return transMap;
}

unordered_map<t_state , t_clazz>
DFAGenerator::getAcceptingStates() const {
    return acceptingStates;
}

unordered_set<t_symbol>
DFAGenerator::getLanguageSymbols() const {
    return languageSymbols;
}

t_state
DFAGenerator::getInitialState() const {
    return initialState;
}

int
DFAGenerator::getNumberOfStates() const {
    return numberOfStates;
}
