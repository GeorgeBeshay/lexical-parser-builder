#include "Grammar.h"
#include <fstream>
#include <utility>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

struct RuleAsStringStruct {
    string LHS;
    vector<vector<string>> productions;
};

Grammar::Grammar(string grammarInputFilePath) {
    generateGrammarFromFile(grammarInputFilePath);
}

void Grammar::generateGrammarFromFile(string &grammarInputFilePath) {
    ifstream grammarText(grammarInputFilePath.c_str());
    if (!grammarText.good()) {
        cout << grammarInputFilePath << ": FILE DOES NOT EXIST!!!.\n";
        exit(EXIT_FAILURE);
    }
    string currentRule, previousRule;
    bool firstRule = true, isStart = true;

    while (getline(grammarText, currentRule)) {
        if (firstRule) {
            previousRule += currentRule;
            firstRule = false;
        } else if (currentRule[0] == '#') {  // Check for the start of a new rule
            generateRule(previousRule, isStart);  // Parse the previous rule
            if (isStart) isStart = false;
            previousRule.clear();
            previousRule += currentRule;  // Start a new rule
        } else   // If it's not a new rule, continue appending to the previous line
            previousRule += " " + currentRule;

    }
    generateRule(previousRule, isStart);  // Parse the last rule
    grammarText.close();
}

