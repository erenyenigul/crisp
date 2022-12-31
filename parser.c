#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "crisp.h"
#include "parser.h"
#include "scanner.h"

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

expression *create_expression(expression_type type, int num_exp)
{
    expression *exp = (expression*) malloc(sizeof(expression));
    exp->exps =  (expression**) malloc(sizeof(expression *) * num_exp);
    exp->num_exps = num_exp;
    exp->filled = 0;
    exp->type = type;

    return exp;
}

expression* parse(tokens *token_collection){
    stack s;
    expression *program;
    
    s.i = 0;

    for (int i = 0; i < token_collection->n; i++)
    {
        token *t = token_collection->tokens[i];

        if (t->type == T_OPEN_PARAN)
        {
            token *t = token_collection->tokens[++i];
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
            }else if(t->type == T_OPEN){
                type = E_OPEN;
                num = 2;
            }else if(t->type == T_WRITE){
                type = E_WRITE;
                num = 2;
            }
            else if(t->type == T_CLOSE){
                type = E_CLOSE;
                num = 1;
            }
            else if(t->type == T_IF){
                type = E_IF;
                num = 2;
            }
            else if(t->type == T_EXCLAMATION){
                type = E_NEGATION;
                num = 1;
            }else if(t->type == T_EQUAL){
                type = E_EQUAL;
                num = 2;
            }
            else
            {
                type = E_CALL;
                num = 2;

                i--;
            }

            expression *exp = create_expression(type, num);
            exp->line = t->line;
            push(&s, exp);
        }
        else if (t->type == T_NUMERIC || t->type == T_STRING || t->type == T_TRUE || t->type == T_FALSE)
        {
            expression *prev_exp = top(&s);
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
            }

            prev_exp->exps[prev_exp->filled++] = const_exp;
        }
        else if (t->type == T_IDENTIFIER)
        {
            expression *prev_exp = top(&s);
            expression *const_exp = create_expression(E_IDENTIFIER, 0);

            const_exp->value.type = V_IDENTIFIER;
            const_exp->value.val.str = t->value.str;
            
            prev_exp->exps[prev_exp->filled++] = const_exp;
        }
        else if (t->type == T_CLOSE_PARAN)
        {
            expression *prev_exp = pop(&s);

            if (s.i == 0)
            {
                program = prev_exp;
                break;
            }
            expression *next_prev = top(&s);

            next_prev->exps[next_prev->filled++] = prev_exp;
        }
    }
    
    return program;
};