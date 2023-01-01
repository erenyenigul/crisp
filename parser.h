#ifndef PARSER_H
#define PARSER_H
#include "list.h"
#include "scanner.h"

struct _function;
typedef struct _function function;

typedef enum { V_ANY,
               V_INT,
               V_STRING,
               V_NULL,
               V_IDENTIFIER,
               V_FUNCTION,
               V_FILE,
               V_BOOL,
} expression_value_type;

typedef enum { E_CONST,
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
               E_EQUAL } expression_type;

typedef union {
	int i;
	char *str;
	int null;
	function *func;
	FILE *file;
	bool boolean;
} expression_values;

typedef struct {
	expression_values val;
	expression_value_type type;
} expression_value;

typedef struct {
	// The type of the expression
	expression_type type;
	expression_value value;
	array_list *exps;

	// To be used for error reporting
	int line;
} expression;

struct _function {
	array_list *ids;
	expression *body;
};

typedef struct {
	array_list *expressions;
} stack;

expression *parse(tokens *token_collection);
#endif