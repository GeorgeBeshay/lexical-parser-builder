#include <gtest/gtest.h>
#include "../src/parser/ParsingAnalyzer.h"

using namespace std;

bool areFilesEqual(const string& file1, const string& file2);

TEST(ParsingAnalyzerTests, additionStatement) {
    // Arrange
    string parsingTableFilePath = "../testing/parsing-table-tests/lecture-example/expectedTable.txt";
    string expectedOutputFp = "../testing/parsing-analyzer-tests/output/additionStatement-expected-output.txt";
    string actualOutputFp = "../testing/parsing-analyzer-tests/output/additionStatement-actual-output.txt";
    t_prodAlt inputParsingTokens {
        ParsingToken("id", true),
        ParsingToken("+", true),
        ParsingToken("id", true),
        END_TOKEN
    };

    // Act
    ParsingAnalyzer parsingAnalyzer(parsingTableFilePath);
    for (auto& inputParsingToken: inputParsingTokens) {
        while (!parsingAnalyzer.doParseStep(inputParsingToken));
    }

    // Assert
    parsingAnalyzer.exportDerivation(actualOutputFp);
    EXPECT_TRUE(areFilesEqual(actualOutputFp, expectedOutputFp));

}

TEST(ParsingAnalyzerTests, additionAndMultiplicationStatement) {
    // Arrange
    string parsingTableFilePath = "../testing/parsing-table-tests/lecture-example/expectedTable.txt";
    string expectedOutputFp = "../testing/parsing-analyzer-tests/output/additionAndMultiplicationStatement-expected-output.txt";
    string actualOutputFp = "../testing/parsing-analyzer-tests/output/additionAndMultiplicationStatement-actual-output.txt";
    t_prodAlt inputParsingTokens {
        ParsingToken("id", true),
        ParsingToken("+", true),
        ParsingToken("id", true),
        ParsingToken("*", true),
        ParsingToken("id", true),
        END_TOKEN
    };

    // Act
    ParsingAnalyzer parsingAnalyzer(parsingTableFilePath);
    for (auto& inputParsingToken: inputParsingTokens) {
        while (!parsingAnalyzer.doParseStep(inputParsingToken));
    }

    // Assert
    parsingAnalyzer.exportDerivation(actualOutputFp);
    EXPECT_TRUE(areFilesEqual(actualOutputFp, expectedOutputFp));

}

TEST(ParsingAnalyzerTests, containingTwoErrors) {
    // Arrange
    string parsingTableFilePath = "../testing/parsing-table-tests/lecture-example/expectedTable.txt";
    string expectedOutputFp = "../testing/parsing-analyzer-tests/output/containingTwoErrors-expected-output.txt";
    string actualOutputFp = "../testing/parsing-analyzer-tests/output/containingTwoErrors-actual-output.txt";
    t_prodAlt inputParsingTokens {
        ParsingToken("(", true),
        ParsingToken("id", true),
        ParsingToken("+", true),
        ParsingToken("(", true),
        ParsingToken("*", true),
        ParsingToken("id", true),
        ParsingToken(")", true),
        END_TOKEN
    };

    // Act
    ParsingAnalyzer parsingAnalyzer(parsingTableFilePath);
    for (auto& inputParsingToken: inputParsingTokens) {
        while (!parsingAnalyzer.doParseStep(inputParsingToken));
    }

    // Assert
    parsingAnalyzer.exportDerivation(actualOutputFp);
    EXPECT_TRUE(areFilesEqual(actualOutputFp, expectedOutputFp));

}