#include <gtest/gtest.h>
#include <string>
#include <fstream>

#define EXECUTABLE_NAME "lexical-analyzer.exe "
#define TESTING_DIRECTORY "../testing/lexical-analyzer-tests/"

using namespace std;

bool areFilesEqual(string file1, string file2);

TEST(LexicalAnalyzerTest, simpleTestProgram) {
    // Arrange
    string testName = "test-1/";
    string transitionTableFilePath = TESTING_DIRECTORY + testName + "transitionTable.txt";
    string inputFilePath = TESTING_DIRECTORY + testName + "input.txt";
    string outputFilePath = TESTING_DIRECTORY + testName + "output.txt";
    string symbolTableFilePath = TESTING_DIRECTORY + testName + "symbolTable.txt";
    string expectedOutputFilePath = TESTING_DIRECTORY + testName + "expectedOutput.txt";
    string expectedSymbolTableFilePath = TESTING_DIRECTORY + testName + "expectedSymbolTable.txt";

    string command = EXECUTABLE_NAME + transitionTableFilePath + " " + inputFilePath +
            " " + outputFilePath + " " + symbolTableFilePath;

    // Act
    system(command.c_str());

    // Assert
    EXPECT_EQ(true, areFilesEqual(outputFilePath, expectedOutputFilePath));
    EXPECT_EQ(true, areFilesEqual(symbolTableFilePath, expectedSymbolTableFilePath));
}


bool areFilesEqual(string file1, string file2) {
    ifstream file1Stream(file1);
    ifstream file2Stream(file2);

    if (!file1Stream.is_open() || !file2Stream.is_open()) {
        cerr << "Error opening one of the files." << endl;
        return false;
    }

    char ch1, ch2;
    while (file1Stream.get(ch1) && file2Stream.get(ch2)) {
        if (ch1 != ch2) {
            return false;
        }
    }

    return true;
}