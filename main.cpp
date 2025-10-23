#include "LexGraph.h"

/**
 * @file main.cpp
 * @brief Nhyk compiler's main function for lexical analysis.
 *
 * This source file contains the main function for performing lexical analysis
 * in the Nhyk compiler. It tokenizes the input source code and displays the resulting tokens.
 *
 * @author MNS Ahimbisibwe
 * @SN 217005435
 * @date 2023/09/03
 * @model Lexical Analysis Design
 * @version D01
 */

int main() {
    // 1. Create a sample source code string
    std::string sourceCode  = "PROG myProgram: VAR x = 10.5; FUNC add(a, b) { RETURN a + b; } 7pop, \"Mike\" ";
    std::string sourceCode2 = "PROG lotto: IF num >= 1 AND num <= 6 AND num IS NOT IN userNumbers THEN";
    std::string sourceCode3 =
                            "PROG exampleProgram: VAR count = 10, rate = 2.5, name = \"Nhyk\"; "
                            "FUNC multiply(x, y) { RETURN x * y; } BEGIN FOR i = 1 TO 5 { OUTPUT i; } "
                            "WHILE count > 0 AND name IS NOT IN [\"John\", \"Doe\"] { count = count - 1; } "
                            "IF rate > 2 THEN { rate = rate - 0.5; } ELIF rate <= 1 THEN { rate = 1; } ELSE"
                            "{ rate = rate + 0.5; } END MATCH rate { CASE 1: OUTPUT \"Low\"; CASE 2: OUTPUT"
                            "\"Medium\"; DEFAULT: OUTPUT \"High\"; } 7pop";

    // 2. Instantiate the lexer with the source code
    NhykLexer lexer(sourceCode);

    // 3. Tokenize the source code
    lexer.tokenize();
    std::string TokenTypeToString(TokenType type);


    // 4. Retrieve the tokens and display them
    const std::vector<Token>& tokens = lexer.getTokens();

    std::cout << tokens;
    std::ofstream outFile("doc\\Lexar.txt");

    if(outFile.is_open()){
        outFile << tokens;
        std::cout << "Output has been saved to 'doc/Lexar.txt'" << std::endl;
    }else
        std::cerr << "Error: Unable to open output file 'doc/Lexar.txt'" << std::endl;

    // Optionally, if implemented error handling, display any errors
    // (assuming getErrors or similar method in lexer)
    // const std::vector<ErrorToken>& errors = lexer.getErrors();
    // for (const ErrorToken& error : errors) {
    //     std::cout << "Error at Line: " << error.line << ", Column: " << error.col << " - " << error.message << std::endl;
    // }

    return 0;
}

