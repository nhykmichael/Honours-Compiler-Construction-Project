#include "LexGraph.h"
#include <algorithm>  // for std::find
/**
 * @file LexGraph.cpp
 * @brief Implementation of the LexGraph class for lexical analysis in the Nhyk compiler.
 *
 * This source file contains the implementation of the LexGraph class, which represents a
 * lexical graph used for tokenization and parsing in the Nhyk compiler.
 *
 * @author MNS Ahimbisibwe
 * @SN 217005435
 * @date 2023/09/03
 * @model Lexical Analysis Design
 * @version D01
 */


// --- LexGraph Implementation ---

LexGraph::LexGraph() : source(""), position(0), start(nullptr) {}

LexGraph::~LexGraph() {}


/**
 * Overloaded output stream insertion operator for writing LexGraph object to an output stream.
 *
 * This operator formats and writes the token information from
 * a LexGraph object to the specified output stream.
 * It iterates through the tokens, converts their types
 * to strings, and concatenates the information for output.
 *
 * @param osout The output stream where the data will be written.
 * @param graph The LexGraph object containing token information to be written.
 * @return A reference to the output stream after writing the data.
 */

 /*
std::ostream& operator<<(std::ostream& osout, const LexGraph& graph){
    std::string str_tokens;
    for(const auto& token : graph.tokens){
        std::string str_type;
        switch(token.getType()){
        case TokenType::IDENTIFIER:
            str_type = "ID";
            break;
        case TokenType::PUNCTUATION:
            str_type = "PUNCTUATION";
            break;
        case TokenType::KEYWORD:
            str_type = "KEYWORD";
            break;
        case TokenType::OPERATOR:
            str_type = "OPERATOR";
            break;
        case TokenType::INT_LITERAL:
            str_type = "INT_LITERAL";
            break;
        case TokenType::DOUBLE_LITERAL:
            str_type = "DOUBLE_LITERAL";
            break;
        case TokenType::BOOLEAN_LITERAL:
            str_type = "BOOLEAN_LITERAL";
            break;
        case TokenType::LITERAL:
            str_type = "LITERAL";
            break;
        // ... add other token as needed
        default:
            str_type = "UNKNOWN";
        }str_tokens += " " + str_type + "(" + token.getLexeme() + ")";
    }osout << str_tokens; return osout; // Write the formatted info to the output stream
}
*/

void LexGraph::traverse(NhykLexicalNode* trNode) {
    // This method implementation is adapted from Prof DA Coulter's example.
    // Source URL: https://eve.uj.ac.za/lectures.php#lecture-it08x87

    // Display the current node being visited along with the current position and source.
    std::cout << "Visiting--> " << trNode->name << " Position--> "
              << position << " Source--> " << source << std::endl;

    // FSM traversal logic begins here:

    // If the source is empty, we've reached the end of the input.
    if(source.length() == 0){
        std::cout << "Ending Traversal --> End of Input" << std::endl;

        // Classify the node to determine the token type.
        classify(trNode);
        return;
    }

    // Check if there's a transition for the current character in the source.
    if(trNode->transitions.find(source[position]) != trNode->transitions.end()){
        std::cout << "Transition Found For-> " << source[position]
                  << " @ " << trNode->name << std::endl;

        // Move to the next node based on the transition.
        NhykLexicalNode* next = trNode->transitions[source[position]];

        // Increment the position as we move forward in the source.
        position++;

        // Recursively traverse to the next node.
        traverse(next);
    }else{
        // If no transition is found for the current character, classify the current node.
        std::cout << "No Transition For-> " << source[position] << std::endl;
        classify(trNode);
    }
}

std::string LexGraph::getSource() const {return source;}
void LexGraph::setSource(const std::string& newSource){source = newSource;}
std::vector<Token>& LexGraph::getTokens() {return tokens;}
void LexGraph::clearTokens() {tokens.clear();}

// --- LexGraphStringLiteral Implementation ---
LexGraphStringLiteral::LexGraphStringLiteral(): s1(), s2(), s3(){
    start = &s1;

    s1.name = "s1";
    s1.terminal = false;
    s1.transitions['"'] = &s2;

    s2.name = "s2";
    s2.terminal = false;  // not a terminal state since we haven't reached the end quote yet
    s2.transitions['"'] = &s3;  // transition to s3 when we find the end quote
    for (int i = 32; i < 127; ++i) {  // for almost all printable characters
        if (i != '"')     // excluding the quote itself
            s2.transitions[i] = &s2;  // remain in s2
    }s3.name = "s3";
    s3.terminal = true;   // this is a terminal state for the string literal
}

