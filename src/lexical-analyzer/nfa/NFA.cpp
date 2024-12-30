#include "NFA.h"


int SingleNFA::stateCounter = 0;

// NFA constructor
NFA::NFA(int start, int accept) : startState(start), acceptState(accept) {}

CombinedNFA::CombinedNFA() : NFA(-1, -1) {}

// Add regular transition
void NFA::addTransition(int from, char symbol, int to) {
    transitions[from][symbol].insert(to);
}

// Add epsilon transition
void NFA::addEpsilonTransition(int from, int to) {
    epsilonTransitions[from].insert(to);
}

void CombinedNFA::addInitialState(int state) {
    initialStates.insert(state);
}

void CombinedNFA::addSymbol(char symbol) {
    symbols.insert(symbol);
}

CombinedNFA CombinedNFA::combineNFA(const vector<NFA> &nfas) {
    CombinedNFA combinedNFA;
    int newStart = SingleNFA::stateCounter++;


    combinedNFA.addInitialState(newStart);
    combinedNFA.startState = newStart;


    for (const auto &nfa: nfas) {

        //extract symbols from nfa transitions
        for (const auto &[from, transMap]: nfa.transitions) {
            for (const auto &[symbol, toStates]: transMap) {
                combinedNFA.addSymbol(symbol);
            }
        }

        combinedNFA.addEpsilonTransition(newStart, nfa.startState);
        combinedNFA.transitions.insert(nfa.transitions.begin(), nfa.transitions.end());
        combinedNFA.epsilonTransitions.insert(nfa.epsilonTransitions.begin(), nfa.epsilonTransitions.end());
        combinedNFA.acceptStateClasses.insert(nfa.acceptStateClasses.begin(), nfa.acceptStateClasses.end());
    }

    return combinedNFA;
}

// Helper to create a basic NFA for a single character
static NFA createBasicNFA(char symbol) {
    int start = SingleNFA::stateCounter++;
    int accept = SingleNFA::stateCounter++;
    NFA nfa(start, accept);
    nfa.addTransition(start, symbol, accept);
    return nfa;
}

static NFA createBasicEpsilonNFA() {
    int start = SingleNFA::stateCounter++;
    int accept = SingleNFA::stateCounter++;
    NFA nfa(start, accept);
    nfa.addEpsilonTransition(start, accept);
    return nfa;
}

static NFA createRangeNFA(char start, char end) {
    int startState = SingleNFA::stateCounter++;
    int acceptState = SingleNFA::stateCounter++;
    NFA nfa(startState, acceptState);
    for (char ch = start; ch <= end; ++ch) {
        nfa.addTransition(startState, ch, acceptState);
    }
    return nfa;
}

// Handle Kleene star (a*)
NFA SingleNFA::handleKleeneStar(const NFA &nfa) {
    int newStart = stateCounter++;
    int newAccept = stateCounter++;

    NFA result(newStart, newAccept);

    // Add epsilon transitions for Kleene star logic
    result.addEpsilonTransition(newStart, nfa.startState);
    result.addEpsilonTransition(nfa.acceptState, nfa.startState);
    result.addEpsilonTransition(newStart, newAccept);
    result.addEpsilonTransition(nfa.acceptState, newAccept);

//    // Copy transitions
    result.transitions.insert(nfa.transitions.begin(), nfa.transitions.end());
    result.epsilonTransitions.insert(nfa.epsilonTransitions.begin(), nfa.epsilonTransitions.end());

    return result;
}

NFA SingleNFA::handlePositiveClosure(const NFA &nfa) {
    int newStart = stateCounter++;
    int newAccept = stateCounter++;

    NFA result(newStart, newAccept);

    // Add epsilon transitions for Kleene star logic
    result.addEpsilonTransition(newStart, nfa.startState);
    result.addEpsilonTransition(nfa.acceptState, nfa.startState);
    result.addEpsilonTransition(nfa.acceptState, newAccept);

    // Copy transitions
    result.transitions.insert(nfa.transitions.begin(), nfa.transitions.end());
    result.epsilonTransitions.insert(nfa.epsilonTransitions.begin(), nfa.epsilonTransitions.end());

    return result;
}

