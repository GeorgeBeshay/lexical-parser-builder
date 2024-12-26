#ifndef LEXICAL_PARSER_BUILDER_GRAMMAR_H
#define LEXICAL_PARSER_BUILDER_GRAMMAR_H

#include "GrammarRule.h"
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

#define EPS "\\L"


class Grammar {
private:

    ParsingToken startSymbol;

    unordered_map<ParsingToken, Rule, ParsingTokenHash, Comparator> grammar;

    void generateRule(string &ruleString, bool isStart);

public:

    Grammar() = default;

    explicit Grammar(string grammarInputFilePath);

    void generateGrammarFromFile(string &grammarInputFilePath);

    const unordered_map<ParsingToken, Rule, ParsingTokenHash, Comparator> &getGrammar();

    void setGrammar(unordered_map<ParsingToken, Rule, ParsingTokenHash, Comparator> &newGrammar);

    void addRule(ParsingToken &lhs, t_prodRule &productions);

    void applyLeftFactoring();

    void removeLeftRecursion();

    ParsingToken getStartSymbol();

    vector<Rule> getRules();

    t_grammar get_t_grammar() const;
};

#endif //LEXICAL_PARSER_BUILDER_GRAMMAR_H