void LexGraphStringLiteral::classify(NhykLexicalNode* node) {
    Token token;
    if (node == &s3) {
        token = Token(TokenType::LITERAL, source.substr(0, position));
    } else {
        token = Token(TokenType::UNKNOWN, source.substr(0, 1));
    }
    tokens.push_back(token);
    source = source.substr(position);
    position = 0;
    clearTokens();
}

// --- LexGraphID Implementation ---

LexGraphID::LexGraphID() : s1(), s2(), s_error() {
    start = &s1;

    s1.name = "s1";
    s1.terminal = false;
    // ... [transitions for identifier recognition]
    for (char chC = 'a'; chC <= 'z'; chC++) s1.transitions[chC] = &s2;
    for (char chC = 'A'; chC <= 'Z'; chC++) s1.transitions[chC] = &s2;

    s2.name = "s2";
    s2.terminal = true;
    // ... [transitions for identifier recognition]
    for (char chC = 'a'; chC <= 'z'; chC++) s2.transitions[chC] = &s2;
    for (char chC = 'A'; chC <= 'Z'; chC++) s2.transitions[chC] = &s2;
    for (char chC = '0'; chC <= '9'; chC++) s2.transitions[chC] = &s2;
    s2.transitions['_'] = &s2;
}

/**
 * @brief Classifies transitions found in the lexical graph node.
 *
 * The `classify` method processes transitions found in the given lexical graph node
 * to classify and tokenize identifiers and keywords. It identifies whether the token
 * represents a keyword or an identifier and creates a corresponding `Token` object.
 * The detected token is added to the `tokens` list for further processing.
 *
 * @details This method examines transitions within the lexical graph node and determines
 * whether the lexeme is a keyword or an identifier. Keywords are compared against a predefined
 * list of language keywords. If the lexeme is a keyword, it is classified as a `KEYWORD` token;
 * otherwise, it is classified as an `IDENTIFIER` token. In the case of an unrecognized token,
 * an `UNKNOWN` token is created.
 *
 * @param node A pointer to the lexical graph node to be classified.
 *
 * @see Token
 * @see TokenType
 * @see clearTokens
 *
 * @return void
 */
void LexGraphID::classify(NhykLexicalNode* node) {
    std::cout << "Classifying Transitions Found <=> ";
    Token token;
    // ... [token classification logic]
    if(node->terminal){
        static const std::vector<std::string> keywords = {
            "PROG", "FUNC", "BEGIN", "VAR", "INTEGER", "DOUBLE",
            "STRING", "RETURN", "END", "INPUT", "OUTPUT", "FOR",
            "TO", "NOT", "WHILE", "BOOL", "TRUE", "FALSE", "IS", "IN",
            "IF", "ELIF", "ELSE", "THEN", "CASE", "VALIDATE",
            "MATCH", "CHECK", "ENUM", "AND"
        };
        std::string lexeme = source.substr(0, position);
        if (std::find(keywords.begin(), keywords.end(), lexeme) != keywords.end()) {
            std::cout << "KEYWORD ";
            token = Token(TokenType::KEYWORD, lexeme);
        }else{
            std::cout << "ID ";
            token = Token(TokenType::IDENTIFIER, lexeme);
        }
    }else{
        token = Token(TokenType::UNKNOWN, source.substr(0, 1));
    }std::cout << "Lexeme: " << token.getLexeme() << std::endl;
    tokens.push_back(token);
    source = source.substr(position);// Update the source to remove the tokenized part
    position = 0; // Reset the position
    clearTokens(); // Clear the tokens list
}

LexGraphOperator::LexGraphOperator() : s1(), s2(){
    start = &s1;

    s1.name = "s1";
    s1.terminal = false;
    s1.transitions['+'] = &s2;
    s1.transitions['-'] = &s2;
    s1.transitions['*'] = &s2;
    s1.transitions['/'] = &s2;
    s1.transitions['='] = &s2;

    s2.name = "s2";
    s2.terminal = true;
}

