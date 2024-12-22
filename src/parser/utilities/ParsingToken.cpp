#include "ParsingToken.h"

ParsingToken::ParsingToken(string token, bool isTerminal) {
    this->token = token;
    this->isTerminal = isTerminal;
}

string ParsingToken::getToken() {
    return this->token;
}

bool ParsingToken::getIsTerminal() {
    return this->isTerminal;
}

bool ParsingToken::getIsEpsilon() {
    return this->getIsTerminal() && this->getToken() == "\\L";
}

bool ParsingToken::operator==(const ParsingToken& other) const {
    return token == other.token &&
           isTerminal == other.isTerminal;
}

bool ParsingToken::operator!=(const ParsingToken &other) const {
    return !(*this == other);
}