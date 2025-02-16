#ifndef LEXICAL_PARSER_BUILDER_PARSINGTABLE_H
#define LEXICAL_PARSER_BUILDER_PARSINGTABLE_H

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../utilities/ParsingToken.h"
#include "../utilities/ParserUtility.h"

#define NOT_FOUND (-1)

using namespace std;

class ParsingTable {
private:
    ParsingToken startSymbol;
    t_grammar grammar;
    vector<vector<t_prodAlt>> parsingTable;
    unordered_map<ParsingToken, int, ParsingTokenHash> nonTerminalToIndexMapper;
    unordered_map<ParsingToken, int, ParsingTokenHash> terminalToIndexMapper;

    bool importParsingTableFromFile(string& filePath);

    bool initializeParsingTable(ParsingToken startSymbol, t_grammar grammar);

    bool constructParsingTable(t_grammar& grammar, t_parsingTokenSetMap& firstSetsMap, t_parsingTokenSetMap& followSetsMap);

    void constructNonTerminalIndexMapper();
    void constructTerminalIndexMapper();

    int getNonTerminalIndex(ParsingToken nonTerminal);
    int getTerminalIndex(ParsingToken terminal);

    void addEpsilonTransInFollowSetEntries(ParsingToken nonTerminal, t_parsingTokenSet followSet);
    void addSyncTransInFollowSetEntries(ParsingToken nonTerminal, t_parsingTokenSet followSet);

    bool isEmptyCell(int nonTerminalIndex, int terminalIndex);

public:
    explicit ParsingTable(string& filePath) {
        if(!importParsingTableFromFile(filePath)) {
            throw runtime_error("There is a problem in reading the parsing table file: " + filePath);
        }
    }

    ParsingTable(ParsingToken startSymbol, t_grammar grammar) : startSymbol(startSymbol), grammar(grammar) {
        if(!initializeParsingTable(startSymbol, grammar)) {
            throw runtime_error("There is a problem in constructing the parsing table.");
        }
    }

    bool exportParsingTableToFile(string& filePath);

    ParsingToken getStartSymbol() { return startSymbol; }

    t_prodAlt move(ParsingToken nonTerminal, ParsingToken terminal);
};


#endif //LEXICAL_PARSER_BUILDER_PARSINGTABLE_H