void LexGraphOperator::classify(NhykLexicalNode* node){
    std::cout << "Classifying Transitions Found <=> ";
    Token token;
    if (node->terminal) {
        std::cout << "OPERATOR ";
        token = Token(TokenType::OPERATOR, source.substr(0, position));
    }else{
        token = Token(TokenType::UNKNOWN, source.substr(0, 1));
    }std::cout << "Lexema: " << token.getLexeme() << std::endl;
    tokens.push_back(token);
    source = source.substr(position);// Update the source to remove the tokenized part
    position = 0; // Reset the position
    clearTokens(); // Clear the tokens list
}

// --- LexGraphLiteral Implementation ---

LexGraphLiteral::LexGraphLiteral() : s1(), s2(), s3(), s_error() {
    // Initial state
    start = &s1;

    s1.name = "s1";
    s1.terminal = false;
    for(char c = '0'; c <= '9'; c++) s1.transitions[c] = &s2;

    s2.name = "s2";
    s2.terminal = true;
    for(char c = '0'; c <= '9'; c++) s2.transitions[c] = &s2;
    s2.transitions['.'] = &s3;

    s3.name = "s3";
    s3.terminal = true;
    for(char c = '0'; c <= '9'; c++) s3.transitions[c] = &s3;

    // Error state
    s_error.name = "s_error";
    // This is a terminal state that will capture invalid numbers.
    s_error.terminal = true;

    for(char c = 'a'; c <= 'z'; c++) {
        s_error.transitions[c] = &s_error;
        s2.transitions[c] = &s_error;
        s3.transitions[c] = &s_error;
    }

    for(char c = 'A'; c <= 'Z'; c++) {
        s_error.transitions[c] = &s_error;
        s2.transitions[c] = &s_error;
        s3.transitions[c] = &s_error;
    }
}


void LexGraphLiteral::classify(NhykLexicalNode* node) {
    std::cout << "Classifying Transitions Found <=> ";
    Token token;
    std::string lexeme = source.substr(0, position);
    if(node == &s2) {
        if(lexeme.find('.') == std::string::npos) {
            std::cout << "INTEGER LITERAL ";
            token = Token(TokenType::INT_LITERAL, lexeme);
        } else {
            std::cout << "DOUBLE LITERAL ";
            token = Token(TokenType::DOUBLE_LITERAL, lexeme);
        }
    } else if(node == &s3) {
        std::cout << "DOUBLE LITERAL ";
        token = Token(TokenType::DOUBLE_LITERAL, lexeme);
    } else if(node == &s_error) {
        std::cout << "UNKNOWN ";
        token = Token(TokenType::UNKNOWN, lexeme);
    }else {
        token = Token(TokenType::UNKNOWN, source.substr(0, 1));
    }
    std::cout << "Lexeme: " << token.getLexeme() << std::endl;
    tokens.push_back(token);
    source = source.substr(position);// Update the source to remove the tokenized part
    position = 0; // Reset the position
    clearTokens(); // Clear the tokens list
}

LexGraphPunctuation::LexGraphPunctuation() : s1() {
    //FSM responsible for recognizing operators and punctualtions
    start = &s1;

    s1.name = "s1";
    s1.terminal = true;
    s1.transitions[':'] = &s1;
    s1.transitions[';'] = &s1;
    s1.transitions[','] = &s1;
    s1.transitions['.'] = &s1;
    s1.transitions['('] = &s1;
    s1.transitions[')'] = &s1;
    s1.transitions['{'] = &s1;
    s1.transitions['}'] = &s1;
}

void LexGraphPunctuation::classify(NhykLexicalNode* node) {
    std::cout << "Classifying Transitions Found <=> ";
    Token token;
    std::string lexeme = source.substr(0, position);
    if(node->terminal) {
        std::cout << "PUNCTUATION ";
        token = Token(TokenType::PUNCTUATION, lexeme);
    } else {
        token = Token(TokenType::UNKNOWN, source.substr(0, 1));
    }
    std::cout << "Lexeme: " << token.getLexeme() << std::endl;
    tokens.push_back(token);
    source = source.substr(position);// Update the source to remove the tokenized part
    position = 0; // Reset the position
    clearTokens(); // Clear the tokens list
}

