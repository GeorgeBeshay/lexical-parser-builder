#include "ParsingAnalyzer.h"

ParsingAnalyzer::ParsingAnalyzer(string &parsingTableFp): parsingTable(parsingTableFp){
    this->parsingStack.push(END_TOKEN);
    this->parsingStack.push(this->parsingTable.getStartSymbol());
    this->appendTokenToCurrentDerivation(this->parsingTable.getStartSymbol().getToken());
    this->appendTokenToCurrentDerivation(END_TOKEN.getToken());
    this->outputFileContent.emplace_back(this->currentDerivation);
}

bool ParsingAnalyzer::doParseStep(const ParsingToken& nextInputToken) {

    if (this->parsingStack.empty()) {
        this->outputFileContent.emplace_back(ERROR_FAILURE_CASE);
        return MATCHED_PARSING_STEP;
    }

    const ParsingToken stackTopElement = this->parsingStack.top();

    // happy case
    if (stackTopElement == nextInputToken) {        // matching
        t_prodAlt tempProdAlt = {nextInputToken};
        this->outputProds.emplace_back(stackTopElement, tempProdAlt);
        this->parsingStack.pop();
        return MATCHED_PARSING_STEP;
    }

    // bad cases
    if (stackTopElement.getIsTerminal()) {
        this->outputFileContent.emplace_back(ERROR_TERMINAL_CASE(stackTopElement.getToken()));
        this->parsingStack.pop();
        return NON_MATCHED_PARSING_STEP;
    }

    t_prodAlt prodAlt = this->parsingTable.move(stackTopElement, nextInputToken);

    if (prodAlt.empty()) {
        this->outputFileContent.emplace_back(ERROR_NT_EMPTY_ENTRY_CASE(stackTopElement.getToken(), nextInputToken.getToken()));
        return MATCHED_PARSING_STEP;        // skip the current input token (don't match it with anything).
    }

    if (prodAlt.size() == 1 && (prodAlt[0] == SYNC_TOKEN)) {
        this->outputFileContent.emplace_back(ERROR_NT_SYNC_ENTRY_CASE(stackTopElement.getToken(), nextInputToken.getToken()));
        this->parsingStack.pop();
        return NON_MATCHED_PARSING_STEP;
    }

    // derivation case, prodAlt is not empty for sure.
    this->outputProds.emplace_back(stackTopElement, prodAlt);
    this->replaceFirstTokenInCurrentDerivation(stackTopElement.getToken(), prodAlt);
    this->outputFileContent.emplace_back(this->currentDerivation);
    this->parsingStack.pop();
    for (auto i = prodAlt.rbegin(); i != prodAlt.rend() ; i++) {
        if (*i == EPSILON_TOKEN) continue;  // don't push the epsilon token to the stack.
        this->parsingStack.push(*i);
    }
    return NON_MATCHED_PARSING_STEP;

}

void ParsingAnalyzer::appendTokenToCurrentDerivation(const string& newToken) {
    if (!this->currentDerivation.empty()) this->currentDerivation += " ";
    this->currentDerivation += newToken;
}

bool ParsingAnalyzer::replaceFirstTokenInCurrentDerivation(const string &oldToken, t_prodAlt prodAlt) {
    string concatenatedProdAlt;
    for (const auto& token: prodAlt) {
        if (token == EPSILON_TOKEN) continue;
        concatenatedProdAlt += token.getToken() + " ";
    }
    if (!concatenatedProdAlt.empty()) concatenatedProdAlt.pop_back();       // remove the extra white space.

    size_t startingIdx = this->currentDerivation.find(oldToken);
    if (startingIdx == string::npos) {
        return false;
    }

    this->currentDerivation.replace(
        startingIdx,
        oldToken.size() + (concatenatedProdAlt.empty() ? 1 : 0),
        concatenatedProdAlt
    );

    return true;
}

bool ParsingAnalyzer::exportDerivation(const string& destinationFp) {

    ofstream outFile(destinationFp);  // Open the file for writing
    if (!outFile) {
        cerr << "Error: Unable to open file for writing parsing analyzer output: " << endl;
        return false;
    }

    for (const auto& line : this->outputFileContent) {
        outFile << line << endl;
    }

    outFile.close();
    if (outFile.fail()) {  // Check if there was an error during write
        cerr << "Error: Unable to close file after writing parsing analyzer output: " << endl;
        return false;
    }

    return true;
}
