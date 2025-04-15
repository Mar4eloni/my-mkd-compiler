#ifndef LEXER_H
#define LEXER_H

#include "Common.h"

// Lexer state
typedef struct {
    wchar_t* source;
    wchar_t* current;
    int line;
    int column;
} Lexer;

// Lexer functions
Lexer* init_lexer(wchar_t* source);
void free_lexer(Lexer* lexer);
Token* next_token(Lexer* lexer);

#endif // LEXER_H