#include "lexical-analyzer/utilities/RegexScanner.h"
#include "lexical-analyzer/nfa/NFA.h"
#include "lexical-analyzer/dfa/DFAGenerator.h"
#include "lexical-analyzer/utilities/TransitionTableContainer.h"
#include "lexical-analyzer/utilities/LexicalUtility.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Please provide: " << argv[0] << " <lexical_rules_file_path> "
                                                 "optional:<transition_table_file_path> "
                                                 "optional:<graphviz_NFA_output_file_path> "
                                                 "optional:<graphviz_DFA_output_file_path> ";
        return 1;
    }
    string lexicalRulesFilePath = argv[1];
    string transitionTableOutputPath = (argc > 2)? argv[2] : "transitionTable.txt";
    string graphvizNfaOutputPath = (argc > 3)? argv[3] : "graphvizNfaOutput.svg";
    string graphvizDfaOutputPath = (argc > 4)? argv[4] : "graphvizDfaOutput.svg";


    RegexScanner regexScanner(lexicalRulesFilePath);
    CombinedNFA combinedNFA ;
    combinedNFA = combinedNFA.generateCombinedNFA(regexScanner);

    DFAGenerator dfaGenerator(
            combinedNFA.transitions,
            combinedNFA.epsilonTransitions,
            combinedNFA.acceptStateClasses,
            combinedNFA.initialStates,
            combinedNFA.symbols
    );
    TransitionTableContainer transitionTableContainer(
            dfaGenerator.getTransMap(),
            dfaGenerator.getAcceptingStates(),
            dfaGenerator.getInitialState(),
            dfaGenerator.getNumberOfStates(),
            dfaGenerator.getLanguageSymbols()
    );

    transitionTableContainer.writeFile(transitionTableOutputPath);

    LexicalUtility::visualizeDfa(
        dfaGenerator.getInitialState(),
        dfaGenerator.getLanguageSymbols(),
        dfaGenerator.getTransMap(),
        dfaGenerator.getAcceptingStates(),
        dfaGenerator.getNumberOfStates(),
        graphvizDfaOutputPath
    );

    LexicalUtility::visualizeNfa(
        combinedNFA.initialStates,
        combinedNFA.symbols,
        combinedNFA.transitions,
        combinedNFA.epsilonTransitions,
        combinedNFA.acceptStateClasses,
        SingleNFA::stateCounter,
        graphvizNfaOutputPath
    );

    return 0;
}
