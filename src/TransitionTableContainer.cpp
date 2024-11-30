#include "TransitionTableContainer.h"

TransitionTableContainer::TransitionTableContainer(
        unordered_map<state, unordered_map<symbol, state>> transMap,
        unordered_map<state, clazz> acceptingStates,
        state initialState,
        int numberOfStates,
        unordered_set<symbol> symbols
        ) {
    this->transitionTable.resize(numberOfStates, vector<int>(symbols.size(), -1));
    this->acceptingStates = acceptingStates;
    this->initialState = initialState;

    int index = 0;
    for(auto& symbol: symbols) {
        this->symbolToIndexMapper[symbol] = index++;
    }

    for(auto& outerPair: transMap) {
        state currentState = outerPair.first;

        for(auto& transition: outerPair.second) {
            int currentSymbolIndex = this->symbolToIndexMapper[transition.first];
            this->transitionTable[currentState][currentSymbolIndex] = transition.second;
        }
    }
}

bool TransitionTableContainer::writeFile(string &filePath) {
    ofstream outFile(filePath);

    if (!outFile) {
        cerr << "Error opening file for writing the transition table!" << endl;
        return false;
    }

    int rows = transitionTable.size();
    int cols = (rows > 0) ? transitionTable[0].size() : 0;

    outFile << rows << " " << cols << endl;

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            outFile << transitionTable[i][j] << " ";
        }
        outFile << endl;
    }

    for (const auto& symbolIndexPair : symbolToIndexMapper) {
        outFile << symbolIndexPair.first << " " << symbolIndexPair.second << endl;
    }
    outFile << endl;

    for (const auto& acceptingState : acceptingStates) {
        outFile << acceptingState.first << " " << acceptingState.second << endl;
    }

    outFile << endl;

    outFile << initialState << endl;

    outFile.close();
    return true;
}

bool TransitionTableContainer::readFile(string &filePath) {
    ifstream inFile(filePath);

    if (!inFile) {
        cerr << "Error opening transition table file for reading!" << endl;
        return false;
    }

    string line;
    int rows, cols;

    getline(inFile, line);
    stringstream ss(line);
    ss >> rows >> cols;

    transitionTable.resize(rows, vector<int>(cols));

    for (int i = 0; i < rows; i++) {
        getline(inFile, line);
        stringstream rowStream(line);
        for (int j = 0; j < cols; j++) {
            rowStream >> transitionTable[i][j];
        }
    }

    while (getline(inFile, line)) {
        if (line.empty()) break;
        stringstream symbolStream(line);
        symbol currentSymbol;
        int index;
        symbolStream >> currentSymbol >> index;
        symbolToIndexMapper[currentSymbol] = index;
    }

    while (getline(inFile, line)) {
        if (line.empty()) break;
        stringstream acceptingStream(line);
        int stateId;
        string stateName;
        acceptingStream >> stateId;
        getline(acceptingStream, stateName);
        acceptingStates[stateId] = stateName.substr(1);
    }

    getline(inFile, line);
    stringstream initialStateStream(line);
    initialStateStream >> initialState;

    inFile.close();
    return true;
}