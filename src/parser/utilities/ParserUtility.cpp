#include "ParserUtility.h"

// Prototype functions
t_parsingTokenSet computeFirstSetsRecursive(t_grammar& grammar,
                                           t_parsingTokenSetMap& firstSetsMap,
                                           unordered_map<ParsingToken, bool, ParsingTokenHash>& visited,
                                           const ParsingToken currentToken);

t_parsingTokenSet handleNonTerminalProdAlt(t_grammar& grammar,
                                          t_parsingTokenSetMap& firstSetsMap,
                                          unordered_map<ParsingToken, bool, ParsingTokenHash>& visited,
                                          t_prodAlt prodAlt);

//------------------------------------------------------------------------------------------------------

t_parsingTokenSetMap ParserUtility::computeFirstSets(t_grammar grammar) {
    t_parsingTokenSetMap firstSets;
    unordered_map<ParsingToken, bool, ParsingTokenHash> visited;

    for(const pair<ParsingToken, t_prodRule>& entry: grammar) {
        computeFirstSetsRecursive(grammar, firstSets, visited, entry.first);
    }

    return firstSets;
}

// Implementation of prototype functions
t_parsingTokenSet computeFirstSetsRecursive(t_grammar& grammar,
                                           t_parsingTokenSetMap& firstSetsMap,
                                           unordered_map<ParsingToken, bool, ParsingTokenHash>& visited,
                                           const ParsingToken currentToken) {

    if (firstSetsMap.count(currentToken) != 0) {
        return firstSetsMap[currentToken];
    }

    if (visited[currentToken]) {
        throw runtime_error("The grammar has left recursion, detected in computation of first sets");
    }

    visited[currentToken] = true;

    t_parsingTokenSet currentTokenFirstSet;
    t_prodRule currentTokenProductionRule = grammar[currentToken];

    for(t_prodAlt productionAlternative: currentTokenProductionRule) {
        if(productionAlternative.size() >= 1) {
            ParsingToken firstTokenInProductionAlternative = productionAlternative[0];

            if(firstTokenInProductionAlternative.getIsTerminal() && !firstTokenInProductionAlternative.isEpsilon()) {
                currentTokenFirstSet.insert(firstTokenInProductionAlternative);
            }
            else if(firstTokenInProductionAlternative.isEpsilon() && productionAlternative.size() == 1) {
                currentTokenFirstSet.insert(firstTokenInProductionAlternative);
            }
            else if(!firstTokenInProductionAlternative.getIsTerminal()) {
                t_parsingTokenSet tempSet = handleNonTerminalProdAlt(grammar, firstSetsMap, visited, productionAlternative);
                currentTokenFirstSet.insert(tempSet.begin(), tempSet.end());
            }
        }
    }

    firstSetsMap[currentToken] = currentTokenFirstSet;
    return currentTokenFirstSet;
}

bool setContainsEpsilon(t_parsingTokenSet tokenSet) {
    for(ParsingToken parsingToken: tokenSet) {
        if(parsingToken.isEpsilon()) {
            return true;
        }
    }

    return false;
}

t_parsingTokenSet filterSetFromEpsilon(t_parsingTokenSet tokenSet) {
    t_parsingTokenSet currentSet;
    for(ParsingToken parsingToken: tokenSet) {
        if(!parsingToken.isEpsilon()) {
            currentSet.insert(parsingToken);
        }
    }

    return currentSet;
}

t_parsingTokenSet handleNonTerminalProdAlt(t_grammar& grammar,
                                           t_parsingTokenSetMap& firstSetsMap,
                                           unordered_map<ParsingToken, bool, ParsingTokenHash>& visited,
                                           t_prodAlt prodAlt) {
    t_parsingTokenSet currentSet;
    t_parsingTokenSet tempSet;
    t_parsingTokenSet filteredSet;
    for(ParsingToken parsingToken: prodAlt) {
        if(!parsingToken.getIsTerminal()) {
            tempSet = computeFirstSetsRecursive(grammar, firstSetsMap, visited, parsingToken);
            filteredSet = filterSetFromEpsilon(tempSet);
            currentSet.insert(filteredSet.begin(), filteredSet.end());

            if(!setContainsEpsilon(tempSet)) {
                return currentSet;
            }
        }
        else {
            currentSet.insert(parsingToken);
            return currentSet;
        }
    }

    currentSet.insert(ParsingToken(EPSILON_STRING, true));
    return currentSet;
}
