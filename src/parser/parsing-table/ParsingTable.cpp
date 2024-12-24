#include "ParsingTable.h"

bool ParsingTable::initializeParsingTable(ParsingToken startSymbol, t_grammar grammar) {
    t_parsingTokenSetMap firstSetsMap = ParserUtility::computeFirstSets(grammar);
    t_parsingTokenSetMap followSetsMap = ParserUtility::computeFollowSets(grammar, startSymbol, firstSetsMap);

    constructNonTerminalIndexMapper();
    constructTerminalIndexMapper();

    this->parsingTable.resize(this->nonTerminalToIndexMapper.size(), vector<t_prodAlt>(this->terminalToIndexMapper.size()));

    return constructParsingTable(grammar, firstSetsMap, followSetsMap);
}

bool ParsingTable::constructParsingTable(t_grammar &grammar, t_parsingTokenSetMap &firstSetsMap,
                                         t_parsingTokenSetMap &followSetsMap) {

    for(const pair<ParsingToken, t_prodRule>& grammarEntry: grammar) {
        ParsingToken nonTerminal = grammarEntry.first;
        int nonTerminalIndex = getNonTerminalIndex(nonTerminal);

        if(ParserUtility::setContainsEpsilon(firstSetsMap[nonTerminal])) {
            addEpsilonToFollowSet(nonTerminal, followSetsMap[nonTerminal]);
        }
        else {
            addSyncToFollowSet(nonTerminal, followSetsMap[nonTerminal]);
        }

        for(t_prodAlt prodAlt: grammarEntry.second) {
            if(prodAlt.size() > 0) {
                ParsingToken firstToken = prodAlt[0];

                if(firstToken == EPSILON_TOKEN) {
                    continue;
                }

                if(firstToken.getIsTerminal()) {
                    int terminalIndex = getTerminalIndex(firstToken);

                    if(!isEmptyCell(nonTerminalIndex, terminalIndex)) {
                        throw runtime_error("The grammar is not LL(1) because the cell of non-terminal " + nonTerminal.getToken() +
                                            " and terminal " + firstToken.getToken() + " is not empty.");
                    }

                    this->parsingTable[nonTerminalIndex][terminalIndex] = prodAlt;
                }
                else {
                    for(ParsingToken tokenInFirstSet: firstSetsMap[firstToken]) {
                        if(tokenInFirstSet == EPSILON_TOKEN) {
                            continue;
                        }

                        int terminalIndex = getTerminalIndex(tokenInFirstSet);

                        if(!isEmptyCell(nonTerminalIndex, terminalIndex)) {
                            throw runtime_error("The grammar is not LL(1) because the cell of non-terminal " + nonTerminal.getToken() +
                                                " and terminal " + firstToken.getToken() + " is not empty.");
                        }

                        this->parsingTable[nonTerminalIndex][terminalIndex] = prodAlt;
                    }
                }
            }
        }
    }

    return true;
}

t_prodAlt ParsingTable::move(ParsingToken nonTerminal, ParsingToken terminal) {
    int nonTerminalIndex = getNonTerminalIndex(nonTerminal);
    int terminalIndex = getTerminalIndex(terminal);

    if(nonTerminalIndex == NOT_FOUND) {
        return {};
    }

    if(terminalIndex == NOT_FOUND) {
        return {};
    }

    return this->parsingTable[nonTerminalIndex][terminalIndex];
}

