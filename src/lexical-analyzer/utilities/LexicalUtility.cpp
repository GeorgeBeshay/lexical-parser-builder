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

    /**
     * Given the source states and the epsilon transitions of the current NFA, this method computes
     * the destination states reached on epsilon transitions from the current source states.
     * @param originalStates
     * @param nfaEpsilonTransMap
     * @return
     */
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

    /**
     * Given a set of source states and a trigger symbol, this method computes the corresponding
     * destination states.
     * @param sourceStates
     * @param symbol_
     * @param nfaTransMap
     * @return
     */
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

    /**
     * Generates a .svg graph plot corresponding to the DFA (with no dead states).
     * @param initialState
     * @param symbols
     * @param transMap
     * @param acceptingStates
     * @param numberOfStates
     * @param imgFileNameWithExtension
     * @return
     */
    bool visualizeDfa(
        state initialState,
        const unordered_set<symbol>& symbols,
        const unordered_map<state, unordered_map<symbol, state>>& transMap,
        const unordered_map<state, clazz>& acceptingStates,
        const int& numberOfStates,
        const string& imgFileNameWithExtension
        ) {

        const string tempDotFileName = "dfa_temp_plot.dot";

        {
            // in case the file was already there, remove it first.
            remove(tempDotFileName.c_str());

            // filling the .dot file.
            ofstream dotFile(tempDotFileName);
            if (!dotFile.is_open()) {
                cerr << "Something had went wrong when opening the .dot file." << endl;
                return false;
            }

            // Global properties
            dotFile << "digraph DeterministicFiniteAutomaton {\n";
            dotFile << "    beautify=true";
            dotFile << "    rankdir=LR;\n"; // Left-to-right layout

            // All states
            dotFile << "    node [shape = circle];\n";
            for (auto &outgoingTransitions: transMap) {
                for (auto &outgoingTransition: outgoingTransitions.second) {
                    dotFile << "    " << outgoingTransitions.first << " -> " << outgoingTransition.second
                            << " [label=\"" << outgoingTransition.first << "\"];\n";
                }
            }

            // Accepting states
            for (auto& acceptingState: acceptingStates) {
                dotFile << " " << acceptingState.first << " [shape=doublecircle, color=green, style=filled];\n";
            }

            // Non-accepting states
            for (state s=0; s < numberOfStates; s++) {
                if (acceptingStates.count(s) != 0) {
                    continue;
                }
                dotFile << " " << s << " [shape=circle, color=purple, style=filled];\n";
            }

            // Initial state
            dotFile << "    " << initialState << " [color=red, style=filled];\n";
            dotFile << "    start [shape=none, label=\"\"];\n";
            dotFile << "    start -> " << initialState << ";\n";

            dotFile << "}\n";
            dotFile.close();
        }

        {
            string command = "dot -Tsvg " + tempDotFileName + " -o " + imgFileNameWithExtension;
            if (system(command.c_str()) != 0) {
                cerr << "Graphviz failed to generate the image." << endl;
                return false;
            }
        }

        {
            // delete the temp .dot file created.
            if (remove(tempDotFileName.c_str()) != 0) {
                cerr << "Failed to delete temporary DOT file." << endl;
                return false;
            }
        }

        return true;
    }

    /**
     * Generates a .svg graph plot corresponding to the NFA.
     * @param initialStates
     * @param symbols
     * @param transMap
     * @param epsilonTransitions
     * @param acceptingStates
     * @param numberOfStates
     * @param imgFileNameWithExtension
     * @return
     */
    bool visualizeNfa(
        const unordered_set<state>& initialStates,
        const unordered_set<symbol>& symbols,
        const unordered_map<state, unordered_map<symbol, unordered_set<state>>>& transMap,
        const unordered_map<state, unordered_set<state>>& epsilonTransitions,
        const unordered_map<state, clazz>& acceptingStates,
        const int& numberOfStates,
        const string& imgFileNameWithExtension
    ) {

        const string tempDotFileName = "nfa_temp_plot.dot";

        {
            // in case the file was already there, remove it first.
            remove(tempDotFileName.c_str());

            // filling the .dot file.
            ofstream dotFile(tempDotFileName);
            if (!dotFile.is_open()) {
                cerr << "Something had went wrong when opening the .dot file." << endl;
                return false;
            }

            // Global properties
            dotFile << "digraph DeterministicFiniteAutomaton {\n";
            dotFile << "    beautify=true";
            dotFile << "    rankdir=LR;\n"; // Left-to-right layout

            // All regular transitions
            dotFile << "    node [shape = circle];\n";
            for (auto &outgoingTransitions: transMap) {
                for (auto &outgoingTransition: outgoingTransitions.second) {
                    for (auto &destinationState: outgoingTransition.second) {
                        dotFile << "    " << outgoingTransitions.first << " -> " << destinationState
                                << " [label=\"" << outgoingTransition.first << "\"];\n";
                    }
                }
            }

            // Epsilon transitions (labeled as ε)
            dotFile << "    node [shape = circle, color=lightgray];\n"; // Optional color for epsilon transitions
            for (auto& epsilonTransition : epsilonTransitions) {
                for (auto& destState : epsilonTransition.second) {
                    dotFile << "    " << epsilonTransition.first << " -> " << destState
                            << " [label=\"ε\", color=blue];\n"; // ε transition in blue or any color you prefer
                }
            }

            // Accepting states
            for (auto& acceptingState: acceptingStates) {
                dotFile << " " << acceptingState.first << " [shape=doublecircle, color=green, style=filled];\n";
            }

            // Non-accepting states
            for (state s=0; s < numberOfStates; s++) {
                if (acceptingStates.count(s) != 0) {
                    continue;
                }
                dotFile << " " << s << " [shape=circle, color=purple, style=filled];\n";
            }

            // Initial state
            for (auto& tempInitState: initialStates) {
                dotFile << "    " << tempInitState << " [color=red, style=filled];\n";
                dotFile << "    start [shape=none, label=\"\"];\n";
                dotFile << "    start -> " << tempInitState << ";\n";
            }

            dotFile << "}\n";
            dotFile.close();
        }

        {
            string command = "dot -Tsvg " + tempDotFileName + " -o " + imgFileNameWithExtension;
            if (system(command.c_str()) != 0) {
                cerr << "Graphviz failed to generate the image." << endl;
                return false;
            }
        }

        {
            // delete the temp .dot file created.
            if (remove(tempDotFileName.c_str()) != 0) {
                cerr << "Failed to delete temporary DOT file." << endl;
                return false;
            }
        }

        return true;
    }

}