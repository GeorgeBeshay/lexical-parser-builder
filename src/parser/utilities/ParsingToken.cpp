#include "ParsingToken.h"

ParsingToken::ParsingToken(string token, bool isTerminal) {
    this->token = token;
    this->isTerminal = isTerminal;
}

string ParsingToken::getToken() const {
    return this->token;
}

bool ParsingToken::getIsTerminal() const {
    return this->isTerminal;
}

bool ParsingToken::isEpsilon() const {
    return this->getIsTerminal() && this->getToken() == "\\L";
}

bool ParsingToken::operator==(const ParsingToken& other) const {
    return token == other.getToken() && isTerminal == other.getIsTerminal();
}

bool ParsingToken::operator!=(const ParsingToken &other) const {
    return !(*this == other);
}