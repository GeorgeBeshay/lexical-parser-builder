#include "ParsingToken.h"

ParsingToken::ParsingToken(string token, bool isTerminal, bool isEpsilon) {
    this->token = token;
    this->isTerminal = isTerminal;
    this->isEpsilon = isEpsilon;
}

string ParsingToken::getToken() {
    return this->token;
}

bool ParsingToken::getIsTerminal() {
    return this->isTerminal;
}

bool ParsingToken::getIsEpsilon() {
    return this->isEpsilon;
}