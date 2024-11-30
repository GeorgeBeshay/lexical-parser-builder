#include <iostream>
#include <unordered_map>

#include "TransitionTableContainer.h"
#include "LexicalAnalyzer.h"

#define IDENTIFIER_CLASS "id"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Please provide: " << argv[0] << " <transition_table_file_path> <input_file_path> "
                                          "optional:<output_file_path> optional:<symbol_table_output_file_path>\n";
        return 1;
    }

    string transitionTableFilePath = argv[1];
    string sourceCodeFilePath = argv[2];
    string outputFilePath = (argc > 3)? argv[3] : "output.txt";
    string symbolTableOutputFilePath = (argc > 4)? argv[4] : "symbol_table.txt";

    TransitionTableContainer transitionTableContainer(transitionTableFilePath);
    LexicalAnalyzer lexicalAnalyzer(transitionTableContainer.getTransitionTable(),
                                    transitionTableContainer.getSymbolToIndexMapper(),
                                    transitionTableContainer.getAcceptingStates(),
                                    transitionTableContainer.getInitialState(),
                                    sourceCodeFilePath);


    // Symbol table to be used in the next phase
    unordered_map<LexicalAnalyzer::lexem, string> symbolTable;

    ofstream outputFile(outputFilePath);
    ofstream symbolTableOutputFile(symbolTableOutputFilePath);

    if(!outputFile || !symbolTableOutputFile) {
        throw runtime_error("Error in opening the output files for lexical analyzer!");
    }

    pair<LexicalAnalyzer::clazz, LexicalAnalyzer::lexem> currentPair;

    while(lexicalAnalyzer.isNextTokenAvailable()) {
        currentPair = lexicalAnalyzer.getNextToken();
        if (currentPair.first.empty() && currentPair.second.empty()) {
            break;
        }

        if(currentPair.first == IDENTIFIER_CLASS) {
            symbolTable[currentPair.second] = "";
            symbolTableOutputFile << currentPair.second << endl;
        }

        outputFile << currentPair.first << " " << currentPair.second << endl;
        cout << currentPair.first << " " << currentPair.second << endl;
    }

    outputFile.close();
    symbolTableOutputFile.close();
    return 0;
}
