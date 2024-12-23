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

t_parsingTokenSet computeFollowSetsRecursive(t_grammar& grammar,
                                             t_parsingTokenSetMap& followSetsMap,
                                             unordered_map<ParsingToken, bool, ParsingTokenHash>& visited,
                                             unordered_map<ParsingToken, bool, ParsingTokenHash>& finished,
                                             const ParsingToken currentToken,
                                             t_parsingTokenSetMap& firstSetsMap);

t_parsingTokenSet handleFollowSetExtraction(t_grammar& grammar,
                                            t_parsingTokenSetMap& followSetsMap,
                                            unordered_map<ParsingToken, bool, ParsingTokenHash>& visited,
                                            unordered_map<ParsingToken, bool, ParsingTokenHash>& finished,
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

t_parsingTokenSetMap ParserUtility::computeFollowSets(t_grammar grammar, ParsingToken startSymbol,
                                                      t_parsingTokenSetMap firstSetsMap) {
    t_parsingTokenSetMap followSetsMap;
    unordered_map<ParsingToken, bool, ParsingTokenHash> visited;
    unordered_map<ParsingToken, bool, ParsingTokenHash> finished;

    ParsingToken endSymbol(END_SYMBOL, true);
    followSetsMap[startSymbol].insert(endSymbol);

    for(const pair<ParsingToken, t_prodRule>& grammarEntry: grammar) {
        computeFollowSetsRecursive(grammar, followSetsMap, visited, finished,
                                   grammarEntry.first, firstSetsMap);
    }

    return followSetsMap;
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

t_parsingTokenSet computeFollowSetsRecursive(t_grammar& grammar,
                                             t_parsingTokenSetMap& followSetsMap,
                                             unordered_map<ParsingToken, bool, ParsingTokenHash>& visited,
                                             unordered_map<ParsingToken, bool, ParsingTokenHash>& finished,
                                             const ParsingToken currentToken,
                                             t_parsingTokenSetMap& firstSetsMap) {
    if (finished[currentToken]) {
        return followSetsMap[currentToken];
    }

    if (visited[currentToken]) {
        throw runtime_error("The grammar has left recursion, detected in computation of follow sets");
    }

    visited[currentToken] = true;
    t_parsingTokenSet currentTokenFollowSet = handleFollowSetExtraction(grammar, followSetsMap, visited, finished, currentToken, firstSetsMap);

    finished[currentToken] = true;
    followSetsMap[currentToken].insert(currentTokenFollowSet.begin(), currentTokenFollowSet.end());

    return followSetsMap[currentToken];
}

t_parsingTokenSet handleFollowSetExtraction(t_grammar& grammar,
                                            t_parsingTokenSetMap& followSetsMap,
                                            unordered_map<ParsingToken, bool, ParsingTokenHash>& visited,
                                            unordered_map<ParsingToken, bool, ParsingTokenHash>& finished,
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

                            if(!setContainsEpsilon(tempFirstSet)) {
                                takeFollowSetOfThisEntry = false;
                                break;
                            }
                        }
                    }

                    if (takeFollowSetOfThisEntry && grammarEntry.first != currentToken) {
                        t_parsingTokenSet tempFollowSet = computeFollowSetsRecursive(grammar, followSetsMap,
                                                                                     visited, finished,
                                                                                     grammarEntry.first, firstSetsMap);

                        currentTokenFollowSet.insert(tempFollowSet.begin(), tempFollowSet.end());
                    }
                }
            }
        }
    }

    return currentTokenFollowSet;
}