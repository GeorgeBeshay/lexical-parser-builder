#include "GrammarUtility.h"

// Splits a string `str` into a vector `vec` by using `delimiter` as the separator
void GrammarUtility::stringToVec(vector<string> &vec, string &str, char delimiter) {
    string modifiedStr;
    bool insideTerminal = false;  // Flag to track if we are inside a terminal

    for (char i: str) {
        if (i == '\'') {
            // Whenever we encounter a single quote, toggle insideTerminal flag
            modifiedStr += insideTerminal ? "' " : " '";
            insideTerminal = !insideTerminal;  // Flip the state
        } else
            // Add other characters as they are
            modifiedStr += i;
    }

    stringstream ssStr(modifiedStr);
    string token;
    while (getline(ssStr, token, delimiter))  // While there are tokens separated by delimiter
        vec.push_back(token);
}

bool GrammarUtility::trimTerminalQuotes(string &str) {
    if (str[0] == '\'' && str[str.length() - 1] == '\'') {  // Check if the first and last characters are single quotes
        str.erase(remove(str.begin(), str.end(), '\''), str.end());  // Remove the single quotes from the string
        return true;  // Return true, indicating that the quotes were removed
    }
    return false;  // Return false if there are no surrounding single quotes
}

// Trims leading and trailing spaces from the given string
void GrammarUtility::trimSpaces(string &str) {
    // Trim leading spaces.
    str.erase(str.begin(), find_if(str.begin(), str.end(), [](int character) {
        return !isspace(character);  // Find the first non-whitespace character
    }));
    // Trim trailing spaces.
    str.erase(find_if(str.rbegin(), str.rend(), [](int character) {
        return !isspace(character);  // Find the first non-whitespace character from the end
    }).base(), str.end());
}

