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
    t_state startState;
    t_state acceptState;
    unordered_map<t_state, unordered_map<t_symbol, unordered_set<t_state>>> transitions; // Regular transitions
    unordered_map<t_state, unordered_set<t_state>> epsilonTransitions;
    unordered_map<t_state, t_clazz> acceptStateClasses; // Mapping of accept t_state to its class



    NFA(t_state start, t_state accept);


    void addTransition(t_state from, t_symbol symbol, t_state to);


    void addEpsilonTransition(t_state from, t_state to);

    virtual void printGraphviz();
};

class SingleNFA {
public:
    static int stateCounter; // Unique t_state counter

public:

    static NFA generateSingleRegexNFA(const string &regex, const t_clazz &className,
                                      const vector<pair<string, string>> &regularDefinitions);

    static NFA generateSingleKeywordNFA(const string &keyword);

    static NFA generateSinglePunctuationNFA(const string &punctuation);

private:

    static NFA
    regexToNFA(const string &regex, const t_clazz &className, const vector<pair<string, string>> &regularDefinitions);

    static NFA handleUnion(const NFA &nfa1, const NFA &nfa2);

    static NFA handleConcatenation(const NFA &nfa1, const NFA &nfa2);

    static NFA handleKleeneStar(const NFA &nfa);

    static NFA handlePositiveClosure(const NFA &nfa);
};

class CombinedNFA : public NFA {
public:
    unordered_set<t_state> initialStates;
    unordered_set<t_symbol> symbols;

    // Constructor
    CombinedNFA();

public:

    CombinedNFA generateCombinedNFA(RegexScanner &regexScanner);

    CombinedNFA combineNFA(const vector<NFA> &nfas);

    void addInitialState(t_state state);

    void addSymbol(t_symbol symbol);

    void printGraphvizSummarized();

    void printGraphviz() override;
};

#endif
