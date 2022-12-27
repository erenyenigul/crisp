#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct _function;
typedef struct _function function;

enum token_type
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
};

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
    E_CALL
};

typedef enum
{
    V_INT,
    V_STRING,
    V_NULL,
    V_IDENTIFIER,
    V_FUNCTION
} expression_value_type;

typedef union
{
    int i;
    char *str;
} token_value;

typedef struct
{
    enum token_type type;
    token_value value;
} token;

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

token *create_token(enum token_type type)
{
    token *t = malloc(sizeof(token));
    t->type = type;

    return t;
}

expression *create_expression(enum expression_type type, int num_exp)
{
    expression *exp = malloc(sizeof(expression));
    exp->exps = malloc(sizeof(expression *) * num_exp);
    exp->num_exps = num_exp;
    exp->filled = 0;
    exp->type = type;

    return exp;
}

typedef struct
{
    expression *arr[300];
    int i;
} stack;

void push(stack *s, expression *exp)
{
    s->arr[s->i++] = exp;
}

expression *pop(stack *s)
{
    return s->arr[--s->i];
}

expression *top(stack *s)
{
    return s->arr[(s->i) - 1];
}

typedef struct
{
    char *ids[100];
    expression_value vals[100];
    int i;
} enviroment;

int str2int(const char *str, int len)
{
    int i;
    int ret = 0;
    for (i = 0; i < len; ++i)
    {
        ret = ret * 10 + (str[i] - '0');
    }
    return ret;
}

void printe(char *msg)
{
    printf("\033[1m\033[31m[Crisp.Exception]\033[0m %s\n", msg);
}
