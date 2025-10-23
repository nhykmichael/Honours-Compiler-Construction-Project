#include "Token.h"

/**
 * @file Token.cpp
 * @brief Defines the Token class for lexical analysis in the Nhyk compiler.
 *
 * This file contains the definition of the Token class, which represents
 * individual tokens generated during lexical analysis in the Nhyk compiler.
 *
 * @author MNS Ahimbisibwe
 * @date [2023/09/03]
 * @model Lexical Analysis Design
 * @version [D01]
 */

Token::Token(TokenType t, const std::string& lex) : type(t), lexeme(lex) {}
Token::Token() : type(TokenType::UNKNOWN), lexeme("") {}


TokenType Token::getType() const {
    return type;
}

const std::string& Token::getLexeme() const {
    return lexeme;
}

#include <stdexcept>

std::string TokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::KEYWORD:         return "KEYWORD";
        case TokenType::IDENTIFIER:      return "IDENTIFIER";
        case TokenType::LITERAL:         return "LITERAL";
        case TokenType::PUNCTUATION:     return "PUNCTUATION";
        case TokenType::OPERATOR:        return "OPERATOR";
        case TokenType::BOOLEAN_LITERAL: return "BOOLEAN_LITERAL";
        case TokenType::INT_LITERAL:     return "INT_LITERAL";
        case TokenType::DOUBLE_LITERAL:  return "DOUBLE_LITERAL";
        case TokenType::UNKNOWN:         return "UNKNOWN";
        default:                         throw std::runtime_error("Unknown TokenType");
    }
}

std::ostream& operator<<(std::ostream& os, const std::vector<Token>& tokens) {
    // Header
    os << "\n TokenType\t\tLexeme: Lex\n";
    os << "------------------------------------\n";

    // Iterate through the tokens and print them
    for (const Token& token : tokens) {
		if (token.getType() == TokenType::KEYWORD || 
			token.getType() == TokenType::OPERATOR || 
			token.getType() == TokenType::LITERAL || 
			token.getType() == TokenType::UNKNOWN) {

			os << "Type: " << TokenTypeToString(token.getType()) << ":\t"
			   << "\tLexeme: " << token.getLexeme() << "\n";
		}else{
			os <<"Type: "<< TokenTypeToString(token.getType())
			   << ":\t" << "Lexeme: " <<token.getLexeme() <<"\n";
		}
        
    }return os;
}
