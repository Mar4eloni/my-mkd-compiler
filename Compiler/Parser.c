#include "Parser.h"
#include "Lexer.h"
#include "Common.h"
#include <stdlib.h>

ASTNode* parse(Lexer* lexer) {
    // Implementation of your parsing logic
    // This would create the AST from tokens
    return NULL; // Return root node of AST
}

void free_ast(ASTNode* node) {
    // Recursively free AST nodes
    if (node == NULL) return;

    for (int i = 0; i < node->child_count; i++) {
        free_ast(node->children[i]);
    }

    free(node->children);
    free(node->token);
    free(node);
}