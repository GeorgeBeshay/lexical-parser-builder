#ifndef LEXICALPARSER_REGEXSCANNER_H
#define LEXICALPARSER_REGEXSCANNER_H

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;

class RegexScanner {

private:
    vector<pair<string, string>> regularDefinitions;
    vector<pair<string, string>> regularExpressions;
    vector<string> reservedKeywords;
    vector<string> reservedPunctuations;

    void readFile(string& filePath);
    void interpretRule(string& line);

public:
    RegexScanner(string& filePath) {
        readFile(filePath);
    }

    const vector<pair<string, string>>& getRegularDefinitions() { return regularDefinitions; }

    const vector<pair<string, string>>& getRegularExpressions() { return regularExpressions; }

    const vector<string>& getReservedKeywords() { return reservedKeywords; }

    const vector<string>& getReservedPunctuations() { return reservedPunctuations; }

    void printOutput();
};


#endif //LEXICALPARSER_REGEXSCANNER_H
