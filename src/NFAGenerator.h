#ifndef NFAGENERATOR_H
#define NFAGENERATOR_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <stack>
#include <set>
#include <stdexcept>

class NFAGenerator {
public:
    struct NFA {
        int startState;  // Start state of the NFA
        int acceptState; // Accept state of the NFA
        std::unordered_map<int, std::unordered_map<char, std::unordered_set<int>>> transitions;  // Transitions between states
        std::unordered_map<int, std::unordered_set<int>> epsilonTransitions;  // Epsilon transitions
        std::unordered_set<char> symbols;  // Set of symbols used in the NFA
    };

    // Generates an NFA for a single regular expression
    static NFA generateNFA(const std::string& regex);

private:
    static NFA createSimpleNFA(char ch);
    static NFA handleUnion(NFA& nfa1, NFA& nfa2);
    static NFA handleConcatenation(NFA& nfa1, NFA& nfa2);
    static NFA handleKleeneStar(NFA& baseNFA);
    static NFA handlePositiveClosure(NFA& baseNFA);
    static void copyTransitions(const NFA& fromNFA, NFA& toNFA, int offset);
    static bool isOperator(char ch);
    static void validateRegex(const std::string& regex);
};

#endif // NFAGENERATOR_H
