#ifndef LEXICAL_PARSER_BUILDER_TRANSITIONTABLE_H
#define LEXICAL_PARSER_BUILDER_TRANSITIONTABLE_H

#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../dfa/DFAGenerator.h"
#include "lexicalTypes.h"

using namespace std;

class TransitionTableContainer {
public:

    TransitionTableContainer(string& filePath) {
        if(!readFile(filePath)) {
            throw runtime_error("There is a problem in reading the transition table file: " + filePath);
        }
    }

    TransitionTableContainer(
        unordered_map<t_state, unordered_map<t_symbol, t_state>> transMap,
        unordered_map<t_state, t_clazz> acceptingStates,
        t_state initialState,
        int numberOfStates,
        unordered_set<t_symbol> symbols
            );

    bool writeFile(string& filePath);

    vector<vector<int>>& getTransitionTable() { return transitionTable; }

    unordered_map<t_symbol, int>& getSymbolToIndexMapper() { return symbolToIndexMapper; }

    unordered_map<t_state, t_clazz>& getAcceptingStates() { return acceptingStates; }

    t_state getInitialState() { return initialState; }

private:
    vector<vector<int>> transitionTable;
    unordered_map<t_symbol, int> symbolToIndexMapper;
    unordered_map<t_state, t_clazz> acceptingStates;
    t_state initialState;

    bool readFile(string& filePath);
};


#endif //LEXICAL_PARSER_BUILDER_TRANSITIONTABLE_H
