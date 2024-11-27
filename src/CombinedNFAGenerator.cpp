#include "CombinedNFAGenerator.h"

NFAGenerator::NFA CombinedNFAGenerator::combineNFAs(const std::vector<NFAGenerator::NFA>& nfas) {
    NFAGenerator::NFA combinedNFA;
    combinedNFA.startState = 0; // New master start state
    int stateOffset = 1;

    for (const auto& nfa : nfas) {
        // Update transitions with offset
        for (const auto& [state, transitions] : nfa.transitions) {
            for (const auto& [symbol, destStates] : transitions) {
                for (int dest : destStates) {
                    combinedNFA.transitions[state + stateOffset][symbol].insert(dest + stateOffset);
                }
            }
        }

        // Update epsilon transitions
        for (const auto& [state, epsilonStates] : nfa.epsilonTransitions) {
            for (int epsilonState : epsilonStates) {
                combinedNFA.epsilonTransitions[state + stateOffset].insert(epsilonState + stateOffset);
            }
        }

        // Add epsilon transition from master start state to NFA's start state
        combinedNFA.epsilonTransitions[combinedNFA.startState].insert(nfa.startState + stateOffset);

        // Merge symbols
        combinedNFA.symbols.insert(nfa.symbols.begin(), nfa.symbols.end());

        stateOffset += 100; // Ensure unique state IDs for each NFA
    }

    combinedNFA.acceptState = stateOffset; // Set final accept state
    return combinedNFA;
}
