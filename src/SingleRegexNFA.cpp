#include "SingleRegexNFA.h"
#include <stack>

// Initialize state counter
int SingleRegexNFA::stateCounter = 0;

// NFA constructor
NFA::NFA(int start, int accept) : startState(start), acceptState(accept) {}

// Add regular transition
void NFA::addTransition(int from, char symbol, int to) {
    transitions[from][symbol].insert(to);
}

// Add epsilon transition
void NFA::addEpsilonTransition(int from, int to) {
    epsilonTransitions[from].insert(to);
}

// Print NFA
void NFA::printNFA() {
    cout << "Start State: " << startState << endl;
    cout << "Accept State: " << acceptState << endl;

    // Print the corresponding class of the accept state
    if (acceptStateClasses.find(acceptState) != acceptStateClasses.end()) {
        cout << "Accept State Class: " << acceptStateClasses[acceptState] << endl;
    } else {
        cout << "Accept State Class: Not Assigned" << endl;
    }

    cout << "Transitions:" << endl;
    for (const auto& [from, transMap] : transitions) {
        for (const auto& [symbol, toStates] : transMap) {
            for (int to : toStates) {
                cout << "  " << from << " --" << symbol << "--> " << to << endl;
            }
        }
    }

    cout << "Epsilon Transitions:" << endl;
    for (const auto& [from, toStates] : epsilonTransitions) {
        for (int to : toStates) {
            cout << "  " << from << " --ε--> " << to << endl;
        }
    }
}

// Helper to create a basic NFA for a single character
static NFA createBasicNFA(char symbol) {
    int start = SingleRegexNFA::stateCounter++;
    int accept = SingleRegexNFA::stateCounter++;
    NFA nfa(start, accept);
    nfa.addTransition(start, symbol, accept);
    return nfa;
}

