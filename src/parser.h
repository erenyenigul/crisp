#ifndef PARSER_H
#define PARSER_H
#include "list.h"
#include "scanner.h"

struct _function;
struct _cls;
struct _instance;
typedef struct _function function;
typedef struct _cls cls;
typedef struct _instance instance;

typedef enum
{
    V_ANY,
    V_INT,
    V_STRING,
    V_NULL,
    V_IDENTIFIER,
    V_FUNCTION,
    V_FILE,
    V_BOOL,
    V_REF,
    V_CLASS
} expression_value_type;

typedef enum
{
    E_MODULE,
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
    E_OPEN,
    E_READ,
    E_FOR,
    E_WRITE,
    E_CLOSE,
    E_IF,
    E_NEGATION,
    E_EQUAL,
    E_DEFINE,
    E_IMPORT,
    E_BEGIN,
    E_AND,
    E_OR,
    E_GREATER,
    E_GREATER_EQUAL,
    E_LESS,
    E_LESS_EQUAL,
    E_LIST,
    E_PUSH,
    E_GET,
    E_POP,
    E_INSERT,
    E_CLASS
} expression_type;

typedef union
{
    int i;
    char *str;
    int null;
    function *func;
    FILE *file;
    bool boolean;
    array_list *ref;
    cls* cls;
    instance* instance;
} expression_values;

typedef struct
{
    expression_values val;
    expression_value_type type;
} expression_value;

typedef struct
{
    // The type of the expression
    expression_type type;
    expression_value value;
    array_list *exps;

    // To be used for error reporting
    int line;
} expression;

typedef struct environment
{
    char *ids[100];
    expression_value vals[100];
    int i;
} environment;

struct _function
{
    array_list *ids;
    expression *body;
    environment env;
};

struct _cls 
{
    environment definitions;
};

struct _instance 
{
    environment env;
} ;

typedef struct
{
    array_list *expressions;
} stack;

expression *parse(tokens *token_collection, char *program);
#endif