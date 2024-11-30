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

}