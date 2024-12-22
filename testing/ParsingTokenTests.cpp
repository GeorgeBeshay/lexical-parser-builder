#include <gtest/gtest.h>
#include <unordered_map>

#include "../src/parser/utilities/ParsingToken.h"
#include "../src/parser/utilities/types.h"

using namespace std;

TEST(ParsingTokenTest, checkEquality) {
    ParsingToken parsingToken("E", false, false);
    ParsingToken parsingToken2("E", false, false);
    ParsingToken parsingToken3("T", false, false);

    EXPECT_EQ(parsingToken, parsingToken2);
    EXPECT_NE(parsingToken, parsingToken3);
}

TEST(ParsingTokenTest, checkHash) {
    ParsingToken parsingToken("E", false, false);
    ParsingToken parsingToken2("E", false, false);
    ParsingToken parsingToken3("T", false, false);

    // To make the key of unordered_map as ParsingToken, you must pass the ParsingTokenHash
    // to tell the unordered_map how to do the hash of ParsingToken
    unordered_map<ParsingToken, int, ParsingTokenHash> countOfTokens;
    countOfTokens[parsingToken]++;
    countOfTokens[parsingToken2]++;
    countOfTokens[parsingToken3]++;

    EXPECT_EQ(countOfTokens[parsingToken], 2);
    EXPECT_EQ(countOfTokens[parsingToken3], 1);
}

TEST(ParsingTokenTest, checkGrammarConstruction) {
    ParsingToken parsingToken("T", false, false);
    ParsingToken parsingToken2("if", true, false);

    production_alternative productionAlternative;
    productionAlternative.push_back(parsingToken);
    productionAlternative.push_back(parsingToken2);

    production_alternative productionAlternative2;
    productionAlternative2.push_back(parsingToken);
    productionAlternative2.push_back(parsingToken2);

    production_rule productionRule;
    productionRule.insert(productionAlternative);
    productionRule.insert(productionAlternative2);

    EXPECT_EQ(productionRule.size(), 1);
}