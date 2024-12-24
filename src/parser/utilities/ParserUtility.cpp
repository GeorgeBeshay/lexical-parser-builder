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

void computeFollowSetsIterative(t_grammar& grammar,
                                t_parsingTokenSetMap& followSetsMap,
                                t_parsingTokenSetMap& firstSetsMap);

void handleFollowSetExtraction(t_grammar& grammar,
                               t_parsingTokenSetMap& followSetsMap,
                               const ParsingToken currentToken,
                               t_parsingTokenSetMap& firstSetsMap);

//------------------------------------------------------------------------------------------------------

t_parsingTokenSetMap ParserUtility::computeFirstSets(t_grammar grammar) {
    t_parsingTokenSetMap firstSetsMap;
    unordered_map<ParsingToken, bool, ParsingTokenHash> visited;

    for(const pair<ParsingToken, t_prodRule>& grammarEntry: grammar) {
        computeFirstSetsRecursive(grammar, firstSetsMap, visited, grammarEntry.first);
    }

    return firstSetsMap;
}

t_parsingTokenSetMap ParserUtility::computeFollowSets(t_grammar grammar, ParsingToken startSymbol, t_parsingTokenSetMap firstSetsMap) {
    t_parsingTokenSetMap followSetsMap;

    followSetsMap[startSymbol].insert(END_TOKEN);

    computeFollowSetsIterative(grammar, followSetsMap, firstSetsMap);

    return followSetsMap;
}

bool ParserUtility::setContainsEpsilon(t_parsingTokenSet tokenSet) {
    for(ParsingToken parsingToken: tokenSet) {
        if(parsingToken.isEpsilon()) {
            return true;
        }
    }

    return false;
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

            if(!ParserUtility::setContainsEpsilon(tempSet)) {
                return currentSet;
            }
        }
        else {
            currentSet.insert(parsingToken);
            return currentSet;
        }
    }

    currentSet.insert(EPSILON_TOKEN);
    return currentSet;
}

void computeFollowSetsIterative(t_grammar& grammar,
                                t_parsingTokenSetMap& followSetsMap,
                                t_parsingTokenSetMap& firstSetsMap) {

    bool changed = true;
    ParsingToken currentToken;
    t_parsingTokenSet currentTokenFollowSet;

    while(changed) {
        changed = false;

        for(const pair<ParsingToken, t_prodRule>& grammarEntry: grammar) {
            currentToken = grammarEntry.first;
            currentTokenFollowSet = followSetsMap[currentToken];

            handleFollowSetExtraction(grammar, followSetsMap, currentToken, firstSetsMap);

            if(currentTokenFollowSet.size() != followSetsMap[currentToken].size()) {
                changed = true;
            }
        }
    }

}

void handleFollowSetExtraction(t_grammar& grammar,
                               t_parsingTokenSetMap& followSetsMap,
                               const ParsingToken currentToken,
                               t_parsingTokenSetMap& firstSetsMap) {
    t_parsingTokenSet currentTokenFollowSet;

    for(const pair<ParsingToken, t_prodRule>& grammarEntry: grammar) {
        for(t_prodAlt prodAlt: grammarEntry.second) {
            for(int i = 0; i < prodAlt.size(); i++) {
                ParsingToken tempToken = prodAlt[i];
                if(currentToken == tempToken) {
                    bool takeFollowSetOfThisEntry = true;

                    for(int j = i + 1; j < prodAlt.size(); j++) {
                        ParsingToken followToken = prodAlt[j];
                        if(followToken.getIsTerminal()) {
                            currentTokenFollowSet.insert(followToken);
                            takeFollowSetOfThisEntry = false;
                            break;
                        }
                        else {
                            t_parsingTokenSet tempFirstSet = firstSetsMap[followToken];
                            t_parsingTokenSet tempFilteredFirstSet = filterSetFromEpsilon(tempFirstSet);
                            currentTokenFollowSet.insert(tempFilteredFirstSet.begin(), tempFilteredFirstSet.end());

                            if(!ParserUtility::setContainsEpsilon(tempFirstSet)) {
                                takeFollowSetOfThisEntry = false;
                                break;
                            }
                        }
                    }

                    if (takeFollowSetOfThisEntry) {
                        t_parsingTokenSet tempFollowSet = followSetsMap[grammarEntry.first];
                        currentTokenFollowSet.insert(tempFollowSet.begin(), tempFollowSet.end());
                    }
                }
            }
        }
    }

    followSetsMap[currentToken].insert(currentTokenFollowSet.begin(), currentTokenFollowSet.end());
}