// Splits a string `str` into a vector `vec` by using `delimiter` as the separator
void stringToVec(vector<string> &vec, string &str, char delimiter) {
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

// Removes single quotes from a string if they surround it
bool trimTerminalQuotes(string &str) {
    if (str[0] == '\'' && str[str.length() - 1] == '\'') {  // Check if the first and last characters are single quotes
        str.erase(remove(str.begin(), str.end(), '\''), str.end());  // Remove the single quotes from the string
        return true;  // Return true, indicating that the quotes were removed
    }
    return false;  // Return false if there are no surrounding single quotes
}

// Trims leading and trailing spaces from the given string
void trimSpaces(string &str) {
    // Trim leading spaces.
    str.erase(str.begin(), find_if(str.begin(), str.end(), [](int character) {
        return !isspace(character);  // Find the first non-whitespace character
    }));
    // Trim trailing spaces.
    str.erase(find_if(str.rbegin(), str.rend(), [](int character) {
        return !isspace(character);  // Find the first non-whitespace character from the end
    }).base(), str.end());
}

// This function tokenizes a rule string into a 'RuleAsStringStruct' structure
RuleAsStringStruct extractRuleComponents(string &ruleAsString) {
    RuleAsStringStruct ruleAsStringStruct;
    vector<string> productionsString;

    // Remove the first character ('#')
    ruleAsString.erase(0, 1);

    // Find the position of the first equal sign, splitting the rule into LHS and RHS
    size_t positionOfEqualSign = ruleAsString.find_first_of('=');
    if (positionOfEqualSign == string::npos) {
        cout << "Invalid rule: " << ruleAsString << "\n";
        exit(EXIT_FAILURE);
    }
    // Extract the LHS symbol (part before '=')
    string lhs = ruleAsString.substr(0, positionOfEqualSign - 1);
    trimSpaces(lhs);
    // Store the LHS symbol token
    ruleAsStringStruct.LHS = lhs;
    // Extract the RHS production rules (part after '=')
    string rhs = ruleAsString.substr(positionOfEqualSign + 1, ruleAsString.length());
    trimSpaces(rhs);
    // Split the RHS into individual production rules separated by '|'
    stringToVec(productionsString, rhs, '|');
    for (auto &productionString: productionsString) {
        vector<string> prod;
        trimSpaces(productionString);
        stringToVec(prod, productionString, ' ');  // Split production into symbols separated by spaces
        ruleAsStringStruct.productions.push_back(prod);  // Add production to the rule struct
        prod.clear();
    }
    return ruleAsStringStruct;
}

void Grammar::generateRule(string &ruleString, bool isStart) {

    RuleAsStringStruct ruleAsStringStruct = extractRuleComponents(
            ruleString);  // Tokenizes the rule into its LHS and RHS
    t_prodRule ruleProductions;  // To hold all productions for this rule
    ParsingToken ruleLHS = ParsingToken(ruleAsStringStruct.LHS, false);  // LHS of the rule
    for (auto &production: ruleAsStringStruct.productions) { // Iterate through the RHS productions
        t_prodAlt prod;  // Creates a new production for the rule
        for (auto &token: production) {  // Iterate through the  tokens of each production
            ParsingToken symbol;
            trimSpaces(token);
            if (token.empty()) continue;
            if (token == "\\L" || trimTerminalQuotes(token))
                symbol = ParsingToken(token, true);  // Create a terminal symbol
            else
                symbol = ParsingToken(token, false);  // Create non-terminal symbols
            prod.push_back(symbol);  // Add the token to the production
        }
        ruleProductions.insert(prod);  // Add the production to the list
    }

    if (isStart) startSymbol = ruleLHS;
    // Check if the grammar already has this LHS symbol, if so, add productions to it.
    if (grammar.find(ruleLHS) != grammar.end())
        grammar[ruleLHS].addProductions(ruleProductions);
        // If not, create a new entry for it and add the rule.
    else
        addRule(ruleLHS, ruleProductions);
}

void Grammar::addRule(ParsingToken &lhs, t_prodRule &productions) {
    GrammarRule rule = GrammarRule(lhs, productions);
    grammar[lhs] = rule;
}

int findLongestCommonPrefixLength(vector<t_prodAlt> prods) {
    const t_prodAlt &prod = prods.front();  // Reference the first production to compare others with

    for (int i = 0; i < prod.size(); i++)
        for (int j = 1; j < prods.size(); j++)
            if (prods[j].size() < i || prods[j][i] != prod[i])
                return i;

    return prod.size();  // All symbols match, return the full length of the prefix
}

void Grammar::applyLeftFactoring() {
    unordered_map<ParsingToken, GrammarRule, ParsingTokenHash, Comparator> leftFactoredGrammar;

    for (auto grammar_entry: grammar) {
        // A map to group productions by their common prefix
        map<string, vector<t_prodAlt>> common_factors;
        // Group productions by the first symbol (i.e., prefix of the production)
        for (const auto &prod: grammar_entry.second.getProductions())
            common_factors[prod[0].getToken()].push_back(prod);

        grammar_entry.second.clearProductions();
        // New productions for the current LHS symbol after factoring
        t_prodRule newFactoredProductions;

        // Process each group of common factors
        for (auto entry: common_factors) {
            if (entry.second.size() == 1)
                // If only one production with this common prefix, add as is
                newFactoredProductions.insert(entry.second.front());
            else {
                int longestCommonPrefixLength = findLongestCommonPrefixLength(entry.second);
                t_prodAlt newProd;
                newProd.reserve(longestCommonPrefixLength); // Reserve space for prefix symbols

                for (int i = 0; i < longestCommonPrefixLength; i++)
                    newProd.push_back(entry.second.front()[i]); // Add the common prefix symbols to the new production

                string newUniqueTokenSymbol = grammar_entry.second.getLhs().getToken() + "'";
                ParsingToken token(newUniqueTokenSymbol, grammar_entry.second.getLhs().getIsTerminal());
                // Ensure the generated token doesn't already exist
                while (leftFactoredGrammar.find(token) != leftFactoredGrammar.end() ||
                       grammar.find(token) != grammar.end()) {
                    newUniqueTokenSymbol += "'";
                    token = ParsingToken(newUniqueTokenSymbol, grammar_entry.second.getLhs().getIsTerminal());
                }

                ParsingToken newUniqueToken(newUniqueTokenSymbol, grammar_entry.second.getLhs().getIsTerminal());

                newProd.push_back(newUniqueToken); // Add the new unique symbol to the new production

                // Create a new production with the original non-terminal and the factored prefix
                ParsingToken originalToken(grammar_entry.second.getLhs());
                newFactoredProductions.insert(newProd);

                // Create new rule productions for the remaining tokens after the common prefix
                t_prodRule newRuleProductions;
                for (const auto &prod: entry.second) {
                    t_prodAlt productionRemainingTokens;

                    // Collect remaining tokens in each production after the common prefix
                    for (int i = longestCommonPrefixLength; i < prod.size(); i++)
                        productionRemainingTokens.push_back(prod[i]);

                    // If the remainder is empty, add an epsilon (empty string) terminal
                    if (productionRemainingTokens.empty()) {
                        ParsingToken eps("\\L", true);
                        productionRemainingTokens.push_back(eps);
                    }
                    newRuleProductions.insert(productionRemainingTokens);
                }
                // Create a new rule with the factored new Unique Token  and its new productions
                GrammarRule addedRule(newUniqueToken, newRuleProductions);

                // Create a temporary CFG to handle recursive left factoring and merge with the new grammar
                Grammar recursiveLeftFactoringGrammar;
                unordered_map<ParsingToken, GrammarRule, ParsingTokenHash, Comparator> tempGrammar;
                tempGrammar[newUniqueToken] = addedRule;
                recursiveLeftFactoringGrammar.setGrammar(tempGrammar);
                recursiveLeftFactoringGrammar.applyLeftFactoring();

                // Merge the result of the recursive left factorization into the new grammar
                for (const auto &recGrammarEntry: recursiveLeftFactoringGrammar.getGrammar())
                    leftFactoredGrammar[recGrammarEntry.first] = recGrammarEntry.second;
            }
        }
        // Add the rule for the original LHS symbol after factoring
        ParsingToken token(grammar_entry.first);
        GrammarRule newRule(token, newFactoredProductions);
        leftFactoredGrammar[token] = newRule;
    }

    // Replace the original grammar with the newly factored grammar
    grammar = leftFactoredGrammar;
}

void handleNonImmediateLeftRecursion(GrammarRule &ruleA, GrammarRule &ruleB) {
    ParsingToken ruleA_LHS = ruleA.getLhs();
    ParsingToken ruleB_LHS = ruleB.getLhs();

    t_prodRule ruleA_productions, ruleB_productions, ruleA_newProductions;
    ruleA_productions = ruleA.getProductions();
    ruleB_productions = ruleB.getProductions();

    for (const auto &prodA: ruleA_productions) {
        if (prodA[0] == ruleB_LHS) {
            for (const auto &prodB: ruleB_productions) {
                t_prodAlt newProdA;
                for (const auto &token: prodB)
                    newProdA.push_back(token);

                for (int i = 1; i < prodA.size(); i++)
                    newProdA.push_back(prodA[i]);

                ruleA_newProductions.insert(newProdA);
            }
        } else
            ruleA_newProductions.insert(prodA);
    }
    ruleA.setProductions(ruleA_newProductions);
}

void handleImmediateLeftRecursion(GrammarRule &rule, vector<GrammarRule> &grammarRules) {
    ParsingToken lhs = rule.getLhs();
    // Create a new intermediate token to replace the recursive LHS (e.g., A -> A')
    ParsingToken newIntermediateToken(lhs.getToken() + "'", lhs.getIsTerminal());

    // Separate productions into those with left recursion (recursiveProductions) and those without (nonRecursiveProductions)
    t_prodRule recursiveProductions, nonRecursiveProductions, originalProductions, modifiedProductions, newRuleProductions;

    // Fetch the original productions from the rule
    originalProductions = rule.getProductions();

    // Check for left recursion in the original productions
    for (auto prod: originalProductions) {
        if (prod[0] == lhs) {
            // If the production starts with LHS (indicating left recursion), add to recursiveProductions
            t_prodAlt newProd;
            for (int i = 1; i < prod.size(); i++)  // Exclude the LHS symbol
                newProd.push_back(prod[i]);

            recursiveProductions.insert(newProd);
        } else
            // Productions without left recursion are added to nonRecursiveProductions
            nonRecursiveProductions.insert(prod);
    }

    // If no left-recursive productions are found, there is nothing to handle
    if (recursiveProductions.empty())
        return;

    // Handle the case where there are no non-left-recursive alternatives (nonRecursiveProductions are empty)
    if (nonRecursiveProductions.empty()) {
        t_prodAlt newProds;
        // Add the new intermediate token to start a new derivation chain
        newProds.push_back(newIntermediateToken);
        modifiedProductions.insert(newProds);
    }

    // Modify the original rule by appending the intermediate token to all nonRecursiveProductions
    for (auto prod: nonRecursiveProductions) {
        prod.push_back(newIntermediateToken);  // Append intermediate token to the prod production
        modifiedProductions.insert(prod);     // Insert modified prod into the new productions
    }

    // For each prod (recursive part), append the intermediate token to allow further derivation
    for (auto prod: recursiveProductions) {
        prod.push_back(newIntermediateToken);
        newRuleProductions.insert(prod);  // Add to new rule productions
    }

    // Update the original rule with the modified productions
    rule.setProductions(modifiedProductions);

    // Add epsilon (\\L) as a possible production for the new intermediate token
    ParsingToken eps("\\L", true);  // Representing the empty string (epsilon)
    t_prodAlt newProd;
    newProd.push_back(eps);
    newRuleProductions.insert(newProd);  // Insert epsilon production

    // Create and add the new rule with intermediate productions to the grammar
    GrammarRule newRule(newIntermediateToken, newRuleProductions);
    grammarRules.push_back(newRule);  // Append the new rule to the grammar
}

void Grammar::removeLeftRecursion() {
    unordered_map<ParsingToken, GrammarRule, ParsingTokenHash, Comparator> leftRecursionFreeGrammar;
    vector<GrammarRule> rules = getRules();

    for (int i = 0; i < rules.size(); i++) {
        for (int j = 0; j < i; j++)
            handleNonImmediateLeftRecursion(rules[i], rules[j]);

        handleImmediateLeftRecursion(rules[i], rules);
    }
    for (const auto &rule: rules)
        leftRecursionFreeGrammar[rule.getLhs()] = rule;

    Grammar::grammar = leftRecursionFreeGrammar;
}

ParsingToken Grammar::getStartSymbol() {
    return startSymbol;
}

vector<GrammarRule> Grammar::getRules() {
    vector<GrammarRule> rules;
    rules.reserve(grammar.size());
    for (const auto &entry: grammar)
        rules.push_back(entry.second);

    return rules;
}

const unordered_map<ParsingToken, GrammarRule, ParsingTokenHash, Comparator> &Grammar::getGrammar() {
    return grammar;
}

void Grammar::setGrammar(unordered_map<ParsingToken, GrammarRule, ParsingTokenHash, Comparator> &newGrammar) {
    Grammar::grammar = newGrammar;
}

t_grammar Grammar::get_t_grammar() const {
    t_grammar t_grammar_map;
    for (const auto &entry: grammar)
        t_grammar_map[entry.first] = entry.second.getProductions();

    return t_grammar_map;
}

void Grammar::printGrammar() {
    for (auto g: grammar)
        std::cout << g.second.toString() << std::endl;
}




