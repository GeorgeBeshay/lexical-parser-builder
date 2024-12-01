#include "LexicalAnalyzer.h"

#define ERROR_MESSAGE "Error: No match for this token"
#define REJECTED_STATE -1

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
}

pair<LexicalAnalyzer::clazz, LexicalAnalyzer::lexem> LexicalAnalyzer::getNextToken() {
    if(!isNextTokenAvailable()) {
        return {};
    }

    clazz className = "";
    lexem token = "";

    char currentCharacter;
    state currentState = this->initialState;
    state nextState;
    symbol currentSymbol;
    bool isError = false;

    while(!this->inputFilePointer.eof()) {
        currentCharacter = this->inputFilePointer.peek();

        if(this->inputFilePointer.eof()) {
            break;
        }

        if(isspace(currentCharacter)) {
            if(token.empty()) {
                this->inputFilePointer.get(currentCharacter);
                continue;
            }
            else {
                break;
            }
        }

        if(this->symbolToIndexMapper.count(currentCharacter) != 0) {
            currentSymbol = currentCharacter;
            nextState = this->transitionTable[currentState][this->symbolToIndexMapper[currentSymbol]];

            if (nextState == REJECTED_STATE) {
                break;
            }

            currentState = nextState;
            token += currentSymbol;
            this->inputFilePointer.get(currentCharacter);
        }
        else {
            if (token == "") {
                isError = true;
                this->inputFilePointer.get(currentCharacter);
                token += currentCharacter;
            }
            break;
        }
    }

    if (this->inputFilePointer.eof()) {
        this->inputFilePointer.close();
    }

    if (token == "") {
        return {};
    }

    if (isError) {
        printErrorMessage(token);
        className = ERROR_MESSAGE;
    }
    else {
        className = checkCurrentState(currentState);
    }

    return {className, token};
}

void LexicalAnalyzer::printErrorMessage(LexicalAnalyzer::lexem nonIdentifiedToken) {
    cout << ERROR_MESSAGE << ": " << nonIdentifiedToken;
}

bool LexicalAnalyzer::isNextTokenAvailable() {
    return this->inputFilePointer.good();
}

LexicalAnalyzer::clazz LexicalAnalyzer::checkCurrentState(LexicalAnalyzer::state currentState) {
    if (this->acceptingStates.count(currentState) != 0) {
        return this->acceptingStates[currentState];
    }
    else {
       return ERROR_MESSAGE;
    }
}