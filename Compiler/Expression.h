#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Common.h"
#include "Lexer.h"

// First declare the enum
typedef enum {
    EXPR_BINARY,
    EXPR_UNARY,
    EXPR_LITERAL,
    EXPR_VARIABLE
} ExpressionType;

// Then declare the struct
typedef struct Expression {
    ExpressionType type;
    Token* token;
    struct Expression* left;
    struct Expression* right;
} Expression;

// Function declarations
Expression* parse_expression(Lexer* lexer);
void free_expression(Expression* expr);

// Helper functions (only needed in Expression.c)
Expression* parse_additive(Lexer* lexer);
Expression* parse_multiplicative(Lexer* lexer);
Expression* parse_primary(Lexer* lexer);

#endif // EXPRESSION_H