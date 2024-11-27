#include "NFAGenerator.h"

// Creates a simple NFA for a single character.
NFAGenerator::NFA NFAGenerator::createSimpleNFA(char ch) {
    NFA nfa;
    nfa.startState = 0;  // Single start state
    nfa.acceptState = 1; // Single accept state

    // Add a transition from start to accept on the given character
    nfa.transitions[0][ch].insert(1);

    // Include the character in the NFA's symbol set
    nfa.symbols.insert(ch);

    return nfa;
}

// Handles union operation: NFA for (nfa1 | nfa2)
NFAGenerator::NFA NFAGenerator::handleUnion(NFA& nfa1, NFA& nfa2) {
    NFA combinedNFA;

    // Create new start and accept states for the combined NFA
    combinedNFA.startState = 0;
    combinedNFA.acceptState = nfa1.acceptState + nfa2.acceptState + 2;

    int offset1 = 1; // Offset for the first NFA
    int offset2 = offset1 + nfa1.acceptState + 1; // Offset for the second NFA

    // Copy transitions from both NFAs into the combined NFA
    copyTransitions(nfa1, combinedNFA, offset1);
    copyTransitions(nfa2, combinedNFA, offset2);

    // Add epsilon transitions from the new start state to both NFAs
    combinedNFA.epsilonTransitions[0].insert(nfa1.startState + offset1);
    combinedNFA.epsilonTransitions[0].insert(nfa2.startState + offset2);

    // Add epsilon transitions from both NFAs' accept states to the new accept state
    combinedNFA.epsilonTransitions[nfa1.acceptState + offset1].insert(combinedNFA.acceptState);
    combinedNFA.epsilonTransitions[nfa2.acceptState + offset2].insert(combinedNFA.acceptState);

    // Merge the symbol sets
    combinedNFA.symbols.insert(nfa1.symbols.begin(), nfa1.symbols.end());
    combinedNFA.symbols.insert(nfa2.symbols.begin(), nfa2.symbols.end());

    return combinedNFA;
}

// Handles concatenation operation: NFA for (nfa1 . nfa2)
NFAGenerator::NFA NFAGenerator::handleConcatenation(NFA& nfa1, NFA& nfa2) {
    NFA concatenatedNFA;

    // Start state is the same as nfa1's start state
    concatenatedNFA.startState = nfa1.startState;

    // Accept state is nfa2's accept state, offset by nfa1's size
    concatenatedNFA.acceptState = nfa1.acceptState + nfa2.acceptState;

    int offset2 = nfa1.acceptState; // Offset for the second NFA

    // Copy transitions for both NFAs into the concatenated NFA
    copyTransitions(nfa1, concatenatedNFA, 0);
    copyTransitions(nfa2, concatenatedNFA, offset2);

    // Add an epsilon transition from nfa1's accept state to nfa2's start state
    concatenatedNFA.epsilonTransitions[nfa1.acceptState].insert(nfa2.startState + offset2);

    // Merge the symbol sets
    concatenatedNFA.symbols.insert(nfa1.symbols.begin(), nfa1.symbols.end());
    concatenatedNFA.symbols.insert(nfa2.symbols.begin(), nfa2.symbols.end());

    return concatenatedNFA;
}

// Handles Kleene star operation: NFA for (nfa*)
NFAGenerator::NFA NFAGenerator::handleKleeneStar(NFA& baseNFA) {
    NFA starNFA;

    // Create new start and accept states
    starNFA.startState = 0;
    starNFA.acceptState = baseNFA.acceptState + 2;

    int offset = 1; // Offset for the base NFA

    // Copy the base NFA's transitions
    copyTransitions(baseNFA, starNFA, offset);

    // Add epsilon transitions for the Kleene star behavior
    starNFA.epsilonTransitions[0].insert(baseNFA.startState + offset);
    starNFA.epsilonTransitions[0].insert(starNFA.acceptState);
    starNFA.epsilonTransitions[baseNFA.acceptState + offset].insert(baseNFA.startState + offset);
    starNFA.epsilonTransitions[baseNFA.acceptState + offset].insert(starNFA.acceptState);

    // Merge the symbol sets
    starNFA.symbols.insert(baseNFA.symbols.begin(), baseNFA.symbols.end());

    return starNFA;
}

// Handles positive closure operation: NFA for (nfa+)
NFAGenerator::NFA NFAGenerator::handlePositiveClosure(NFA& baseNFA) {
    NFA plusNFA;

    // Create new start and accept states
    plusNFA.startState = 0;
    plusNFA.acceptState = baseNFA.acceptState + 2;

    int offset = 1; // Offset for the base NFA

    // Copy the base NFA's transitions
    copyTransitions(baseNFA, plusNFA, offset);

    // Add epsilon transitions for positive closure
    plusNFA.epsilonTransitions[0].insert(baseNFA.startState + offset);
    plusNFA.epsilonTransitions[baseNFA.acceptState + offset].insert(baseNFA.startState + offset);
    plusNFA.epsilonTransitions[baseNFA.acceptState + offset].insert(plusNFA.acceptState);

    // Merge the symbol sets
    plusNFA.symbols.insert(baseNFA.symbols.begin(), baseNFA.symbols.end());

    return plusNFA;
}

// Copies transitions from one NFA to another, applying an offset to the state numbers
void NFAGenerator::copyTransitions(const NFA& fromNFA, NFA& toNFA, int offset) {
    for (const auto& [state, transitions] : fromNFA.transitions) {
        for (const auto& [symbol, destinations] : transitions) {
            for (int dest : destinations) {
                toNFA.transitions[state + offset][symbol].insert(dest + offset);
            }
        }
    }

    for (const auto& [state, destinations] : fromNFA.epsilonTransitions) {
        for (int dest : destinations) {
            toNFA.epsilonTransitions[state + offset].insert(dest + offset);
        }
    }
}

// Checks if a character is a regex operator
bool NFAGenerator::isOperator(char ch) {
    return ch == '|' || ch == '*' || ch == '+' || ch == '(' || ch == ')';
}

// Validates the input regex for edge cases
void NFAGenerator::validateRegex(const std::string& regex) {
    if (regex.empty()) {
        throw std::invalid_argument("Regex cannot be empty.");
    }

    int openParentheses = 0;
    for (size_t i = 0; i < regex.size(); ++i) {
        char ch = regex[i];
        if (!isalnum(ch) && !isOperator(ch)) {
            throw std::invalid_argument("Invalid character in regex: " + std::string(1, ch));
        }

        if (ch == '(') {
            ++openParentheses;
        } else if (ch == ')') {
            --openParentheses;
            if (openParentheses < 0) {
                throw std::invalid_argument("Unmatched closing parenthesis.");
            }
        }

        // Check for repeated or misplaced operators
        if (i > 0 && isOperator(ch) && isOperator(regex[i - 1]) && regex[i - 1] != ')') {
            throw std::invalid_argument("Misplaced or repeated operator: " + std::string(1, ch));
        }
    }

    if (openParentheses != 0) {
        throw std::invalid_argument("Unmatched opening parenthesis.");
    }
}

// Generates an NFA for the given regex
NFAGenerator::NFA NFAGenerator::generateNFA(const std::string& regex) {
    validateRegex(regex);

    // To be implemented: regex parsing and NFA construction logic.
    return NFA();
}
