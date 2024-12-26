#include "GrammarRule.h"

#define EPS "\\L"
using namespace std;

Rule::Rule(ParsingToken &lhs, t_prodRule &productions) {
    this->lhs = lhs;
    this->productions = productions;
}

const ParsingToken &Rule::getLhs() const {
    return lhs;
}

const t_prodRule &Rule::getProductions() const {
    return productions;
}

void Rule::clearProductions() {
    productions.clear();
}

void Rule::setProductions(t_prodRule &newProductions) {
    productions = newProductions;
}

void Rule::addProductions(t_prodRule &newProductions) {
    for (const auto &production: newProductions)
        productions.insert(production);
}

string Rule::toString() {
    string ruleString;
    ruleString += lhs.getToken() + " -> ";
    string productionString;
    for (const auto &production: productions) {
        for (const ParsingToken &token: production) {
            if (token.getToken() == EPS)
                productionString += "ε ";
            else
                productionString += token.getToken() + " ";
        }
        productionString += "| ";
        ruleString += productionString.substr(0, productionString.size());
        productionString.clear();
    }
    return ruleString.substr(0, ruleString.size() - 2); // Remove last space and "|".
}


