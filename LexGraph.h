#ifndef LEXGRAPH_H_INCLUDED
#define LEXGRAPH_H_INCLUDED

/**
 * @file LexGraph.h
 * @brief Defines the LexGraph class for lexical analysis in the Nhyk compiler.
 *
 * This file contains the definition of the LexGraph class, which represents a
 * lexical graph used for tokenization and parsing in the Nhyk compiler.
 *
 * @author MNS Ahimbisibwe
 * @SN 217005435
 * @date [2023/09/03]
 * @model Lexical Analysis Design
 * @version [D01]
 */

#include <map>
#include "Token.h"

class NhykLexicalNode {
    /*This header class style adapted from: [Prof DA Coulter's Example]
    *Source URL: [https://eve.uj.ac.za/lectures.php#lecture-it08x87]
    */
    /**
     * @file LexGraph.h
     * @brief Defines the NhykLexicalNode class for building lexical graphs.
     *
     * This file contains the definition of the NhykLexicalNode class, which is used
     * to construct nodes in lexical graphs for tokenization and parsing.
     *
     @author MNS Ahimbisibwe
     * @SN 217005435
     * @date [2023/09/03]
     * @model Lexical Analysis Design
     * @version [D01]
     */

public:
    std::string name;
    std::map<char, NhykLexicalNode*> transitions;
    bool terminal;
};

/*
*   LexGraph maintains a starting state('start'), set of transitions between states
*   (encorded in 'NhykLexicalNode' objects) and a method ('classify') to determine the type of token
*   once traversal ends
*/
class LexGraph {
    /*
    * The LexGraph class represents a lexical graph for parsing and tokenization.
    * Some parts of this header class were adapted from Prof DA Coulter's Example.
    * Source URL: [https://eve.uj.ac.za/lectures.php#lecture-it08x87]
    */

protected:
    std::string source;                // The source string to be tokenized.
    //Using an unsigned type can help catch bugs in additional to can represent large number
    unsigned int position;             // Current position in the source string.
    NhykLexicalNode* start;            // Starting node of the lexical graph.
    std::vector<Token> tokens;         // Stores the tokens extracted from the source.

    // Traverses the lexical graph from the given node.
    virtual void traverse(NhykLexicalNode* node);

    // Classifies the given node. Implementation is provided by derived classes.
    virtual void classify(NhykLexicalNode* node) = 0;

public:
    LexGraph();                        // Default constructor.
    virtual ~LexGraph();               // Destructor. Use 'virtual' for proper destruction in derived classes.

    // Public interface to the traverse function for external use.
    void publicTraverse(NhykLexicalNode* node){traverse(node);}

    // Overload stream insertion operator for output.
    // This method will be called whenever '<<' is invoked on the object
    friend std::ostream& operator<<(std::ostream& out, const LexGraph& graph);

    // Getter for the source string.
    std::string getSource() const;

    // Getter for the tokens vector.
    std::vector<Token>& getTokens();

    // Setter for the source string.
    void setSource(const std::string& newSource);

    // Getter for the starting node of the lexical graph.
    NhykLexicalNode* getStartNode() const {return start;}

    // Setter for the starting node of the lexical graph.
    void setStartNode(NhykLexicalNode* node){start = node;}

    void clearTokens();
};


class LexGraphID : public LexGraph {

private:
    NhykLexicalNode s1;
    NhykLexicalNode s2;
    NhykLexicalNode s_error;

public:
    LexGraphID();
    void classify(NhykLexicalNode* node) override;
};

// --- LexGraphStringLiteral Implementation ---
class LexGraphStringLiteral : public LexGraph{
public:
    LexGraphStringLiteral();
    void classify(NhykLexicalNode* node) override;
private:
    NhykLexicalNode s1, s2, s3;
};

/*LexGraphOperator derived from LexGraph base class. Derived class will implement
the FSM for recognizing operators based on the transitions between the states
Designed to recognize basic operators like '+'...*/
class LexGraphOperator : public LexGraph{
private:
    NhykLexicalNode s1;
    NhykLexicalNode s2;
public:
    LexGraphOperator();
    void classify(NhykLexicalNode* node) override;
};

class LexGraphLiteral : public LexGraph{
private:
    NhykLexicalNode s1, s2, s3, s_error;
public:
    LexGraphLiteral();
    void classify(NhykLexicalNode* node) override;
};

class LexGraphPunctuation : public LexGraph {
private:
    NhykLexicalNode s1;

public:
    LexGraphPunctuation();
    void classify(NhykLexicalNode* node) override;
};

class NhykLexer {
private:
    std::string source;
    unsigned int position;
    std::vector<Token> tokens;

    LexGraphID idFSM;
    LexGraphStringLiteral stringLiteralFSM;
    LexGraphOperator operatorFSM;
    LexGraphLiteral literalFSM;
    LexGraphPunctuation punctuationFSM;

public:
    NhykLexer(const std::string& src);
    void tokenize();
    const std::vector<Token>& getTokens() const;
    // Getter for source
    std::string getSource() const;
    // Setter for source
    void setSource(const std::string& newSource);
};

#endif // LEXGRAPH_H_INCLUDED
