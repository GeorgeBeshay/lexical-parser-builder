#include "RegexScanner.h"
#include "NFA.h"

int main() {
    string filePath = "../inputs/LexicalRulesInput.txt";
    RegexScanner regexScanner(filePath);
    regexScanner.printOutput();

    CombinedNFA combinedNFA ;
    combinedNFA = combinedNFA.generateCombinedNFA(regexScanner);

    // CombinedNFA attributes
//    unordered_map<state, unordered_map<symbol, unordered_set<state>>> transitions;
//    unordered_map<state, unordered_set<state>> epsilonTransitions;
//    unordered_map<state, clazz> acceptStateClasses;
//    unordered_set<state> initialStates;
//    unordered_set<symbol> symbols;


    return 0;
}
