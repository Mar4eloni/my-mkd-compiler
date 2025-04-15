#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <assert.h>
#include "Lexer.h"
#include "Parser.h"
#include "Expression.h"

void init_locale() {
    setlocale(LC_ALL, "mk_MK.UTF-8");
}

void test_lexer(Lexer* lexer) {
    wprintf(L"\n=== Lexer Output ===\n");
    //Token* token; - throws error in the if for access violation when declared here ?! weird !


    while (1) {
        Token* token = next_token(lexer);

        // First check if token is NULL
        if (!token) {
            wprintf(L"Error: Received NULL token\n");
            break;
        }

        // Then check for EOF
        if (token->type == T_EOF) {
            wprintf(L"End of file\n");
            if (token->value) free(token->value);
            free(token);
            break;
        }

        // Safely access token members
        wprintf(L"Line %d, Column %d: ", token->line, token->column);

        switch (token->type) {
        case T_KEYWORD: wprintf(L"Keyword: %ls\n", token->value); break;
        case T_IDENTIFIER: wprintf(L"Identifier: %ls\n", token->value); break;
        case T_NUMBER: wprintf(L"Number: %ls\n", token->value); break;
        case T_OPERATOR: wprintf(L"Operator: %ls\n", token->value); break;
        case T_STRING: wprintf(L"String: %ls\n", token->value); break;
        case T_DELIMITER: wprintf(L"Delimiter: %ls\n", token->value); break;
        default: wprintf(L"Unknown token type: %d\n", token->type);
        }

        // Cleanup
        if (token->value) free(token->value);
        free(token);
    }
}

void print_ast(ASTNode* node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; i++) wprintf(L"  ");

    switch (node->type) {
    case NODE_PROGRAM: wprintf(L"PROGRAM\n"); break;
    case NODE_IF_STATEMENT: wprintf(L"IF\n"); break;
    case NODE_WHILE_LOOP: wprintf(L"WHILE\n"); break;
    case NODE_EXPRESSION:
        wprintf(L"EXPRESSION");
        if (node->token) wprintf(L" (Token: %ls)", node->token->value);
        wprintf(L"\n");
        break;
    default: wprintf(L"UNKNOWN NODE TYPE\n");
    }

    for (int i = 0; i < node->child_count; i++) {
        print_ast(node->children[i], depth + 1);
    }
}

void test_expression_evaluation(Lexer* lexer, int expected) {
    Expression* expr = parse_expression(lexer);
    if (expr) {
        int result = evaluate_expression(expr);
        wprintf(L"\n=== Evaluation ===\n");
        wprintf(L"Expression: %ls\n", lexer->source);
        wprintf(L"Expected: %d | Actual: %d | %s\n",
            expected, result,
            result == expected ? "PASS" : "FAIL");
        free_expression(expr);
    }
}

int main() {
    init_locale();

    struct {
        wchar_t* source;
        int expected;
    } tests[] = {
        {L"5 + 3 * 2", 11},
        {L"(5 + 3) * 2", 16},
        {L"10 - 4 / 2", 8},
        {L"2 * 3 + 4 * 5", 26},
        {L"ако x > 5 тогаш испечати x крај", 0},
        {NULL, 0}
    };

    for (int i = 0; tests[i].source; i++) {
        wprintf(L"\n\n===== TEST %d =====\n", i + 1);
        Lexer* lexer = init_lexer(tests[i].source);

        // Test lexer
        Lexer* lex_copy = init_lexer(tests[i].source);
        test_lexer(lex_copy);
        free_lexer(lex_copy);

        // Test evaluation for math expressions
        if (tests[i].expected != 0) {
            Lexer* eval_lexer = init_lexer(tests[i].source);
            test_expression_evaluation(eval_lexer, tests[i].expected);
            free_lexer(eval_lexer);
        }

        // Test parser
        ASTNode* ast = parse(lexer);
        wprintf(L"\n=== AST ===\n");
        print_ast(ast, 0);

        free_ast(ast);
        free_lexer(lexer);
    }

    return 0;
}