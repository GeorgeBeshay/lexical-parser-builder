#include "LexicalUtility.h"

namespace LexicalUtility {

    /**
     * Return true if partition p1 and p2 are equal.
     * @param p1
     * @param p2
     * @return
     */
    bool areEqualPartitions(const t_statesPartition& p1, const t_statesPartition& p2) {
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
     * Any two states s and t should be in the same t_group iff they have transitions on all input symbols to states within the same t_group.
     * @param grp
     * @param parentPartition
     * @param transMap
     * @param languageSymbols
     * @return
     */
    t_statesPartition partitionGroup(
        const t_group& grp,
        const t_statesPartition& parentPartition,
        const unordered_map<t_state, unordered_map<t_symbol, t_state>>& transMap,
        const unordered_set<t_symbol>& languageSymbols
        ) {
        t_statesPartition childPartition;

        map<vector<t_state>, unordered_set<t_state>> destinationStatesToGrpMapper;
        map<t_state, size_t> stateToGrpIdxMapper;

        for (size_t idx = 0; idx < parentPartition.size() ; idx++) {
            for (auto& tempState: parentPartition[idx]) {
                stateToGrpIdxMapper[tempState] = idx;
            }
        }

        for (auto& tempState: grp) {
            vector<t_state> tempDestinationStates(languageSymbols.size());
            size_t currentSymbolIdx = 0;
            for (auto& symbol: languageSymbols) {
                if (
                    (transMap.count(tempState) == 0) ||
                    (transMap.at(tempState).count(symbol) == 0)
                    ) {
                    // in case of having no transitions from this t_state or on this t_symbol.
                    tempDestinationStates[currentSymbolIdx++] = REJECTING_STATE;
                } else {
                    tempDestinationStates[currentSymbolIdx++] = (t_state) stateToGrpIdxMapper[transMap.at(tempState).at(symbol)];
                }
            }
            // add the current t_state to its corresponding subgroup.
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
    unordered_set<t_state> computeEpsilonClosure(
        const unordered_set<t_state> &originalStates,
        const unordered_map<t_state, unordered_set<t_state>> &nfaEpsilonTransMap
    ) {
        unordered_set<t_state> epsilonClosure;
        stack<t_state> remainingStates;

        // pushing all states into the stack.
        for (auto t: originalStates) {
            remainingStates.push(t);
            epsilonClosure.insert(t);
        }

        while (!remainingStates.empty()) {
            t_state tempState = remainingStates.top();
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
     * Given a set of source states and a trigger t_symbol, this method computes the corresponding
     * destination states.
     * @param sourceStates
     * @param symbol_
     * @param nfaTransMap
     * @return
     */
    unordered_set<t_state> makeNfaTransition(
        const set<t_state> &sourceStates,
        t_symbol symbol_,
        const unordered_map<t_state, unordered_map<t_symbol, unordered_set<t_state>>> &nfaTransMap
    ) {
        unordered_set<t_state> destinations;
        for (auto t: sourceStates) {
            if (
                nfaTransMap.find(t) == nfaTransMap.end() ||
                    nfaTransMap.at(t).find(symbol_) == nfaTransMap.at(t).end()
                ) {
                continue;
            }
            unordered_set<t_state> tempDestinations = nfaTransMap.at(t).at(symbol_);
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
        t_state initialState,
        const unordered_set<t_symbol>& symbols,
        const unordered_map<t_state, unordered_map<t_symbol, t_state>>& transMap,
        const unordered_map<t_state, t_clazz>& acceptingStates,
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
            for (t_state s=0; s < numberOfStates; s++) {
                if (acceptingStates.count(s) != 0) {
                    continue;
                }
                dotFile << " " << s << " [shape=circle, color=purple, style=filled];\n";
            }

            // Initial t_state
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
        const unordered_set<t_state>& initialStates,
        const unordered_set<t_symbol>& symbols,
        const unordered_map<t_state, unordered_map<t_symbol, unordered_set<t_state>>>& transMap,
        const unordered_map<t_state, unordered_set<t_state>>& epsilonTransitions,
        const unordered_map<t_state, t_clazz>& acceptingStates,
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
            for (t_state s=0; s < numberOfStates; s++) {
                if (acceptingStates.count(s) != 0) {
                    continue;
                }
                dotFile << " " << s << " [shape=circle, color=purple, style=filled];\n";
            }

            // Initial t_state
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