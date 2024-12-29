#include <gtest/gtest.h>
#include <unordered_map>
#include <string>

#include "../src/parser/utilities/ParsingToken.h"
#include "../src/parser/utilities/parsingTypes.h"
#include "../src/parser/parsing-table/ParsingTable.h"

using namespace std;

bool areFilesEqual(const string& file1, const string& file2);

TEST(ParsingTableTests, simpleTestcase) {
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
    string filePath = "../testing/parsing-table-tests/simple-testcase/writtenFile.txt";
    string expectedFilePath = "../testing/parsing-table-tests/simple-testcase/expectedTable.txt";

    // Act
    ParsingTable parsingTable(nonTerminal, grammar);
    parsingTable.writeFile(filePath);

    ParsingTable parsingTable1(filePath);
    parsingTable1.writeFile(filePath);

    EXPECT_EQ(true, areFilesEqual(filePath, expectedFilePath));
}

TEST(ParsingTableTests, lectureExample) {
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

    string filePath = "../testing/parsing-table-tests/lecture-example/writtenFile.txt";
    string expectedFilePath = "../testing/parsing-table-tests/lecture-example/expectedTable.txt";

    // Act
    ParsingTable parsingTable(nonTerminalE, grammar);
    parsingTable.writeFile(filePath);

    ParsingTable parsingTable1(filePath);
    parsingTable1.writeFile(filePath);

    EXPECT_EQ(true, areFilesEqual(filePath, expectedFilePath));
}
