#include <gtest/gtest.h>
#include <unordered_map>

#include "../src/parser/utilities/ParsingToken.h"
#include "../src/parser/utilities/parsingTypes.h"
#include "../src/parser/utilities/ParserUtility.h"

using namespace std;

TEST(ParsingFirstFollowSetsTests, simpleTestcase) {
    // Prepare
    ParsingToken nonTerminal("E", false);
    ParsingToken aTerminal("a", true);
    ParsingToken bTerminal("b", true);
    ParsingToken epsTerminal = EPSILON_TOKEN;

    t_prodRule prodRule({
        {aTerminal},
        {bTerminal},
        {epsTerminal}
    });

    t_grammar grammar;
    grammar[nonTerminal] = prodRule;

    // Act
    t_parsingTokenSetMap firstSetsMap = ParserUtility::computeFirstSets(grammar);
    t_parsingTokenSetMap followSetsMap = ParserUtility::computeFollowSets(grammar, nonTerminal, firstSetsMap);

    // Check
    EXPECT_EQ(1, firstSetsMap.size());
    EXPECT_EQ(3, firstSetsMap[nonTerminal].size());
    EXPECT_EQ(1, firstSetsMap[nonTerminal].count(aTerminal));
    EXPECT_EQ(1, firstSetsMap[nonTerminal].count(bTerminal));
    EXPECT_EQ(1, firstSetsMap[nonTerminal].count(epsTerminal));

    EXPECT_EQ(1, followSetsMap.size());
    EXPECT_EQ(1, followSetsMap[nonTerminal].size());
    EXPECT_EQ(1, followSetsMap[nonTerminal].count(END_TOKEN));
}

TEST(ParsingFirstFollowSetsTests, exampleFromLecture) {
    // Prepare
    ParsingToken nonTerminalE("E", false);
    ParsingToken nonTerminalEDash("EDash", false);
    ParsingToken nonTerminalT("T", false);
    ParsingToken nonTerminalTDash("TDash", false);
    ParsingToken nonTerminalF("F", false);
    ParsingToken plusTerminal("+", true);
    ParsingToken multiplyTerminal("*", true);
    ParsingToken openBracketTerminal("(", true);
    ParsingToken closeBracketTerminal(")", true);
    ParsingToken idTerminal("id", true);
    ParsingToken epsTerminal = EPSILON_TOKEN;

    t_prodRule prodRuleE({
        {nonTerminalT, nonTerminalEDash}
    });

    t_prodRule prodRuleEDash({
        {plusTerminal, nonTerminalT, nonTerminalEDash},
        {epsTerminal}
    });

    t_prodRule prodRuleT({
        {nonTerminalF, nonTerminalTDash}
    });

    t_prodRule prodRuleTDash({
        {multiplyTerminal, nonTerminalF, nonTerminalTDash},
        {epsTerminal}
    });

    t_prodRule prodRuleF({
        {openBracketTerminal, nonTerminalE, closeBracketTerminal},
        {idTerminal}
    });

    t_grammar grammar;
    grammar[nonTerminalE] = prodRuleE;
    grammar[nonTerminalEDash] = prodRuleEDash;
    grammar[nonTerminalT] = prodRuleT;
    grammar[nonTerminalTDash] = prodRuleTDash;
    grammar[nonTerminalF] = prodRuleF;

    ParsingToken endSymbol = END_TOKEN;

    // Act
    t_parsingTokenSetMap firstSetsMap = ParserUtility::computeFirstSets(grammar);
    t_parsingTokenSetMap followSetsMap = ParserUtility::computeFollowSets(grammar, nonTerminalE, firstSetsMap);

    // Check
    EXPECT_EQ(5, firstSetsMap.size());
    EXPECT_EQ(2, firstSetsMap[nonTerminalE].size());
    EXPECT_EQ(2, firstSetsMap[nonTerminalEDash].size());
    EXPECT_EQ(2, firstSetsMap[nonTerminalT].size());
    EXPECT_EQ(2, firstSetsMap[nonTerminalTDash].size());
    EXPECT_EQ(2, firstSetsMap[nonTerminalF].size());

    EXPECT_EQ(1, firstSetsMap[nonTerminalE].count(openBracketTerminal));
    EXPECT_EQ(1, firstSetsMap[nonTerminalE].count(idTerminal));
    EXPECT_EQ(1, firstSetsMap[nonTerminalTDash].count(multiplyTerminal));
    EXPECT_EQ(1, firstSetsMap[nonTerminalTDash].count(epsTerminal));
    EXPECT_EQ(1, firstSetsMap[nonTerminalT].count(openBracketTerminal));
    EXPECT_EQ(1, firstSetsMap[nonTerminalT].count(idTerminal));
    EXPECT_EQ(1, firstSetsMap[nonTerminalEDash].count(plusTerminal));
    EXPECT_EQ(1, firstSetsMap[nonTerminalEDash].count(epsTerminal));
    EXPECT_EQ(1, firstSetsMap[nonTerminalF].count(openBracketTerminal));
    EXPECT_EQ(1, firstSetsMap[nonTerminalF].count(idTerminal));

    EXPECT_EQ(5, followSetsMap.size());
    EXPECT_EQ(2, followSetsMap[nonTerminalE].size());
    EXPECT_EQ(2, followSetsMap[nonTerminalEDash].size());
    EXPECT_EQ(3, followSetsMap[nonTerminalT].size());
    EXPECT_EQ(3, followSetsMap[nonTerminalTDash].size());
    EXPECT_EQ(4, followSetsMap[nonTerminalF].size());

    EXPECT_EQ(1, followSetsMap[nonTerminalE].count(endSymbol));
    EXPECT_EQ(1, followSetsMap[nonTerminalE].count(closeBracketTerminal));
    EXPECT_EQ(1, followSetsMap[nonTerminalEDash].count(endSymbol));
    EXPECT_EQ(1, followSetsMap[nonTerminalEDash].count(closeBracketTerminal));
    EXPECT_EQ(1, followSetsMap[nonTerminalT].count(plusTerminal));
    EXPECT_EQ(1, followSetsMap[nonTerminalT].count(endSymbol));
    EXPECT_EQ(1, followSetsMap[nonTerminalT].count(closeBracketTerminal));
    EXPECT_EQ(1, followSetsMap[nonTerminalTDash].count(plusTerminal));
    EXPECT_EQ(1, followSetsMap[nonTerminalTDash].count(endSymbol));
    EXPECT_EQ(1, followSetsMap[nonTerminalTDash].count(closeBracketTerminal));
    EXPECT_EQ(1, followSetsMap[nonTerminalF].count(plusTerminal));
    EXPECT_EQ(1, followSetsMap[nonTerminalF].count(endSymbol));
    EXPECT_EQ(1, followSetsMap[nonTerminalF].count(closeBracketTerminal));
    EXPECT_EQ(1, followSetsMap[nonTerminalF].count(multiplyTerminal));
}

