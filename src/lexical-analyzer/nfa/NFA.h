#ifndef SINGLEREGEXNFA_H
#define SINGLEREGEXNFA_H

#include "../utilities/lexicalTypes.h"
#include "../utilities/RegexScanner.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <stack>
#include <iostream>
#include <stack>
#include "set"
#include <regex>


class NFA {
public:
    state startState;
    state acceptState;
    unordered_map<state, unordered_map<symbol, unordered_set<state>>> transitions; // Regular transitions
    unordered_map<state, unordered_set<state>> epsilonTransitions;
    unordered_map<state, clazz> acceptStateClasses; // Mapping of accept state to its class



    NFA(state start, state accept);


    void addTransition(state from, symbol symbol, state to);


    void addEpsilonTransition(state from, state to);

    virtual void printGraphviz();
};

class SingleNFA {
public:
    static int stateCounter; // Unique state counter

public:

    static NFA generateSingleRegexNFA(const string &regex, const clazz &className,
                                      const vector<pair<string, string>> &regularDefinitions);

    static NFA generateSingleKeywordNFA(const string &keyword);

    static NFA generateSinglePunctuationNFA(const string &punctuation);

private:

    static NFA
    regexToNFA(const string &regex, const clazz &className, const vector<pair<string, string>> &regularDefinitions);

    static NFA handleUnion(const NFA &nfa1, const NFA &nfa2);

    static NFA handleConcatenation(const NFA &nfa1, const NFA &nfa2);

    static NFA handleKleeneStar(const NFA &nfa);

    static NFA handlePositiveClosure(const NFA &nfa);
};

class CombinedNFA : public NFA {
public:
    unordered_set<state> initialStates;
    unordered_set<symbol> symbols;

    // Constructor
    CombinedNFA();

public:

    CombinedNFA generateCombinedNFA(RegexScanner &regexScanner);

    CombinedNFA combineNFA(const vector<NFA> &nfas);

    void addInitialState(state state);

    void addSymbol(symbol symbol);

    void printGraphvizSummarized();

    void printGraphviz() override;
};

#endif
