#ifndef COMBINEDNFAGENERATOR_H
#define COMBINEDNFAGENERATOR_H

#include "NFAGenerator.h"

class CombinedNFAGenerator {
public:
    static NFAGenerator::NFA combineNFAs(const std::vector<NFAGenerator::NFA>& nfas);
};

#endif // COMBINEDNFAGENERATOR_H
