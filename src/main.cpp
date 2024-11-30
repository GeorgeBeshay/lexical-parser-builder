#include "RegexScanner.h"
#include "NFA.h"

int main() {
    string filePath = "../inputs/LexicalRulesInput.txt";
    RegexScanner regexScanner(filePath);
    regexScanner.printOutput();

    CombinedNFA combinedNFA ;
    combinedNFA = combinedNFA.generateCombinedNFA(regexScanner);

    // CombinedNFA attributes
//    unordered_map<State, unordered_map<Symbol, unordered_set<State>>> transitions;
//    unordered_map<State, unordered_set<State>> epsilonTransitions;
//    unordered_map<State, Class> acceptStateClasses;
//    unordered_set<State> initialStates;
//    unordered_set<Symbol> symbols;


    return 0;
}