TEST(ParsingFirstFollowSetsTests, moreComplicatedExample) {
    // Prepare
    ParsingToken nonTerminalE("E", false);
    ParsingToken nonTerminalEDash("EDash", false);
    ParsingToken nonTerminalT("T", false);
    ParsingToken nonTerminalTDash("TDash", false);
    ParsingToken nonTerminalF("F", false);
    ParsingToken nonTerminalFDash("FDash", false);
    ParsingToken nonTerminalP("P", false);
    ParsingToken plusTerminal("+", true);
    ParsingToken multiplyTerminal("*", true);
    ParsingToken openBracketTerminal("(", true);
    ParsingToken closeBracketTerminal(")", true);
    ParsingToken aTerminal("a", true);
    ParsingToken bTerminal("b", true);
    ParsingToken epsTerminal = EPSILON_TOKEN;
    ParsingToken endSymbol = END_TOKEN;

    t_prodRule prodRuleE({
        {nonTerminalT, nonTerminalEDash}
    });

    t_prodRule prodRuleEDash({
        {plusTerminal, nonTerminalE},
        {epsTerminal}
    });

    t_prodRule prodRuleT({
        {nonTerminalF, nonTerminalTDash}
    });

    t_prodRule prodRuleTDash({
        {epsTerminal}
    });

    t_prodRule prodRuleF({
        {nonTerminalP, nonTerminalFDash}
    });

    t_prodRule prodRuleFDash ({
        {multiplyTerminal, nonTerminalF},
        {epsTerminal}
    });

    t_prodRule prodRuleP ({
        {openBracketTerminal, nonTerminalE, closeBracketTerminal},
        {aTerminal},
        {bTerminal},
        {epsTerminal}
    });

    t_grammar grammar;
    grammar[nonTerminalE] = prodRuleE;
    grammar[nonTerminalEDash] = prodRuleEDash;
    grammar[nonTerminalT] = prodRuleT;
    grammar[nonTerminalTDash] = prodRuleTDash;
    grammar[nonTerminalF] = prodRuleF;
    grammar[nonTerminalFDash] = prodRuleFDash;
    grammar[nonTerminalP] = prodRuleP;

    // Act
    t_parsingTokenSetMap firstSetsMap = ParserUtility::computeFirstSets(grammar);
    t_parsingTokenSetMap followSetsMap = ParserUtility::computeFollowSets(grammar, nonTerminalE, firstSetsMap);

    // Check
    EXPECT_EQ(7, firstSetsMap.size());
    EXPECT_EQ(6, firstSetsMap[nonTerminalE].size());
    EXPECT_EQ(2, firstSetsMap[nonTerminalEDash].size());
    EXPECT_EQ(5, firstSetsMap[nonTerminalT].size());
    EXPECT_EQ(1, firstSetsMap[nonTerminalTDash].size());
    EXPECT_EQ(5, firstSetsMap[nonTerminalF].size());
    EXPECT_EQ(2, firstSetsMap[nonTerminalFDash].size());
    EXPECT_EQ(4, firstSetsMap[nonTerminalP].size());

    EXPECT_EQ(7, followSetsMap.size());
    EXPECT_EQ(2, followSetsMap[nonTerminalE].size());
    EXPECT_EQ(2, followSetsMap[nonTerminalEDash].size());
    EXPECT_EQ(3, followSetsMap[nonTerminalT].size());
    EXPECT_EQ(3, followSetsMap[nonTerminalTDash].size());
    EXPECT_EQ(3, followSetsMap[nonTerminalF].size());
    EXPECT_EQ(3, followSetsMap[nonTerminalFDash].size());
    EXPECT_EQ(4, followSetsMap[nonTerminalP].size());
}