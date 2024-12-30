#ifndef LEXICAL_PARSER_BUILDER_LEXICALANALYZER_H
#define LEXICAL_PARSER_BUILDER_LEXICALANALYZER_H

#include <vector>
#include <unordered_map>
#include <map>
#include <fstream>
#include <cctype>
#include <utility>

#include "dfa/DFAGenerator.h"
#include "utilities/lexicalTypes.h"

using namespace std;

class LexicalAnalyzer {
public:

    LexicalAnalyzer(vector<vector<int>>& transitionTable,
                    unordered_map<t_symbol, int>& symbolToIndexMapper,
                    unordered_map<t_state, t_clazz>& acceptingStates,
                    t_state initialState,
                    const string& inputFilePath);

    pair<t_clazz, t_lexem> getNextToken();
    bool isNextTokenAvailable();

private:
    vector<vector<int>> transitionTable;
    unordered_map<t_symbol, int> symbolToIndexMapper;
    unordered_map<t_state, t_clazz> acceptingStates;
    t_state initialState;
    ifstream inputFilePointer;
    string remainingCharacters;

    void printErrorMessage(t_lexem token);
    pair<t_clazz, t_lexem> checkCurrentState(t_lexem token, vector<int> currentTokenStates);
};


#endif //LEXICAL_PARSER_BUILDER_LEXICALANALYZER_H
