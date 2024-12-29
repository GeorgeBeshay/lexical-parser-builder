#include <iostream>
#include <unordered_map>

#include "lexical-analyzer/utilities/TransitionTableContainer.h"
#include "lexical-analyzer/LexicalAnalyzer.h"
#include "lexical-analyzer/utilities/lexicalTypes.h"
#include "parser/utilities/parsingTypes.h"
#include "parser/ParsingAnalyzer.h"

using namespace std;

#define TRANS_TBL_PLC_HLD  ("<transition_table_file_path>")
#define INP_FP_PLC_HLD ("<input_file_path>")
#define PRS_TBL_PLC_HLD ("<parsing_table_file_path>")
#define OP_FP_PLC_HLD ("<output_file_path>")
#define SYMBOL_TBL_OP_FP_PLC_HLD ("<symbol_table_output_file_path>")
#define PRS_DERIVATION_PLC_HLD ("<parsing_derivation_output_file_path>")

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Please provide the following arguments:"
        << "\n\t" << TRANS_TBL_PLC_HLD
        << "\n\t" << INP_FP_PLC_HLD
        << "\n\t" << PRS_TBL_PLC_HLD
        <<"\n\n"
        << "Optional:"
        << "\n\t" << OP_FP_PLC_HLD
        << "\n\t" << SYMBOL_TBL_OP_FP_PLC_HLD
        << "\n\t" << PRS_DERIVATION_PLC_HLD
        << "\n\t";

        return EXIT_FAILURE;
    }

    string transitionTableFilePath = argv[1];
    string sourceCodeFilePath = argv[2];
    string parsingTableFilePath = argv[3];
    string lexicalAnalyzerOutputFilePath = (argc > 4) ? argv[4] : "lexical_analyzer_output.txt";
    string symbolTableOutputFilePath = (argc > 5)? argv[5] : "symbol_table.txt";
    string parsingDerivationOutputFilePath = (argc > 6) ? argv[6] : "parsing_derivation.txt";

    TransitionTableContainer transitionTableContainer(transitionTableFilePath);
    LexicalAnalyzer lexicalAnalyzer(transitionTableContainer.getTransitionTable(),
                                    transitionTableContainer.getSymbolToIndexMapper(),
                                    transitionTableContainer.getAcceptingStates(),
                                    transitionTableContainer.getInitialState(),
                                    sourceCodeFilePath);
    ParsingAnalyzer parsingAnalyzer(parsingTableFilePath);


    // Symbol table to be used in the next phase
    unordered_map<t_lexem, string> symbolTable;

    ofstream lexicalAnalyzerOutputFile(lexicalAnalyzerOutputFilePath);
    ofstream symbolTableOutputFile(symbolTableOutputFilePath);

    if(!lexicalAnalyzerOutputFile || !symbolTableOutputFile) {
        throw runtime_error("Error in opening the output files for lexical analyzer!");
    }

    pair<t_clazz, t_lexem> currentPair;

    while(lexicalAnalyzer.isNextTokenAvailable()) {

        currentPair = lexicalAnalyzer.getNextToken();
        while(!parsingAnalyzer.doParseStep(ParsingToken(currentPair.first, true)));

        if(currentPair.first == IDENTIFIER_CLASS) {
            symbolTable[currentPair.second] = "";
        }

        lexicalAnalyzerOutputFile << currentPair.first << " " << currentPair.second << endl;
    }
    while(!parsingAnalyzer.doParseStep(END_TOKEN));

    for(auto& symbolRow: symbolTable) {
        symbolTableOutputFile << symbolRow.first << endl;
    }

    lexicalAnalyzerOutputFile.close();
    symbolTableOutputFile.close();
    parsingAnalyzer.exportDerivation(parsingDerivationOutputFilePath);
    return 0;
}
