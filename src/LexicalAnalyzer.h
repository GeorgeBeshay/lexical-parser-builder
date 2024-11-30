#ifndef LEXICAL_PARSER_BUILDER_LEXICALANALYZER_H
#define LEXICAL_PARSER_BUILDER_LEXICALANALYZER_H

#include <vector>
#include <unordered_map>
#include <map>
#include <fstream>
#include <cctype>
#include <utility>

#include "DFAGenerator.h"

using namespace std;

class LexicalAnalyzer {
public:
    using state = DFAGenerator::state;
    using symbol = DFAGenerator::symbol;
    using clazz = DFAGenerator::clazz;
    using lexem = string;

    LexicalAnalyzer(vector<vector<int>>& transitionTable,
                    unordered_map<symbol, int>& symbolToIndexMapper,
                    unordered_map<state, clazz>& acceptingStates,
                    state initialState,
                    const string& inputFilePath);

    pair<clazz, lexem> getNextToken();
    bool isNextTokenAvailable();

private:
    vector<vector<int>> transitionTable;
    unordered_map<symbol, int> symbolToIndexMapper;
    unordered_map<state, clazz> acceptingStates;
    state initialState;
    ifstream inputFilePointer;

    void printErrorMessage(lexem nonIdentifiedToken);
    clazz checkCurrentState(state currentState);
};


#endif //LEXICAL_PARSER_BUILDER_LEXICALANALYZER_H
