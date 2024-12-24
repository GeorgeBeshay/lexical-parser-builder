#ifndef LEXICAL_PARSER_BUILDER_PARSINGANALYZER_H
#define LEXICAL_PARSER_BUILDER_PARSINGANALYZER_H

#include <stack>
#include "parsing-table/ParsingTable.h"

#define ERROR_TERMINAL_CASE(expectedToken) ("Error: Input token mismatch, expected a " + expectedToken + " instead. Recovery: inserting " + expectedToken + ".")
#define ERROR_NT_EMPTY_ENTRY_CASE(stackTopElement, inputToken) ("Error: M[" + stackTopElement + ", " + inputToken + "] entry in parsing table is empty. Recovery: Will skip this input token.")
#define ERROR_NT_SYNC_ENTRY_CASE(stackTopElement, inputToken) ("Error: M[" + stackTopElement + ", " + inputToken + "] entry in parsing table is SYNC. Recovery: Will pop and neglect the stack top element.")
#define MATCHED_PARSING_STEP true
#define NON_MATCHED_PARSING_STEP false

class ParsingAnalyzer {

public:
    explicit ParsingAnalyzer(string& parsingTableFp);
    bool doParseStep(const ParsingToken& nextInputToken);
    bool exportDerivation(const string& destinationFp);

private:
    ParsingTable parsingTable;
    stack<ParsingToken> parsingStack;
    vector<pair<ParsingToken, t_prodAlt>> outputProds;
    string currentDerivation;
    vector<string> outputFileContent;

    void appendTokenToCurrentDerivation(const string& newToken);
    bool replaceFirstTokenInCurrentDerivation(const string& oldToken, t_prodAlt prodAlt);

};


#endif //LEXICAL_PARSER_BUILDER_PARSINGANALYZER_H
