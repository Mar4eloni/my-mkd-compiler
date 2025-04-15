#ifndef PARSER_H
#define PARSER_H

#include "Common.h"
#include "Lexer.h"

// AST Node types
typedef enum {
    NODE_PROGRAM,
    NODE_VARIABLE_DECL,
    NODE_FUNCTION_DECL,
    NODE_IF_STATEMENT,
    NODE_WHILE_LOOP,
    NODE_EXPRESSION,
    NODE_ASSIGNMENT
} NodeType;

// AST Node structure
typedef struct ASTNode {
    NodeType type;
    Token* token;
    struct ASTNode** children;
    int child_count;
} ASTNode;

// Parser functions
ASTNode* parse(Lexer* lexer);
void free_ast(ASTNode* node);

#endif // PARSER_H