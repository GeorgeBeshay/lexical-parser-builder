#ifndef LEXICAL_PARSER_BUILDER_TRANSITIONTABLE_H
#define LEXICAL_PARSER_BUILDER_TRANSITIONTABLE_H

#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "DFAGenerator.h"
#include "types.h"

using namespace std;

class TransitionTableContainer {
public:

    TransitionTableContainer(string& filePath) {
        if(!readFile(filePath)) {
            throw runtime_error("There is a problem in reading the transition table file: " + filePath);
        }
    }

    TransitionTableContainer(
            unordered_map<state, unordered_map<symbol, state>> transMap,
            unordered_map<state, clazz> acceptingStates,
            state initialState,
            int numberOfStates,
            unordered_set<symbol> symbols
            );

    bool writeFile(string& filePath);

    vector<vector<int>>& getTransitionTable() { return transitionTable; }

    unordered_map<symbol, int>& getSymbolToIndexMapper() { return symbolToIndexMapper; }

    unordered_map<state, clazz>& getAcceptingStates() { return acceptingStates; }

    state getInitialState() { return initialState; }

private:
    vector<vector<int>> transitionTable;
    unordered_map<symbol, int> symbolToIndexMapper;
    unordered_map<state, clazz> acceptingStates;
    state initialState;

    bool readFile(string& filePath);
};


#endif //LEXICAL_PARSER_BUILDER_TRANSITIONTABLE_H
