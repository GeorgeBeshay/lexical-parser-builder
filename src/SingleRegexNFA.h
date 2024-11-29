#ifndef SINGLEREGEXNFA_H
#define SINGLEREGEXNFA_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <stack>
#include <iostream>

using namespace std;

// Type aliases for clarity
using State = int;
using Symbol = char;
using Class = string;

class NFA {
public:
    State startState; // Start state of the NFA
    State acceptState; // Accept state of the NFA
    unordered_map<State, unordered_map<Symbol, unordered_set<State>>> transitions; // Regular transitions
    unordered_map<State, unordered_set<State>> epsilonTransitions; // Epsilon transitions
    unordered_map<State, Class> acceptStateClasses; // Mapping of accept state to its class

    // Constructor
    NFA(State start, State accept);

    // Add regular transition
    void addTransition(State from, Symbol symbol, State to);

    // Add epsilon transition
    void addEpsilonTransition(State from, State to);

    // Print NFA for debugging
    void printNFA();

    void printGraphviz();
};

class SingleRegexNFA {
public:
    static int stateCounter; // Unique state counter

public:
    // Generate NFA for a single regex and class
    static NFA generateSingleRegexNFA(const string& regex, const Class& className);

private:
    // Helper functions
    static NFA regexToNFA(const string& regex, const Class& className);
    static NFA handleUnion(const NFA& nfa1, const NFA& nfa2);
    static NFA handleConcatenation(const NFA& nfa1, const NFA& nfa2);
    static NFA handleKleeneStar(const NFA& nfa);
    static NFA handlePositiveClosure(const NFA& nfa);
};

#endif
