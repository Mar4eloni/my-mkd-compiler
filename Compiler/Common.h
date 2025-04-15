#ifndef COMMON_H
#define COMMON_H

#include <wchar.h>
#include <locale.h>

// Token types
typedef enum {
    T_KEYWORD,
    T_IDENTIFIER,
    T_NUMBER,
    T_OPERATOR,
    T_DELIMITER,
    T_STRING,
    T_EOF,
    T_UNKNOWN
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    wchar_t* value;
    int line;
    int column;
} Token;

// Function to check Macedonian Cyrillic characters
int is_macedonian_letter(wchar_t c);

#endif 