NhykLexer::NhykLexer(const std::string& src) : source(src), position(0) {}

// --- where token processing happens:
/**
 * @brief Tokenizes the input source code and populates the `tokens` vector.
 *
 * The `tokenize` method processes the input source code character by character,
 * identifying and tokenizing different types of tokens such as identifiers, literals,
 * operators, punctuation, and string literals. It utilizes various lexical state machines
 * to recognize and classify these tokens. When a valid token is identified, it is added
 * to the `tokens` vector. If an unknown token is encountered, an error message is
 * displayed to the standard error stream.
 *
 * @details This method iterates through the source code, character by character, and uses
 * lexical state machines (FSMs) to classify and tokenize the input. It handles whitespace,
 * identifiers, literals, operators, punctuation, and string literals. Detected tokens are
 * added to the `tokens` vector.
 *
 * @see NhykLexicalNode
 * @see LexGraph
 * @see LexGraphID
 * @see LexGraphStringLiteral
 * @see LexGraphOperator
 * @see LexGraphLiteral
 * @see LexGraphPunctuation
 *
 * @return void
 */
void NhykLexer::tokenize() {
    while (position < source.size()) {
        char currentChar = source[position];
        // Check if the current character is a whitespace character
        if (std::isspace(currentChar)) {
            position++;  // skip whitespace characters
            continue;  // continue to the next iteration of the loop
        }else if (std::isalpha(currentChar)) {
            idFSM.setSource(getSource().substr(position));
            idFSM.publicTraverse(idFSM.getStartNode());  // start the traversal from the starting state
            tokens.push_back(idFSM.getTokens()[0]);
            position += idFSM.getTokens()[0].getLexeme().length();
            // Clear the FSM's source to avoid any potential side effects in the next iterations
            idFSM.setSource("");
        } else if (std::isdigit(currentChar)) {
            literalFSM.setSource(source.substr(position));
            literalFSM.publicTraverse(literalFSM.getStartNode());
            tokens.push_back(literalFSM.getTokens()[0]);
            position += literalFSM.getTokens()[0].getLexeme().length();
            // Clear the FSM's source to avoid any potential side effects in the next iterations
            literalFSM.setSource("");
        } else if (currentChar == '+' || currentChar == '-' ||
                   currentChar == '*' || currentChar == '/' || currentChar == '='){
            operatorFSM.setSource(source.substr(position));
            operatorFSM.publicTraverse(operatorFSM.getStartNode());
            tokens.push_back(operatorFSM.getTokens()[0]);
            position += operatorFSM.getTokens()[0].getLexeme().length();
            // Clear the FSM's source to avoid any potential side effects in the next iterations
            operatorFSM.setSource("");
        } else if (currentChar == ':' || currentChar == ';' || currentChar == ',' ||
                   currentChar == '.' || currentChar == '(' || currentChar == ')' ||
                   currentChar == '{' || currentChar == '}') {
            punctuationFSM.setSource(source.substr(position));
            punctuationFSM.publicTraverse(punctuationFSM.getStartNode());
            tokens.push_back(punctuationFSM.getTokens()[0]);
            position += punctuationFSM.getTokens()[0].getLexeme().length();
            // Clear the FSM's source to avoid any potential side effects in the next iterations
            punctuationFSM.setSource("");
        } else if (currentChar == '"') {
            stringLiteralFSM.setSource(source.substr(position));
            stringLiteralFSM.publicTraverse(stringLiteralFSM.getStartNode());
            tokens.push_back(stringLiteralFSM.getTokens()[0]);
            position += stringLiteralFSM.getTokens()[0].getLexeme().length();
            // Clear the FSM's source to avoid any potential side effects in the next iterations
            stringLiteralFSM.setSource("");
        } else if (std::isspace(currentChar)) {
            position++;  // skip spaces
        } else {
            // Error handling for unknown tokens
            std::cerr << "Error at position " << position << ": Unknown token \""
                      << currentChar << "\"." << std::endl;
            position++;
        }
    }
}

std::string NhykLexer::getSource() const {return source;}
void NhykLexer::setSource(const std::string& newSource){source = newSource;}
const std::vector<Token>& NhykLexer::getTokens() const {return tokens;}
