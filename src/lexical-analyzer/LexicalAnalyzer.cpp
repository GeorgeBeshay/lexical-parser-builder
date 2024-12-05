#include "LexicalAnalyzer.h"

#define ERROR_MESSAGE "Error: No match for this token"

LexicalAnalyzer::LexicalAnalyzer(vector<vector<int>>& transitionTable,
                                 unordered_map<symbol, int>& symbolToIndexMapper,
                                 unordered_map<state, clazz>& acceptingStates,
                                 state initialState,
                                 const string& inputFilePath)
        : transitionTable(transitionTable),
          symbolToIndexMapper(symbolToIndexMapper),
          acceptingStates(acceptingStates),
          initialState(initialState),
          inputFilePointer(inputFilePath) {
    if (!inputFilePointer.is_open()) {
        throw runtime_error("Failed to open input file: " + inputFilePath);
    }
    this->remainingCharacters = "";
}

pair<clazz, lexem> LexicalAnalyzer::getNextToken() {
    if(!isNextTokenAvailable() && this->remainingCharacters.empty()) {
        return {};
    }

    lexem token = "";
    char currentCharacter;
    state currentState = this->initialState;
    state nextState;
    vector<state> currentTokenStates;

    // Check if there are some remaining characters from the last time
    for(int i = 0; i < this->remainingCharacters.size(); i++) {
        currentCharacter = this->remainingCharacters[i];
        token += currentCharacter;

        nextState = (this->symbolToIndexMapper.count(currentCharacter) != 0)?
                    this->transitionTable[currentState][this->symbolToIndexMapper[currentCharacter]] :
                    REJECTING_STATE;

        currentState = nextState;
        currentTokenStates.push_back(nextState);

        if(nextState == REJECTING_STATE) {
            this->remainingCharacters = this->remainingCharacters.substr(token.size());
            return checkCurrentState(token, currentTokenStates);
        }
    }
    this->remainingCharacters = "";

    // Continue reading from the file one character at a time
    while(this->inputFilePointer.get(currentCharacter)) {
        if(isspace(currentCharacter)) {
            if(token.empty()) {
                continue;
            }
            else {
                break;
            }
        }

        token += currentCharacter;

        nextState = (this->symbolToIndexMapper.count(currentCharacter) != 0) ?
                    this->transitionTable[currentState][this->symbolToIndexMapper[currentCharacter]] :
                    REJECTING_STATE;

        currentTokenStates.push_back(nextState);

        if (nextState == REJECTING_STATE) {
            return checkCurrentState(token, currentTokenStates);
        }

        currentState = nextState;
    }

    return checkCurrentState(token, currentTokenStates);
}

void LexicalAnalyzer::printErrorMessage(lexem token) {
    cout << ERROR_MESSAGE << ": " << token << endl;
}

bool LexicalAnalyzer::isNextTokenAvailable() {
    return this->inputFilePointer.good() || !this->remainingCharacters.empty();
}

pair<clazz, lexem> LexicalAnalyzer::checkCurrentState(lexem token, vector<int> currentTokenStates) {
    if(currentTokenStates.empty()) {
        return {};
    }

    for(int i = currentTokenStates.size() - 1; i >= 0; i--) {
        if(this->acceptingStates.count(currentTokenStates[i]) != 0) {
            lexem tmpToken = token.substr(0, i + 1);
            this->remainingCharacters = token.substr(i + 1) + this->remainingCharacters;

            return {this->acceptingStates[currentTokenStates[i]], tmpToken};
        }
    }

    printErrorMessage(token);
    return {ERROR_MESSAGE, token};
}