#include "parser.h"
#include "crisp.h"
#include "list.h"
#include "scanner.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

stack *create_stack()
{
    stack *s = malloc(sizeof(stack));
    s->expressions = create_list(10, sizeof(expression));

    return s;
}

expression *push_expression(stack *s, expression_type type, int num_exp, int line)
{
    expression *exp = calloc_list(s->expressions);
    exp->type = type;
    exp->exps = create_list(num_exp, sizeof(expression));
    exp->line = line;
    return exp;
}

expression *pop(stack *s)
{
    expression *exp = malloc(sizeof(expression));
    pop_list(s->expressions, s->expressions->size - 1, exp);

    return exp;
}

expression *top(stack *s)
{
    return get_list(s->expressions, s->expressions->size - 1);
}

void destroy_stack(stack *s)
{
    destroy_list(s->expressions);
    free(s);
}

expression *create_expression(expression_type type, int num_exp)
{
    expression *exp = (expression *)malloc(sizeof(expression));
    exp->exps = create_list(num_exp, sizeof(expression));
    exp->type = type;

    return exp;
}

expression *parse(tokens *token_collection, char *program)
{
    stack *s = create_stack();
    expression *module = create_expression(E_MODULE, 1);
    token *t;
    for (int i = 0; i < token_collection->n; i++)
    {
        t = token_collection->tokens[i];

        if (t->type == T_OPEN_PARAN)
        {
            t = token_collection->tokens[++i];
            expression_type type;
            int num;
            if (t->type == T_PLUS)
            {
                type = E_ADDITION;
                num = 2;
            }
            else if (t->type == T_MINUS)
            {
                type = E_SUBTRACTION;
                num = 2;
            }
            else if (t->type == T_STAR)
            {
                type = E_MULTIPLICATION;
                num = 2;
            }
            else if (t->type == T_SLASH)
            {
                type = E_DIVISON;
                num = 2;
            }
            else if (t->type == T_PRINT)
            {
                type = E_PRINT;
                num = 1;
            }
            else if (t->type == T_LET)
            {
                type = E_LET;
                num = 3;
            }
            else if (t->type == T_DEFINE)
            {
                type = E_DEFINE;
                num = 2;
            }
            else if (t->type == T_FUNCTION)
            {
                type = E_FUNCTION;
                num = 2;
            }
            else if (t->type == T_READ)
            {
                type = E_READ;
                num = 1;
            }
            else if (t->type == T_FOR)
            {
                type = E_FOR;
                num = 2;
            }
            else if (t->type == T_OPEN)
            {
                type = E_OPEN;
                num = 2;
            }
            else if (t->type == T_IMPORT)
            {
                type = E_IMPORT;
                num = 1;
            }
            else if (t->type == T_WRITE)
            {
                type = E_WRITE;
                num = 2;
            }
            else if (t->type == T_CLOSE)
            {
                type = E_CLOSE;
                num = 1;
            }
            else if (t->type == T_IF)
            {
                type = E_IF;
                num = 2;
            }
            else if (t->type == T_EXCLAMATION)
            {
                type = E_NEGATION;
                num = 1;
            }
            else if (t->type == T_OR)
            {
                type = E_OR;
                num = 2;
            }
            else if (t->type == T_AND)
            {
                type = E_AND;
                num = 2;
            }
            else if (t->type == T_EQUAL)
            {
                type = E_EQUAL;
                num = 2;
            }
            else if (t->type == T_LESS)
            {
                type = E_LESS;
                num = 2;
            }
            else if (t->type == T_LESS_EQUAL)
            {
                type = E_LESS_EQUAL;
                num = 2;
            }
            else if (t->type == T_GREATER_EQUAL)
            {
                type = E_GREATER_EQUAL;
                num = 2;
            }
            else if (t->type == T_GREATER)
            {
                type = E_GREATER;
                num = 2;
            }
            else if (t->type == T_BEGIN)
            {
                type = E_BEGIN;
                num = 2;
            }
            else
            {
                type = E_CALL;
                num = 2;

                i--;
            }

            expression *exp = push_expression(s, type, num, t->line);
        }
        else if (t->type == T_NUMERIC || t->type == T_STRING ||
                 t->type == T_TRUE || t->type == T_FALSE)
        {
            expression *prev_exp = top(s);
            expression *const_exp = create_expression(E_CONST, 0);

            switch (t->type)
            {
            case T_NUMERIC:
                const_exp->value.type = V_INT;
                const_exp->value.val.i = t->value.i;
                break;
            case T_STRING:
                const_exp->value.type = V_STRING;
                const_exp->value.val.str = t->value.str;
                break;
            case T_TRUE:
                const_exp->value.type = V_BOOL;
                const_exp->value.val.boolean = t->value.boolean;
                break;
            case T_FALSE:
                const_exp->value.type = V_BOOL;
                const_exp->value.val.boolean = t->value.boolean;
                break;
            default:
                break;
            }

            if(prev_exp == NULL){
                add_list(module->exps, const_exp);
            }else{
                add_list(prev_exp->exps, const_exp);
            }
        }
        else if (t->type == T_IDENTIFIER)
        {
            expression *prev_exp = top(s);
            expression *const_exp =
                create_expression(E_IDENTIFIER, 0);

            const_exp->value.type = V_IDENTIFIER;
            const_exp->value.val.str = t->value.str;
            const_exp->line = t->line;
            if(prev_exp == NULL){
                add_list(module->exps, const_exp);
            }
            else{
                add_list(prev_exp->exps, const_exp);
            }
        }
        else if (t->type == T_CLOSE_PARAN)
        {
            if (s->expressions->size == 0)
                error(program, "Unmatched paranthesis", t->line);

            expression *prev_exp = pop(s);

            if (s->expressions->size == 0)
            {
                add_list(module->exps, prev_exp);
                continue;
            }
            expression *next_prev = top(s);

            add_list(next_prev->exps, prev_exp);
        }
    }
    if (s->expressions->size != 0)
        error(program, "Unmatched paranthesis", t->line);
    destroy_stack(s);

    return module;
};