// Handle Kleene star (a*)
NFA SingleRegexNFA::handleKleeneStar(const NFA& nfa) {
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
NFA SingleRegexNFA ::handlePositiveClosure(const NFA &nfa) {
    int newStart = stateCounter++;
    int newAccept = stateCounter++;

    NFA result(newStart, newAccept);

    // Add epsilon transitions for Kleene star logic
    result.addEpsilonTransition(newStart, nfa.startState);
    result.addEpsilonTransition(nfa.acceptState, nfa.startState);
    result.addEpsilonTransition(nfa.acceptState, newAccept);

//    // Copy transitions
    result.transitions.insert(nfa.transitions.begin(), nfa.transitions.end());
    result.epsilonTransitions.insert(nfa.epsilonTransitions.begin(), nfa.epsilonTransitions.end());

    return result;
}
// Handle concatenation (a.b)
NFA SingleRegexNFA::handleConcatenation(const NFA& nfa1, const NFA& nfa2) {
    NFA result(nfa1.startState, nfa2.acceptState);

    result.transitions.insert(nfa1.transitions.begin(), nfa1.transitions.end());
    result.epsilonTransitions.insert(nfa1.epsilonTransitions.begin(), nfa1.epsilonTransitions.end());
    result.transitions.insert(nfa2.transitions.begin(), nfa2.transitions.end());
    result.epsilonTransitions.insert(nfa2.epsilonTransitions.begin(), nfa2.epsilonTransitions.end());

    // Link nfa1's accept state to nfa2's start state
    result.addEpsilonTransition(nfa1.acceptState, nfa2.startState);

    return result;
}

// Handle union (a|b)
NFA SingleRegexNFA::handleUnion(const NFA& nfa1, const NFA& nfa2) {
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

string preprocessRegex(const string& regex) {
    string processedRegex = "";
    size_t n = regex.size();

    for (size_t i = 0; i < n; ++i) {
        processedRegex += regex[i];

        if (i + 1 < n) {
            char currentChar = regex[i];
            char nextChar = regex[i + 1];
            cout << "Current Char: " << currentChar << " Next Char: " << nextChar << endl;

            // Insert implicit concatenation '.' in the following cases:
            if ((isalpha(currentChar) && (isalpha(nextChar) || nextChar == '(')) || // Literal followed by literal/group
                (currentChar == ')' && (isalpha(nextChar) || nextChar == '(')) ||   // Group followed by literal/group
                (currentChar == '*' && (isalpha(nextChar) || nextChar == '(')) ||  // Kleene star followed by literal/group
                (currentChar == '+' && (isalpha(nextChar) || nextChar == '('))) {  // One or more repetitions followed by literal/group
                cout << "Inserting implicit concatenation" << endl;
                processedRegex += '&';  // Insert implicit concatenation
            }
        }
    }
    cout << "Processed Regex: " << processedRegex << endl;
    return processedRegex;
}


// Here you can use your existing NFA generation method that processes the regex
NFA SingleRegexNFA::regexToNFA(const string& regex, const Class& className) {
    string modifiedRegex = preprocessRegex(regex);  // Preprocess the regex to insert '.'
    cout << "Modified Regex: " << modifiedRegex << endl;
    stack<NFA> nfaStack;
    stack<char> operatorStack;

    auto precedence = [](char op) -> int {
        switch (op) {
            case '*': return 3;  // Kleene star has highest precedence
            case '+': return 3;  // Positive closure has same precedence as Kleene star
            case '|': return 1;  // Union has lower precedence
            case '&': return 2;  // Implicit concatenation has precedence between '|' and '*'
            default: return 0;
        }
    };

    auto applyOperator = [&nfaStack](char op) {
        if (op == '*') {
            NFA nfa = nfaStack.top();
            nfaStack.pop();
            nfaStack.push(SingleRegexNFA::handleKleeneStar(nfa));
        }
        else if (op == '+') {
            NFA nfa = nfaStack.top();
            nfaStack.pop();
            nfaStack.push(SingleRegexNFA::handlePositiveClosure(nfa));
        }
        else if (op == '|') {
            NFA nfa2 = nfaStack.top();
            nfaStack.pop();
            NFA nfa1 = nfaStack.top();
            nfaStack.pop();
            nfaStack.push(SingleRegexNFA::handleUnion(nfa1, nfa2));
        }
        else if (op == '&') {  // Handle implicit concatenation
            NFA nfa2 = nfaStack.top();
            nfaStack.pop();
            NFA nfa1 = nfaStack.top();
            nfaStack.pop();
            nfaStack.push(SingleRegexNFA::handleConcatenation(nfa1, nfa2)); // Concatenate NFAs
        }
    };

    for (size_t i = 0; i < modifiedRegex.size(); ++i) {
        char ch = modifiedRegex[i];

        if (isalpha(ch)) {  // Process literal characters
            nfaStack.push(createBasicNFA(ch));
        }
        else if (ch == '(') {  // Push '(' onto the operator stack
            operatorStack.push(ch);
        }
        else if (ch == ')') {  // Process until '(' is encountered
            while (!operatorStack.empty() && operatorStack.top() != '(') {
                applyOperator(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.pop();  // Pop '('
        }
        else if (ch == '*' || ch == '|' || ch == '+' || ch == '&') {  // Handle operators
            while (!operatorStack.empty() && precedence(operatorStack.top()) >= precedence(ch)) {
                applyOperator(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.push(ch);
        }
    }

    // Apply remaining operators in the stack
    while (!operatorStack.empty()) {
        applyOperator(operatorStack.top());
        operatorStack.pop();
    }

    NFA resultNFA = nfaStack.top();
    resultNFA.acceptStateClasses[resultNFA.acceptState] = className;  // Set accept state class
    return resultNFA;
}

// Generate NFA for a single regex with class name
NFA SingleRegexNFA::generateSingleRegexNFA(const string& regex, const Class& className) {
    return regexToNFA(regex, className); // Pass class to regexToNFA
}

void NFA::printGraphviz() {
    cout << "digraph finite_state_machine {" << endl;
    cout << "    rankdir=LR;" << endl; // Left-to-right direction
    cout << "    size=\"8,5\";" << endl;

    // Highlight start state with an incoming invisible arrow
    cout << "    node [shape = point]; START;" << endl;
    cout << "    START -> " << startState << " [ label = \"start\" ];" << endl;

    // Mark accept state with a double circle
    cout << "    node [shape = doublecircle]; " << acceptState << ";" << endl;

    // Mark all other states with a single circle
    cout << "    node [shape = circle];" << endl;

    // Add regular transitions
    for (const auto& [from, transMap] : transitions) {
        for (const auto& [symbol, toStates] : transMap) {
            for (int to : toStates) {
                cout << "    " << from << " -> " << to
                     << " [ label = \"" << symbol << "\" ];" << endl;
            }
        }
    }

    // Add epsilon transitions
    for (const auto& [from, toStates] : epsilonTransitions) {
        for (int to : toStates) {
            cout << "    " << from << " -> " << to
                 << " [ label = \"ε\" ];" << endl;
        }
    }

    cout << "}" << endl;
}