// Handle concatenation (a.b)
NFA SingleNFA::handleConcatenation(const NFA &nfa1, const NFA &nfa2) {
    NFA result(nfa1.startState, nfa2.acceptState);

    result.transitions.insert(nfa1.transitions.begin(), nfa1.transitions.end());
    result.epsilonTransitions.insert(nfa1.epsilonTransitions.begin(), nfa1.epsilonTransitions.end());
    result.transitions.insert(nfa2.transitions.begin(), nfa2.transitions.end());
    result.epsilonTransitions.insert(nfa2.epsilonTransitions.begin(), nfa2.epsilonTransitions.end());

    // Link nfa1's accept t_state to nfa2's start t_state
    result.addEpsilonTransition(nfa1.acceptState, nfa2.startState);

    return result;
}

// Handle union (a|b)
NFA SingleNFA::handleUnion(const NFA &nfa1, const NFA &nfa2) {
    int newStart = stateCounter++;
    int newAccept = stateCounter++;

    NFA result(newStart, newAccept);

    // Add epsilon transitions for the union logic
    result.addEpsilonTransition(newStart, nfa1.startState);
    result.addEpsilonTransition(newStart, nfa2.startState);
    result.addEpsilonTransition(nfa1.acceptState, newAccept);
    result.addEpsilonTransition(nfa2.acceptState, newAccept);

    result.transitions.insert(nfa1.transitions.begin(), nfa1.transitions.end());
    result.epsilonTransitions.insert(nfa1.epsilonTransitions.begin(), nfa1.epsilonTransitions.end());

    result.transitions.insert(nfa2.transitions.begin(), nfa2.transitions.end());
    result.epsilonTransitions.insert(nfa2.epsilonTransitions.begin(), nfa2.epsilonTransitions.end());

    return result;
}

bool isOperator(char c) {
    return c == '*' || c == '+' || c == '|';
}

bool isOpeningBracket(char c) {
    return c == '(' || c == '[';
}

bool isClosingBracket(char c) {
    return c == ')' || c == ']';
}

bool isConcatenationPossible(char c1, char c2) {
    return
            !(c1 == '|' ||
              c1 == '\\' ||
              isOperator(c2) ||
              (isClosingBracket(c1) && isClosingBracket(c2)) ||
              (isOpeningBracket(c1) && isOpeningBracket(c2)) ||
              (isOperator(c1) && isClosingBracket(c2)) ||
              (isOpeningBracket(c1) && isOperator(c2)) ||
              (isClosingBracket(c1) && isOperator(c2)) ||
              (isOpeningBracket(c1) && c2 == '\\')
            );
}


string applyImplicitConcatenation(const string &regex) {
    string processedRegex = "";
    size_t n = regex.size();

    for (size_t i = 0; i < n; ++i) {
        processedRegex += regex[i];

        if (i + 1 < n) {
            char currentChar = regex[i];
            char nextChar;
            if (currentChar == '[') {
                while (currentChar != ']') {
                    i++;
                    currentChar = regex[i];
                    processedRegex += currentChar;
                }
            }
            if (i + 1 < n) {
                nextChar = regex[i + 1];
            } else continue;


            if (isConcatenationPossible(currentChar, nextChar)) {
                processedRegex += '&';  // Insert implicit concatenation
            }
        }
    }
    return processedRegex;
}

