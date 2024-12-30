

#ifndef LEXICAL_PARSER_BUILDER_GRAMMARUTILITY_H
#define LEXICAL_PARSER_BUILDER_GRAMMARUTILITY_H



#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

namespace GrammarUtility {
    bool trimTerminalQuotes(string &str);

    void trimSpaces(string &str);

    void stringToVec(vector<string> &vec, string &str, char delimiter);
};




#endif //LEXICAL_PARSER_BUILDER_GRAMMARUTILITY_H
