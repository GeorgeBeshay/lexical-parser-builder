#include <iostream>
#include <bits/stdc++.h>

#include "RegexScanner.h"

using namespace std;

int main() {
    string filePath = "../../inputs/LexicalRulesInput.txt";
    RegexScanner regexScanner(filePath);
    regexScanner.printOutput();
    return 0;
}
