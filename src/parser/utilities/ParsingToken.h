#ifndef LEXICAL_PARSER_BUILDER_PARSINGTOKEN_H
#define LEXICAL_PARSER_BUILDER_PARSINGTOKEN_H

#include <string>
#include <functional>

using namespace std;

class ParsingToken {

private:
    string token;
    bool isTerminal;

public:
    ParsingToken() = default;
    ParsingToken(string token, bool isTerminal);
    string getToken() const;
    bool getIsTerminal() const;
    bool isEpsilon() const;

    bool operator==(const ParsingToken& other) const;
    bool operator!=(const ParsingToken& other) const;
};

struct ParsingTokenHash {
    size_t operator()(const ParsingToken& pt) const {
        return hash<string>()(pt.getToken()) ^
               (hash<bool>()(pt.getIsTerminal()) << 1);
    }
};

struct ParsingTokenVectorHash {
    size_t operator()(const vector<ParsingToken>& vec) const {
        size_t hashValue = 0;
        for (const auto& token : vec) {
            // This operation makes hashing value for vector of ParsingToken, and done by this way for create hash value more distributed and uniform
            hashValue ^= ParsingTokenHash{}(token) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
        }
        return hashValue;
    }
};


#endif //LEXICAL_PARSER_BUILDER_PARSINGTOKEN_H
