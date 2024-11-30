#include "LexicalUtility.h"

namespace LexicalUtility {

    /**
     * Return true if partition p1 and p2 are equal.
     * @param p1
     * @param p2
     * @return
     */
    bool areEqualPartitions(const statesPartition& p1, const statesPartition& p2) {
        if (p1.size() != p2.size()) {
            return false;
        }

        for (size_t i = 0; i < p1.size(); i++) {
            if (p1[i] != p2[i]) {
                return false;
            }
        }

        return true;
    }

    /**
     * Any two states s and t should be in the same group iff they have transitions on all input symbols to states within the same group.
     * @param grp
     * @param parentPartition
     * @param transMap
     * @param languageSymbols
     * @return
     */
    statesPartition partitionGroup(
        const group& grp,
        const statesPartition& parentPartition,
        const unordered_map<state, unordered_map<symbol, state>>& transMap,
        const unordered_set<symbol>& languageSymbols
        ) {
        statesPartition childPartition;

        map<vector<state>, unordered_set<state>> destinationStatesToGrpMapper;
        map<state, size_t> stateToGrpIdxMapper;

        for (size_t idx = 0; idx < parentPartition.size() ; idx++) {
            for (auto& tempState: parentPartition[idx]) {
                stateToGrpIdxMapper[tempState] = idx;
            }
        }

        for (auto& tempState: grp) {
            vector<state> tempDestinationStates(languageSymbols.size());
            size_t currentSymbolIdx = 0;
            for (auto& symbol: languageSymbols) {
                if (
                    (transMap.count(tempState) == 0) ||
                    (transMap.at(tempState).count(symbol) == 0)
                    ) {
                    // in case of having no transitions from this state or on this symbol.
                    tempDestinationStates[currentSymbolIdx++] = REJECTING_STATE;
                } else {
                    tempDestinationStates[currentSymbolIdx++] = (state) stateToGrpIdxMapper[transMap.at(tempState).at(symbol)];
                }
            }
            // add the current state to its corresponding subgroup.
            destinationStatesToGrpMapper[tempDestinationStates].insert(tempState);
        }

        for (auto& tempGrp: destinationStatesToGrpMapper) {
            childPartition.push_back(tempGrp.second);
        }

        return childPartition;
    }

    unordered_set<state> computeEpsilonClosure(
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

    unordered_set<state> makeNfaTransition(
        const set<state> &sourceStates,
        symbol symbol_,
        const unordered_map<state, unordered_map<symbol, unordered_set<state>>> &nfaTransMap
    ) {
        unordered_set<state> destinations;
        for (auto t: sourceStates) {
            if (
                nfaTransMap.find(t) == nfaTransMap.end() ||
                    nfaTransMap.at(t).find(symbol_) == nfaTransMap.at(t).end()
                ) {
                continue;
            }
            unordered_set<state> tempDestinations = nfaTransMap.at(t).at(symbol_);
            destinations.insert(tempDestinations.begin(), tempDestinations.end());
        }

        return destinations;
    }

}