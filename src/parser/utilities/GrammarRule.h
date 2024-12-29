#ifndef LEXICAL_PARSER_BUILDER_GRAMMARRULE_H
#define LEXICAL_PARSER_BUILDER_GRAMMARRULE_H

#include "types.h"
#include "ParsingToken.h"
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

class Rule {
private:
    ParsingToken lhs;
    t_prodRule productions;

public:
    Rule() = default;

    explicit Rule(ParsingToken &, t_prodRule &);

    const ParsingToken &getLhs() const;

    const t_prodRule &getProductions() const;

    string toString();

    void clearProductions();

    void setProductions(t_prodRule &);

    void addProductions(t_prodRule &);
};

#endif // LEXICAL_PARSER_BUILDER_GRAMMARRULE_H
