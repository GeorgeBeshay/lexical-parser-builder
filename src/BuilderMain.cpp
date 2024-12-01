#include "RegexScanner.h"
#include "NFA.h"
#include "DFAGenerator.h"
#include "TransitionTableContainer.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Please provide: " << argv[0] << " <lexical_rules_file_path> optional:<transition_table_file_path> ";
        return 1;
    }
    string lexicalRulesFilePath = argv[1];
    string transitionTableOutputPath = (argc > 2)? argv[2] : "transitionTable.txt";

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

    return 0;
}
