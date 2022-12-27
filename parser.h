#ifndef PARSER_H
#define PARSER_H
#include "scanner.h"

struct _function;
typedef struct _function function;

typedef enum
{
    V_INT,
    V_STRING,
    V_NULL,
    V_IDENTIFIER,
    V_FUNCTION,
    V_FILE
} expression_value_type;

enum expression_type
{
    E_CONST,
    E_ADDITION,
    E_SUBTRACTION,
    E_MULTIPLICATION,
    E_DIVISON,
    E_PRINT,
    E_VARIABLE,
    E_LET,
    E_IDENTIFIER,
    E_FUNCTION,
    E_CALL,
    E_OPEN
};

typedef union
{
    int i;
    char *str;
    int null;
    function *func;
} expression_values;

typedef struct
{
    expression_values val;
    expression_value_type type;
} expression_value;

struct _expression
{
    enum expression_type type;
    int num_exps;
    int filled;
    expression_value value;

    struct _expression **exps;
};

typedef struct _expression expression;

struct _function{
    char* id;
    expression* body;   
};

typedef struct
{
    expression *arr[300];
    int i;
} stack;

expression* parse(tokens *token_collection);
#endif