NFA SingleNFA::regexToNFA(const string &regex, const t_clazz &className,
                          const vector<pair<string, string>> &regularDefinitions) {
    string modifiedRegex = regex;

    // Step 1: First pass - replace regular definitions with their patterns using word boundaries
    bool changesMade = true;
    while (changesMade) {
        changesMade = false;

        // We need to process definitions in a specific order to avoid partial replacements.
        for (const auto &def: regularDefinitions) {
            // Use word boundaries to ensure we're replacing complete words only
            std::regex wordRegex("\\b" + def.first + "\\b");  // Match whole word only (e.g., "digit" not "digits")
            string replacedRegex = regex_replace(modifiedRegex, wordRegex, "(" + def.second + ")");

            if (replacedRegex != modifiedRegex) {
                modifiedRegex = replacedRegex;
                changesMade = true;
            }
        }
    }

    // Step 2: Handle range notation and ensure ranges are wrapped in []
    for (size_t i = 0; i < modifiedRegex.size(); ++i) {
        // Handle explicit ranges like "a-z", "0-9", etc.
        if (i + 2 < modifiedRegex.size() && modifiedRegex[i + 1] == '-') {
            // We have a range, e.g., 'a-z', '0-9'
            if (isalpha(modifiedRegex[i]) && isalpha(modifiedRegex[i + 2]) ||
                isdigit(modifiedRegex[i]) && isdigit(modifiedRegex[i + 2])) {
                modifiedRegex = modifiedRegex.substr(0, i) + '[' + modifiedRegex.substr(i, 3) + ']' +
                                modifiedRegex.substr(i + 3);
                i += 2;  // Skip the range and continue
            }
        }
    }


    modifiedRegex.erase(remove(modifiedRegex.begin(), modifiedRegex.end(), ' '), modifiedRegex.end());


    modifiedRegex = applyImplicitConcatenation(modifiedRegex);

    stack<NFA> nfaStack;
    stack<char> operatorStack;

    // Define the precedence of operators
    auto precedence = [](char op) -> int {
        switch (op) {
            case '*':
                return 3;  // Kleene star has highest precedence
            case '+':
                return 3;  // Positive closure has same precedence as Kleene star
            case '|':
                return 1;  // Union has lower precedence
            case '&':
                return 2;  // Implicit concatenation has precedence between '|' and '*'
            default:
                return 0;
        }
    };

    // Apply operators to the NFA stack
    auto applyOperator = [&nfaStack](char op) {
        if (op == '*') {
            NFA nfa = nfaStack.top();
            nfaStack.pop();
            nfaStack.push(SingleNFA::handleKleeneStar(nfa));
        } else if (op == '+') {
            NFA nfa = nfaStack.top();
            nfaStack.pop();
            nfaStack.push(SingleNFA::handlePositiveClosure(nfa));
        } else if (op == '|') {
            NFA nfa2 = nfaStack.top();
            nfaStack.pop();
            NFA nfa1 = nfaStack.top();
            nfaStack.pop();
            nfaStack.push(SingleNFA::handleUnion(nfa1, nfa2));
        } else if (op == '&') {  // Handle implicit concatenation
            NFA nfa2 = nfaStack.top();
            nfaStack.pop();
            NFA nfa1 = nfaStack.top();
            nfaStack.pop();
            nfaStack.push(SingleNFA::handleConcatenation(nfa1, nfa2)); // Concatenate NFAs
        }
    };

    // Iterate through the modified regex
    for (size_t i = 0; i < modifiedRegex.size(); ++i) {
        char ch = modifiedRegex[i];
        if (ch == ' ') {
            continue;
        }
        // Handle escape characters
        if (ch == '\\' && i + 1 < modifiedRegex.size()) {
            // If we encounter a backslash, treat the next character as a literal
            char nextCh = modifiedRegex[i + 1];
            if (nextCh == 'L') { nfaStack.push(createBasicEpsilonNFA()); }
            else { nfaStack.push(createBasicNFA(nextCh)); }  // Treat the character after the backslash as a literal
            i++;  // Skip the next character
            continue;
        }


        if (ch == '(') {  // Push '(' onto the operator stack
            operatorStack.push(ch);
        } else if (ch == ')') {  // Process until '(' is encountered
            while (!operatorStack.empty() && operatorStack.top() != '(') {
                applyOperator(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.pop();  // Pop '('
        } else if (ch == '*' || ch == '|' || ch == '+' || ch == '&') {  // Handle operators
            while (!operatorStack.empty() && precedence(operatorStack.top()) >= precedence(ch)) {
                applyOperator(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.push(ch);
        } else if (ch == '[') {  // Handle ranges (start of a range)
            size_t endPos = modifiedRegex.find(']', i);
            if (endPos != string::npos) {
                string range = modifiedRegex.substr(i + 1, endPos - i - 1);
                // If it's a range, create the corresponding NFA
                if (range.find('-') != string::npos) {  // It's a range (e.g., a-z, 0-9)
                    char start = range[0];
                    char end = range[2];
                    nfaStack.push(createRangeNFA(start, end));
                }
                i = endPos;  // Skip past the closing ']'
            }
        } else {  // Handle literals
            nfaStack.push(createBasicNFA(ch));
        }
    }

    // Apply remaining operators in the stack
    while (!operatorStack.empty()) {
        applyOperator(operatorStack.top());
        operatorStack.pop();
    }

    NFA resultNFA = nfaStack.top();
    resultNFA.acceptStateClasses[resultNFA.acceptState] = className;  // Set accept t_state class
    return resultNFA;
}


// Generate NFA for a single regex with class name
NFA SingleNFA::generateSingleRegexNFA(const string &regex, const t_clazz &className,
                                      const vector<pair<string, string>> &regularDefinitions) {
    return regexToNFA(regex, className, regularDefinitions); // Pass class to regexToNFA
}

NFA SingleNFA::generateSingleKeywordNFA(const string &keyword) {
    vector<pair<string, string>> regularDefinitions = {};
    return regexToNFA(keyword, keyword, regularDefinitions); // Pass class to regexToNFA
}

NFA SingleNFA::generateSinglePunctuationNFA(const string &punctuation) {


    if (punctuation[0] == '\\') {
        NFA nfa = createBasicNFA(punctuation[1]);
        nfa.acceptStateClasses[nfa.acceptState] = punctuation[1];
        return nfa;
    }

    NFA nfa = createBasicNFA(punctuation[0]);
    nfa.acceptStateClasses[nfa.acceptState] = punctuation;
    return nfa;

}

CombinedNFA CombinedNFA::generateCombinedNFA(RegexScanner &regexScanner) {
    vector<NFA> nfas;

    // add keyword NFAs
    for (const auto &keyword: regexScanner.getReservedKeywords()) {
        nfas.push_back(SingleNFA::generateSingleKeywordNFA(keyword));
    }
    for (const auto &punctuation: regexScanner.getReservedPunctuations()) {
        nfas.push_back(SingleNFA::generateSinglePunctuationNFA(punctuation));
    }
    for (const auto &[className, regex]: regexScanner.getRegularExpressions()) {
        nfas.push_back(SingleNFA::generateSingleRegexNFA(regex, className, regexScanner.getRegularDefinitions()));
    }

    CombinedNFA combinedNFA;
    combinedNFA = combinedNFA.combineNFA(nfas);
    return combinedNFA;
}

void NFA::printGraphviz() {
    cout << "digraph finite_state_machine {" << endl;
    cout << "    rankdir=LR;" << endl; // Left-to-right direction
    cout << "    size=\"8,5\";" << endl;

    // Highlight start t_state with an incoming invisible arrow
    cout << "    node [shape = point]; START;" << endl;
    cout << "    START -> " << startState << " [ label = \"start\" ];" << endl;

    // Mark accept t_state with a double circle
    cout << "    node [shape = doublecircle]; " << acceptState << ";" << endl;

    // Mark all other states with a single circle
    cout << "    node [shape = circle];" << endl;

    // Add regular transitions
    for (const auto &[from, transMap]: transitions) {
        for (const auto &[symbol, toStates]: transMap) {
            for (int to: toStates) {
                cout << "    " << from << " -> " << to
                     << " [ label = \"" << symbol << "\" ];" << endl;
            }
        }
    }

    // Add epsilon transitions
    for (const auto &[from, toStates]: epsilonTransitions) {
        for (int to: toStates) {
            cout << "    " << from << " -> " << to
                 << " [ label = \"ε\" ];" << endl;
        }
    }

    cout << "}" << endl;
}


void CombinedNFA::printGraphvizSummarized() {
    cout << "digraph finite_state_machine {" << endl;
    cout << "    rankdir=LR;" << endl; // Left-to-right direction
    cout << "    size=\"8,5\";" << endl;

    // Highlight start t_state with an incoming invisible arrow
    cout << "    node [shape = point]; START;" << endl;
    cout << "    START -> " << startState << " [ label = \"start\" ];" << endl;

    // Mark accept states with their associated classes
    cout << "    node [shape = doublecircle];" << endl;
    for (const auto &[state, stateClass]: acceptStateClasses) {
        cout << "    " << state << " [ label = \"" << state << "\\nClass: " << stateClass << "\" ];" << endl;
    }

    // Mark all other states with a single circle
    cout << "    node [shape = circle];" << endl;

    // Track transitions that have been summarized
    set<pair<int, int>> summarizedTransitions;

    // Add regular transitions
    for (const auto &[from, transMap]: transitions) {
        for (const auto &[symbol, toStates]: transMap) {
            for (int to: toStates) {
                // Track all symbols for transitions between `from` and `to`
                set<char> stateSymbols;
                for (const auto &[fromState, transMap2]: transitions) {
                    for (const auto &[symbol2, toStates2]: transMap2) {
                        for (int toState: toStates2) {
                            if (fromState == from && toState == to) {
                                stateSymbols.insert(symbol2);
                            }
                        }
                    }
                }

                // If there's only one transition for this pair of states and it's "E" or any single letter or digit, don't summarize
                if (stateSymbols.size() == 1) {
                    char symbol = *stateSymbols.begin();  // Get the single t_symbol
                    cout << "    " << from << " -> " << to
                         << " [ label = \"" << symbol << "\" ];" << endl;
                } else {
                    // Summarize the range for digits or alphabetic symbols
                    if (isdigit(symbol)) { // If the t_symbol is a digit (0-9)
                        string summarizedLabel = "0-9";
                        if (summarizedTransitions.find({from, to}) == summarizedTransitions.end()) {
                            cout << "    " << from << " -> " << to
                                 << " [ label = \"" << summarizedLabel << "\" ];" << endl;
                            summarizedTransitions.insert({from, to});
                        }
                    } else if (islower(symbol)) { // If the t_symbol is a lowercase letter (a-z)
                        string summarizedLabel = "a-z";
                        if (summarizedTransitions.find({from, to}) == summarizedTransitions.end()) {
                            cout << "    " << from << " -> " << to
                                 << " [ label = \"" << summarizedLabel << "\" ];" << endl;
                            summarizedTransitions.insert({from, to});
                        }
                    } else if (isupper(symbol)) { // If the t_symbol is an uppercase letter (A-Z)
                        string summarizedLabel = "A-Z";
                        if (summarizedTransitions.find({from, to}) == summarizedTransitions.end()) {
                            cout << "    " << from << " -> " << to
                                 << " [ label = \"" << summarizedLabel << "\" ];" << endl;
                            summarizedTransitions.insert({from, to});
                        }
                    } else {
                        // For other symbols (like punctuation), print them normally
                        cout << "    " << from << " -> " << to
                             << " [ label = \"" << symbol << "\" ];" << endl;
                    }
                }
            }
        }
    }

    // Add epsilon transitions
    for (const auto &[from, toStates]: epsilonTransitions) {
        for (int to: toStates) {
            cout << "    " << from << " -> " << to
                 << " [ label = \"ε\" ];" << endl;
        }
    }

    cout << "}" << endl;
}

void CombinedNFA::printGraphviz() {
    cout << "digraph finite_state_machine {" << endl;
    cout << "    rankdir=LR;" << endl; // Left-to-right direction
    cout << "    size=\"8,5\";" << endl;

    // Highlight start t_state with an incoming invisible arrow
    cout << "    node [shape = point]; START;" << endl;
    cout << "    START -> " << startState << " [ label = \"start\" ];" << endl;

    // Mark accept states with their associated classes
    cout << "    node [shape = doublecircle];" << endl;
    for (const auto &[state, stateClass]: acceptStateClasses) {
        cout << "    " << state << " [ label = \"" << state << "\\nClass: " << stateClass << "\" ];" << endl;
    }

    // Mark all other states with a single circle
    cout << "    node [shape = circle];" << endl;

    // Add regular transitions
    for (const auto &[from, transMap]: transitions) {
        for (const auto &[symbol, toStates]: transMap) {
            for (int to: toStates) {
                cout << "    " << from << " -> " << to
                     << " [ label = \"" << symbol << "\" ];" << endl;
            }
        }
    }

    // Add epsilon transitions
    for (const auto &[from, toStates]: epsilonTransitions) {
        for (int to: toStates) {
            cout << "    " << from << " -> " << to
                 << " [ label = \"ε\" ];" << endl;
        }
    }

    cout << "}" << endl;
}