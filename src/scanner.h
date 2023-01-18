#ifndef SCANNER_H
#define SCANNER_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    T_OPEN_PARAN,
    T_CLOSE_PARAN,
    T_PLUS,
    T_MINUS,
    T_NUMERIC,
    T_STAR,
    T_SLASH,
    T_PRINT,
    T_STRING,
    T_IDENTIFIER,
    T_QUOTATION,
    T_LET,
    T_FUNCTION,
    T_READ,
    T_FOR,
    T_OPEN,
    T_CLOSE,
    T_WRITE,
    T_TRUE,
    T_FALSE,
    T_IF,
    T_EXCLAMATION,
    T_EQUAL,
    T_LIST,
    T_DEFINE,
    T_IMPORT,
    T_BEGIN,
    T_AND,
    T_OR,
    T_LESS,
    T_GREATER,
    T_LESS_EQUAL,
    T_GREATER_EQUAL,
    T_DOT,
    T_DOUBLE_DOT,
    T_PUSH,
    T_GET,
    T_POP,
    T_INSERT,
    T_OPEN_BRACKET,
    T_CLOSE_BRACKET,
    T_CLASS
} token_type;

typedef union
{
    int i;
    char *str;
    bool boolean;
} token_value;

typedef struct
{
    token_type type;
    token_value value;
    int line;
} token;

typedef struct
{
    token *tokens[100];
    int n;
} tokens;

tokens *scan(char *program, int line);
#endif