#ifndef LEXICAL_PARSER_BUILDER_PARSINGTOKEN_H
#define LEXICAL_PARSER_BUILDER_PARSINGTOKEN_H

#include <string>

using namespace std;

class ParsingToken {

private:
    string token;
    bool isTerminal;
    bool isEpsilon;

public:
    ParsingToken(string token, bool isTerminal, bool isEpsilon);
    string getToken();
    bool getIsTerminal();
    bool getIsEpsilon();
};


#endif //LEXICAL_PARSER_BUILDER_PARSINGTOKEN_H
