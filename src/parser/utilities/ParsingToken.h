#ifndef LEXICAL_PARSER_BUILDER_PARSINGTOKEN_H
#define LEXICAL_PARSER_BUILDER_PARSINGTOKEN_H

#include <string>
#include <functional>

using namespace std;

class ParsingToken {

private:
    string token;
    bool isTerminal;
    bool isEpsilon;

public:
    ParsingToken() = default;
    ParsingToken(string token, bool isTerminal, bool isEpsilon);
    string getToken();
    bool getIsTerminal();
    bool getIsEpsilon();

    bool operator==(const ParsingToken& other) const;
    friend struct ParsingTokenHash;
};

struct ParsingTokenHash {
    std::size_t operator()(const ParsingToken& pt) const {
        return std::hash<std::string>()(pt.token) ^
               (std::hash<bool>()(pt.isTerminal) << 1) ^
               (std::hash<bool>()(pt.isEpsilon) << 2);
    }
};


#endif //LEXICAL_PARSER_BUILDER_PARSINGTOKEN_H
