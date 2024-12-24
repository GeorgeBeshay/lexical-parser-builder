#ifndef LEXICAL_PARSER_BUILDER_PARSINGTABLE_H
#define LEXICAL_PARSER_BUILDER_PARSINGTABLE_H

#include <unordered_map>

#include "../utilities/ParsingToken.h"
#include "../utilities/ParserUtility.h"

using namespace std;

class ParsingTable {
private:
    ParsingToken startSymbol;
    t_grammar grammar;
    vector<vector<t_prodAlt>> parsingTable;
    unordered_map<ParsingToken, int, ParsingTokenHash> nonTerminalToIndexMapper;
    unordered_map<ParsingToken, int, ParsingTokenHash> terminalToIndexMapper;

    bool readFile(string& filePath);

    bool constructParsingTable(ParsingToken startSymbol, t_grammar grammar);

public:
    ParsingTable(string& filePath) {
        if(!readFile(filePath)) {
            throw runtime_error("There is a problem in reading the parsing table file: " + filePath);
        }
    }

    ParsingTable(ParsingToken startSymbol, t_grammar grammar) {
        if(!constructParsingTable(startSymbol, grammar)) {
            throw runtime_error("There is a problem in constructing the parsing table.");
        }
    }

    bool writeFile(string& filePath);

    ParsingToken getStartSymbol() { return startSymbol; }

    t_prodAlt move(ParsingToken nonTerminal, ParsingToken terminal);
};


#endif //LEXICAL_PARSER_BUILDER_PARSINGTABLE_H
