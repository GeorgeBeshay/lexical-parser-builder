#ifndef LEXICAL_PARSER_BUILDER_DFAGENERATOR_H
#define LEXICAL_PARSER_BUILDER_DFAGENERATOR_H

#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <stack>
#include <iostream>
#include "../utilities/lexicalTypes.h"
#include "../utilities/LexicalUtility.h"

using namespace std;

class DFAGenerator {

public:

    DFAGenerator(
            const unordered_map<t_state, unordered_map<t_symbol, unordered_set<t_state>>>& nfaTransMap,
            const unordered_map<t_state, unordered_set<t_state>>& nfaEpsilonTransMap,
            const unordered_map<t_state, t_clazz>& nfaAcceptingStates,
            const unordered_set<t_state>& nfaInitialStates,
            const unordered_set<t_symbol>& symbols
            );

    t_statesPartition getInitialPartition();

    /*
     * getters
     * marked with 'const' to indicate that this method should not modify any of the object fields.
     * Can be safely called on const DFAGenerator objects.
     */
    unordered_map<t_state, unordered_map<t_symbol, t_state>> getTransMap() const;
    unordered_map<t_state, t_clazz> getAcceptingStates() const;
    unordered_set<t_symbol> getLanguageSymbols() const;
    t_state getInitialState() const;
    int getNumberOfStates() const;

private:
    unordered_map<t_state, unordered_map<t_symbol, t_state>> transMap;
    unordered_map<t_state, t_clazz> acceptingStates;
    unordered_set<t_symbol> languageSymbols;
    t_state initialState;
    int numberOfStates;

    // core algorithm
    void subsetConstruction(
            const unordered_map<t_state, unordered_map<t_symbol, unordered_set<t_state>>>& nfaTransMap,
            const unordered_map<t_state, unordered_set<t_state>>& nfaEpsilonTransMap,
            const unordered_map<t_state, t_clazz>& nfaAcceptingStates,
            const unordered_set<t_state>& nfaInitialStates,
            const unordered_set<t_symbol>& symbols
            );

    void computeAcceptingDfaStates(
            const unordered_map<t_state, set<t_state>>& dfaToNfaMapper,
            const unordered_map<t_state, t_clazz>& nfaAcceptingStates
            );

    void minimizeDfa();

    void compactDfa();

};


#endif //LEXICAL_PARSER_BUILDER_DFAGENERATOR_H
