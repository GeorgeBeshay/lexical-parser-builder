#ifndef SINGLEREGEXNFA_H
#define SINGLEREGEXNFA_H

#include "RegexScanner.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <stack>
#include <iostream>
#include <stack>
#include "set"
#include <regex>

using namespace std;

// Type aliases for clarity
using State = int;
using Symbol = char;
using Class = string;

class NFA {
public:
    State startState;
    State acceptState;
    unordered_map<State, unordered_map<Symbol, unordered_set<State>>> transitions; // Regular transitions
    unordered_map<State, unordered_set<State>> epsilonTransitions;
    unordered_map<State, Class> acceptStateClasses; // Mapping of accept state to its class



    NFA(State start, State accept);


    void addTransition(State from, Symbol symbol, State to);


    void addEpsilonTransition(State from, State to);

    virtual void printGraphviz();
};

class SingleNFA {
public:
    static int stateCounter; // Unique state counter

public:

    static NFA generateSingleRegexNFA(const string &regex, const Class &className,
                                      const vector<pair<string, string>> &regularDefinitions);

    static NFA generateSingleKeywordNFA(const string &keyword);

    static NFA generateSinglePunctuationNFA(const string &punctuation);

private:

    static NFA
    regexToNFA(const string &regex, const Class &className, const vector<pair<string, string>> &regularDefinitions);

    static NFA handleUnion(const NFA &nfa1, const NFA &nfa2);

    static NFA handleConcatenation(const NFA &nfa1, const NFA &nfa2);

    static NFA handleKleeneStar(const NFA &nfa);

    static NFA handlePositiveClosure(const NFA &nfa);
};

class CombinedNFA : public NFA {
public:
    unordered_set<State> initialStates;
    unordered_set<Symbol> symbols;

    // Constructor
    CombinedNFA();

public:

    CombinedNFA generateCombinedNFA(RegexScanner &regexScanner);

    CombinedNFA combineNFA(const vector<NFA> &nfas);

    void addInitialState(State state);

    void addSymbol(Symbol symbol);

    void printGraphvizSummarized();

    void printGraphviz() override;
};

#endif
