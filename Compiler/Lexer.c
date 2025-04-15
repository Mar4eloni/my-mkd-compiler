#define _CRT_SECURE_NO_WARNINGS // god send !
#include "Lexer.h"
#include "Common.h"
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
// Initialize lexer
Lexer* init_lexer(wchar_t* source) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->source = source;
    lexer->current = source;
    lexer->line = 1;
    lexer->column = 1;
    return lexer;
}

// Free lexer resources
void free_lexer(Lexer* lexer) {
    free(lexer);
}

// Check if character is Macedonian Cyrillic
int is_macedonian_letter(wchar_t c) {
    // Basic Macedonian Cyrillic range
    return (c >= L'А' && c <= L'Я') ||
        (c >= L'а' && c <= L'я') ||
        c == L'Ѓ' || c == L'ѓ' ||
        c == L'Ќ' || c == L'ќ' ||
        c == L'Ѕ' || c == L'ѕ' ||
        c == L'Ј' || c == L'ј';
}

// Get next token - THIS WAS A HEAD ACHE !
Token* next_token(Lexer* lexer) {
    if (!lexer || !lexer->current) {
        Token* token = malloc(sizeof(Token));
        token->type = T_EOF;
        token->value = NULL;
        token->line = lexer ? lexer->line : 0;
        token->column = lexer ? lexer->column : 0;
        return token;
    }

    // Skip whitespace
    while (*lexer->current == L' ' || *lexer->current == L'\t' || *lexer->current == L'\n') {
        if (*lexer->current == L'\n') {
            lexer->line++;
            lexer->column = 1;
        }
        else {
            lexer->column++;
        }
        lexer->current++;
    }

    // Check for EOF
    if (*lexer->current == L'\0') {
        Token* token = malloc(sizeof(Token));
        token->type = T_EOF;
        token->value = _wcsdup(L"EOF");  // Changed to _wcsdup
        token->line = lexer->line;
        token->column = lexer->column;
        return token;
    }

    Token* token = malloc(sizeof(Token));
    token->line = lexer->line;
    token->column = lexer->column;

    // Handle numbers
    if (iswdigit(*lexer->current)) {
        token->type = T_NUMBER;
        wchar_t* start = lexer->current;
        while (iswdigit(*lexer->current)) {
            lexer->current++;
            lexer->column++;
        }
        size_t length = lexer->current - start;
        token->value = calloc(length + 1, sizeof(wchar_t));
        wcsncpy_s(token->value, length + 1, start, length);  // Secure version
        return token;
    }

    // Handle Macedonian identifiers/keywords
    if (is_macedonian_letter(*lexer->current)) {
        wchar_t* start = lexer->current;
        while (is_macedonian_letter(*lexer->current) || iswdigit(*lexer->current)) {
            lexer->current++;
            lexer->column++;
        }
        size_t length = lexer->current - start;
        token->value = calloc(length + 1, sizeof(wchar_t));
        wcsncpy_s(token->value, length + 1, start, length);  // Secure version

        if (wcscmp(token->value, L"ако") == 0 ||
            wcscmp(token->value, L"тогаш") == 0 ||
            wcscmp(token->value, L"додека") == 0 ||
            wcscmp(token->value, L"крај") == 0 ||
            wcscmp(token->value, L"испечати") == 0) {
            token->type = T_KEYWORD;
        }
        else {
            token->type = T_IDENTIFIER;
        }
        return token;
    }

    // Handle operators
    if (*lexer->current == L'+' || *lexer->current == L'-' ||
        *lexer->current == L'*' || *lexer->current == L'/' ||
        *lexer->current == L'>' || *lexer->current == L'<' ||
        *lexer->current == L'=') {
        token->type = T_OPERATOR;
        token->value = calloc(2, sizeof(wchar_t));
        token->value[0] = *lexer->current;
        token->value[1] = L'\0';
        lexer->current++;
        lexer->column++;
        return token;
    }

    // Handle strings
    if (*lexer->current == L'"') {
        token->type = T_STRING;
        lexer->current++;
        lexer->column++;
        wchar_t* start = lexer->current;
        while (*lexer->current != L'"' && *lexer->current != L'\0') {
            lexer->current++;
            lexer->column++;
        }
        size_t length = lexer->current - start;
        token->value = calloc(length + 1, sizeof(wchar_t));
        wcsncpy_s(token->value, length + 1, start, length);  // Secure version
        if (*lexer->current == L'"') {
            lexer->current++;
            lexer->column++;
        }
        return token;
    }

    // Handle delimiters
    if (*lexer->current == L'(' || *lexer->current == L')' ||
        *lexer->current == L'{' || *lexer->current == L'}' ||
        *lexer->current == L';' || *lexer->current == L',') {
        token->type = T_DELIMITER;
        token->value = calloc(2, sizeof(wchar_t));
        token->value[0] = *lexer->current;
        token->value[1] = L'\0';
        lexer->current++;
        lexer->column++;
        return token;
    }

    // Default case
    token->type = T_UNKNOWN;
    token->value = calloc(2, sizeof(wchar_t));
    token->value[0] = *lexer->current;
    token->value[1] = L'\0';
    lexer->current++;
    lexer->column++;
    return token;
}