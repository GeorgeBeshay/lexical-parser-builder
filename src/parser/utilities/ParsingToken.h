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
    bool operator!=(const ParsingToken& other) const;
    friend struct ParsingTokenHash; // To be able to access the private members of class
};

struct ParsingTokenHash {
    std::size_t operator()(const ParsingToken& pt) const {
        return std::hash<std::string>()(pt.token) ^
               (std::hash<bool>()(pt.isTerminal) << 1) ^
               (std::hash<bool>()(pt.isEpsilon) << 2);
    }
};

struct ParsingTokenVectorHash {
    std::size_t operator()(const std::vector<ParsingToken>& vec) const {
        std::size_t hash_value = 0;
        for (const auto& token : vec) {
            hash_value ^= ParsingTokenHash{}(token) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
        }
        return hash_value;
    }
};


#endif //LEXICAL_PARSER_BUILDER_PARSINGTOKEN_H
