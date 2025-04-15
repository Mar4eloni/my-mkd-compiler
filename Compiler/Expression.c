#include "Expression.h"
#include <stdlib.h>
#include <wchar.h>

static Expression* parse_additive(Lexer* lexer);
static Expression* parse_multiplicative(Lexer* lexer);
static Expression* parse_primary(Lexer* lexer);

Expression* parse_expression(Lexer* lexer) {
    return parse_additive(lexer);
}

static Expression* parse_additive(Lexer* lexer) {
    Expression* left = parse_multiplicative(lexer);
    Token* token = next_token(lexer);

    while (token && token->type == T_OPERATOR &&
        (wcscmp(token->value, L"+") == 0 || wcscmp(token->value, L"-") == 0)) {
        Expression* expr = malloc(sizeof(Expression));
        expr->type = EXPR_BINARY;
        expr->token = token;
        expr->left = left;
        expr->right = parse_multiplicative(lexer);
        left = expr;
        token = next_token(lexer);
    }

    if (token) {
        lexer->current -= wcslen(token->value);
        free(token->value);
        free(token);
    }
    return left;
}

static Expression* parse_multiplicative(Lexer* lexer) {
    Expression* left = parse_primary(lexer);
    Token* token = next_token(lexer);

    while (token && token->type == T_OPERATOR &&
        (wcscmp(token->value, L"*") == 0 || wcscmp(token->value, L"/") == 0)) {
        Expression* expr = malloc(sizeof(Expression));
        expr->type = EXPR_BINARY;
        expr->token = token;
        expr->left = left;
        expr->right = parse_primary(lexer);
        left = expr;
        token = next_token(lexer);
    }

    if (token) {
        lexer->current -= wcslen(token->value);
        free(token->value);
        free(token);
    }
    return left;
}

static Expression* parse_primary(Lexer* lexer) {
    Token* token = next_token(lexer);
    if (!token) return NULL;

    Expression* expr = malloc(sizeof(Expression));
    if (!expr) {
        free(token->value);
        free(token);
        return NULL;
    }

    if (token->type == T_NUMBER) {
        expr->type = EXPR_LITERAL;
        expr->token = token;
        expr->left = expr->right = NULL;
    }
    else if (token->type == T_IDENTIFIER) {
        expr->type = EXPR_VARIABLE;
        expr->token = token;
        expr->left = expr->right = NULL;
    }
    else if (wcscmp(token->value, L"(") == 0) {
        free(token->value);
        free(token);
        expr = parse_expression(lexer);
        token = next_token(lexer);
        if (!token || wcscmp(token->value, L")") != 0) {
            free_expression(expr);
            if (token) {
                free(token->value);
                free(token);
            }
            return NULL;
        }
        free(token->value);
        free(token);
    }
    else {
        free(token->value);
        free(token);
        free(expr);
        return NULL;
    }

    return expr;
}

int evaluate_expression(Expression* expr) {
    if (!expr) return 0;

    switch (expr->type) {
    case EXPR_LITERAL:
        return _wtoi(expr->token->value);

    case EXPR_BINARY: {
        int left = evaluate_expression(expr->left);
        int right = evaluate_expression(expr->right);

        if (wcscmp(expr->token->value, L"+") == 0) return left + right;
        if (wcscmp(expr->token->value, L"-") == 0) return left - right;
        if (wcscmp(expr->token->value, L"*") == 0) return left * right;
        if (wcscmp(expr->token->value, L"/") == 0) return left / right;
        return 0;
    }

    default:
        return 0;
    }
}

void free_expression(Expression* expr) {
    if (!expr) return;
    free_expression(expr->left);
    free_expression(expr->right);
    if (expr->token) {
        free(expr->token->value);
        free(expr->token);
    }
    free(expr);
}