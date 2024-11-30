#include "RegexScanner.h"
#include "NFA.h"

int main() {
    // Example file path for RegexScanner
    string filePath = "../../inputs/LexicalRulesInput.txt";
    RegexScanner regexScanner(filePath);
    regexScanner.printOutput();

    vector<NFA> nfas;

    // add keyword NFAs
    for (const auto& keyword : regexScanner.getReservedKeywords()) {
        nfas.push_back(SingleNFA::generateSingleKeywordNFA(keyword));
    }
    for (const auto& punctuation : regexScanner.getReservedPunctuations()) {
        nfas.push_back(SingleNFA::generateSinglePunctuationNFA(punctuation));
    }
    // add regular expression NFAs

//    NFA nfa = SingleNFA::generateSingleRegexNFA("digit+ | digit+ . digits ( \\L | E digits)", "number", regexScanner.getRegularDefinitions());
//
//    nfas.push_back(nfa);



    for (const auto& [className, regex] : regexScanner.getRegularExpressions()) {
        nfas.push_back(SingleNFA::generateSingleRegexNFA(regex, className, regexScanner.getRegularDefinitions()));
    }

//    nfa.printGraphviz();

    CombinedNFA combinedNFA ;
    combinedNFA = combinedNFA.combineNFA(nfas);
    combinedNFA.printCombinedNFA();
    combinedNFA.printGraphviz();
    combinedNFA.printGraphvizSummarized();







    return 0;
}
