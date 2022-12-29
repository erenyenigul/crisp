#ifndef SCANNER_H
#define SCANNER_H
#include <stdio.h>
#include <stdbool.h>
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
    T_IF
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
} token;

typedef struct
{
    token *tokens[100];
    int n;
} tokens;

tokens *scan(char *program, int n);
#endif