bool ParsingTable::writeFile(string &filePath) {
    ofstream outFile(filePath);

    if (!outFile) {
        cerr << "Error opening file for writing the parsing table! " + filePath << endl;
        return false;
    }

    for(const pair<ParsingToken, int>& nonTerminalPair: this->nonTerminalToIndexMapper) {
        outFile << nonTerminalPair.first.getToken() << " " << nonTerminalPair.first.getIsTerminal() << " " << nonTerminalPair.second << endl;
    }
    outFile << endl;

    for(const pair<ParsingToken, int>& terminalPair: this->terminalToIndexMapper) {
        outFile << terminalPair.first.getToken() << " " << terminalPair.first.getIsTerminal() << " " << terminalPair.second << endl;
    }
    outFile << endl;

    outFile << nonTerminalToIndexMapper.size() << " " << terminalToIndexMapper.size() << endl;

    for(int i = 0; i < this->parsingTable.size(); i++) {
        for(int j = 0; j < this->parsingTable[i].size(); j++) {
            outFile << i << " " << j;

            for(ParsingToken parsingToken: this->parsingTable[i][j]) {
                outFile << " " << parsingToken.getToken() << " " << parsingToken.getIsTerminal();
            }

            outFile << endl;
        }
    }

    outFile.close();

    return true;
}

bool ParsingTable::readFile(string &filePath) {

    return true;
}

void ParsingTable::constructNonTerminalIndexMapper() {
    for(const pair<ParsingToken, t_prodRule>& grammarEntry: this->grammar) {
        if(!grammarEntry.first.getIsTerminal() && this->nonTerminalToIndexMapper.count(grammarEntry.first) == 0) {
            this->nonTerminalToIndexMapper[grammarEntry.first] = this->nonTerminalToIndexMapper.size();
        }
    }
}

void ParsingTable::constructTerminalIndexMapper() {
    for(const pair<ParsingToken, t_prodRule>& grammarEntry: this->grammar) {
        for(t_prodAlt prodAlt: grammarEntry.second) {
            for(ParsingToken parsingToken: prodAlt) {

                if(parsingToken.getIsTerminal() && parsingToken != EPSILON_TOKEN && parsingToken != SYNC_TOKEN) {
                    if(this->terminalToIndexMapper.count(parsingToken) == 0) {
                        this->terminalToIndexMapper[parsingToken] = this->terminalToIndexMapper.size();
                    }
                }

            }
        }
    }

    if(this->terminalToIndexMapper.count(END_TOKEN) == 0) {
        this->terminalToIndexMapper[END_TOKEN] = this->terminalToIndexMapper.size();
    }
}

int ParsingTable::getNonTerminalIndex(ParsingToken nonTerminal) {
    if(!nonTerminal.getIsTerminal() && this->nonTerminalToIndexMapper.count(nonTerminal) != 0) {
        return this->nonTerminalToIndexMapper[nonTerminal];
    }
    else {
        return NOT_FOUND;
    }
}

int ParsingTable::getTerminalIndex(ParsingToken terminal) {
    if(terminal.getIsTerminal() && this->terminalToIndexMapper.count(terminal) != 0 && terminal != EPSILON_TOKEN && terminal != SYNC_TOKEN) {
        return this->terminalToIndexMapper[terminal];
    }
    else {
        return NOT_FOUND;
    }
}

void ParsingTable::addEpsilonToFollowSet(ParsingToken nonTerminal, t_parsingTokenSet followSet) {
    int nonTerminalIndex = getNonTerminalIndex(nonTerminal);

    for(ParsingToken followToken: followSet) {
        int terminalIndex = getTerminalIndex(followToken);

        this->parsingTable[nonTerminalIndex][terminalIndex] = { EPSILON_TOKEN };
    }
}

void ParsingTable::addSyncToFollowSet(ParsingToken nonTerminal, t_parsingTokenSet followSet) {
    int nonTerminalIndex = getNonTerminalIndex(nonTerminal);

    for(ParsingToken followToken: followSet) {
        int terminalIndex = getTerminalIndex(followToken);

        this->parsingTable[nonTerminalIndex][terminalIndex] = { SYNC_TOKEN };
    }
}

bool ParsingTable::isEmptyCell(int nonTerminalIndex, int terminalIndex) {
    return this->parsingTable[nonTerminalIndex][terminalIndex].size() == 0;
}