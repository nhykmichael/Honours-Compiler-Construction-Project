#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

/**
 * @file Token.h
 * @brief Defines the Token class for lexical analysis in the Nhyk compiler.
 *
 * This file contains the definition of the Token class, which represents
 * individual tokens generated during lexical analysis in the Nhyk compiler.
 *
 * @author MNS Ahimbisibwe
 * @SN 217005435
 * @date [2023/09/03]
 * @model Lexical Analysis Design
 * @version [D01]
 */

enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    LITERAL,
    PUNCTUATION,
    OPERATOR,
    BOOLEAN_LITERAL,
    INT_LITERAL,
    DOUBLE_LITERAL,
    UNKNOWN
};

class Token {
private:
    TokenType type;
    std::string lexeme;

public:
    Token();
    Token(TokenType t, const std::string& lex);

    TokenType getType() const;
    const std::string& getLexeme() const;

    friend std::ostream& operator<<(std::ostream& os, const std::vector<Token>& tokens);
};

#endif // TOKEN_H
