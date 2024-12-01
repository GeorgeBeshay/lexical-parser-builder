#include "RegexScanner.h"

#define DEFINITION_REGEX     regex(R"(^\s*(\w+)\s*=\s*(.+)$)")
#define EXPRESSION_REGEX     regex(R"(^\s*(\w+)\s*:\s*(.+)$)")
#define KEYWORDS_REGEX       regex(R"(\{(.*)\})")
#define PUNCTUATIONS_REGEX   regex(R"(\[(.*)\])")

void RegexScanner::readFile(string &filePath) {
    ifstream file(filePath);

    if (!file) {
        throw runtime_error("Failed to open file: " + filePath);
    }

    string line;
    while (getline(file, line)) {
        interpretRule(line);
    }
}

void RegexScanner::interpretRule(std::string &line) {
    smatch match;

    if (regex_match(line, match, DEFINITION_REGEX)) {
        regularDefinitions.emplace_back(match[1].str(), match[2].str());
    }

    else if (regex_match(line, match, EXPRESSION_REGEX)) {
        regularExpressions.emplace_back(match[1].str(), match[2].str());
    }

    else if (regex_search(line, match, PUNCTUATIONS_REGEX)) {
        istringstream iss(match[1].str());
        string punctuation;

        while (iss >> punctuation) {
            reservedPunctuations.emplace_back(punctuation);
        }
    }

    else if (regex_search(line, match, KEYWORDS_REGEX)) {
        istringstream iss(match[1].str());
        string keyword;

        while (iss >> keyword) {
            reservedKeywords.emplace_back(keyword);
        }
    }

    else {
        cerr << "This line does not match with anything: " << line << endl;
    }
}

void RegexScanner::printOutput() {
    cout << "Regular Definitions (in order):\n";
    for (const auto& [name, def] : regularDefinitions) {
        cout << "  " << name << " = " << def << endl;
    }
    cout << "\nRegular Expressions (in order):\n";
    for (const auto& [name, expr] : regularExpressions) {
        cout << "  " << name << " : " << expr << endl;
    }
    cout << "\nReserved Keywords:\n";
    for (const auto& keyword : reservedKeywords) {
        cout << "  " << keyword << endl;
    }
    cout << "\nPunctuations:\n";
    for (const auto& punctuation : reservedPunctuations) {
        cout << "  " << punctuation << endl;
    }
}