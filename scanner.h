#ifndef SCANNER_H
#define SCANNER_H
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
    T_FUNCTION
} token_type;

typedef union
{
    int i;
    char *str;
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