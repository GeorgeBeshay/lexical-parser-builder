#include "lexical-analyzer/utilities/RegexScanner.h"
#include "lexical-analyzer/nfa/NFA.h"
#include "lexical-analyzer/dfa/DFAGenerator.h"
#include "lexical-analyzer/utilities/TransitionTableContainer.h"
#include "lexical-analyzer/utilities/LexicalUtility.h"
#include "parser/Grammar/Grammar.h"
#include "parser/parsing-table/ParsingTable.h"

#define LXCL_RLS_FP_PLC_HLD ("<lexical_rules_file_path>")
#define GRM_RLS_FP_PLC_HLD ("<grammar_rules_file_path>")
#define TRANS_TBL_PLC_HLD  ("<transition_table_file_path>")
#define PRS_TBL_PLC_HLD ("<parsing_table_file_path>")
#define GRPHVIZ_NFA_OP_FP_PLC_HLD ("<graphviz_NFA_output_file_path>")
#define GRPHVIZ_DFA_OP_FP_PLC_HLD ("<graphviz_DFA_output_file_path>")

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cerr << "Please provide the following arguments:"
        << "\n\t" << LXCL_RLS_FP_PLC_HLD
        << "\n\t" << GRM_RLS_FP_PLC_HLD
        <<"\n\n"
        << "Optional:"
        << "\n\t" << TRANS_TBL_PLC_HLD
        << "\n\t" << PRS_TBL_PLC_HLD
        << "\n\t" << GRPHVIZ_NFA_OP_FP_PLC_HLD
        << "\n\t" << GRPHVIZ_DFA_OP_FP_PLC_HLD
        << "\n\t";

        return 1;
    }

    string lexicalRulesFilePath = argv[1];
    string grammarRulesFp = argv[2];
    string transitionTableOutputPath = (argc > 3)? argv[3] : "transitionTable.txt";
    string parsingTableOutputFp = (argc > 4)? argv[4] : "parsingTable.txt";
    string graphvizNfaOutputPath = (argc > 5)? argv[5] : "graphvizNfaOutput.svg";
    string graphvizDfaOutputPath = (argc > 6)? argv[6] : "graphvizDfaOutput.svg";

    // ------------------ Reading and processing lexical rules ------------------

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

    // ------------------ Reading and processing grammar ------------------
    Grammar grammar(grammarRulesFp);
    grammar.applyLeftFactoring();
    grammar.removeLeftRecursion();
    ParsingTable parsingTable(grammar.getStartSymbol(), grammar.get_t_grammar());
    parsingTable.exportParsingTableToFile(parsingTableOutputFp);

    return 0;
}
