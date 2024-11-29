#include "LexicalUtility.h"

namespace LexicalUtility {

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

    statesPartition partitionGroup(
        const group& grp,
        const statesPartition& parentPartition,
        const unordered_map<state, unordered_map<symbol, state>>& transMap
        ) {
        statesPartition childPartition;
        return childPartition;
    }

}