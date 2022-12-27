#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "scheme.h"

char *program = "(let f (function x (print (+ (+ \"hello, \" x) \"!\"))) (f \"eren\"))";

bool is_string_char(char c){
    return isascii(c) && c != '"';
}

expression_value eval(expression *exp, enviroment env)
{
    expression_value res;

    switch (exp->type)
    {
    case E_CONST:
        res.val = exp->value.val;
        res.type = exp->value.type;
        break;
    case E_IDENTIFIER:
        for (int i = env.i - 1; i >= 0; i--)
        {
            if (strcmp(exp->value.val.str, env.ids[i]) == 0)
            {
                res = env.vals[i];
                break;
            }
        }
        break;
    case E_ADDITION:
    {
        expression_value val1 = eval(exp->exps[0], env);
        if (val1.type == V_INT)
        {
            res.val.i = val1.val.i + eval(exp->exps[1], env).val.i;
            res.type = V_INT;
        }
        else if (val1.type == V_STRING)
        {
            expression_value val2 = eval(exp->exps[1], env);

            char *concat_str = malloc((strlen(val1.val.str) + strlen(val2.val.str) + 1) * sizeof(char));
            concat_str[0] = '\0';
            strcat(concat_str, val1.val.str);
            strcat(concat_str, val2.val.str);

            res.val.str = concat_str;
            res.type = V_STRING;
        }
        break;
    }

    case E_SUBTRACTION:
        res.val.i = eval(exp->exps[0], env).val.i - eval(exp->exps[1], env).val.i;
        res.type = V_INT;
        break;

    case E_MULTIPLICATION:
        res.val.i = eval(exp->exps[0], env).val.i * eval(exp->exps[1], env).val.i;
        res.type = V_INT;
        break;

    case E_DIVISON:
    {
        int divisor = eval(exp->exps[1], env).val.i;

        if (divisor == 0)
        {
            printe("You are trying to divide by 0!\n");
            res.type = V_NULL;
            break;
        }

        res.val.i = eval(exp->exps[0], env).val.i / divisor;
        res.type = V_INT;
        break;
    }
    case E_PRINT:
    {
        expression_value value = eval(exp->exps[0], env);

        if (value.type == V_INT)
        {
            printf("%d\n", value.val.i);
        }
        else if (value.type == V_STRING)
        {
            printf("%s\n", value.val.str);
        }

        res.type = V_NULL;
        break;
    }
    case E_LET:
    {
        char *id = (exp->exps[0])->value.val.str;
        expression_value val = eval(exp->exps[1], env);

        char *cpy = malloc(sizeof(char) * (strlen(id) + 1));

        strcpy(cpy, id);
        env.ids[env.i] = cpy;

        env.vals[env.i] = val;
        env.i++;

        expression_value in = eval(exp->exps[2], env);
        res = in;
        break;
    }
    case E_FUNCTION:
    {
        char *id = (exp->exps[0])->value.val.str;

        char *cpy = malloc(sizeof(char) * (strlen(id) + 1));
        strcpy(cpy, id);
        function *f = malloc(sizeof(function));

        f->id = cpy;
        f->body = exp->exps[1];

        res.val.func = f;
        res.type = V_FUNCTION;
        break;
    }
    case E_CALL:
    {
        function *f;
        for (int i = env.i - 1; i >= 0; i--)
        {
            if (strcmp(exp->exps[0]->value.val.str, env.ids[i]) == 0)
            {
                f = env.vals[i].val.func;
                break;
            }
        }
        enviroment f_env = env;
        f_env.ids[f_env.i] = f->id;
        f_env.vals[f_env.i] = eval(exp->exps[1], env);
        ;
        f_env.i++;

        res = eval(f->body, f_env);
        break;
    }
    default:
        res.type = V_NULL;
        break;
    }

    return res;
}

int main()
{
    int n = strlen(program);

    token *tokens[100];
    int last_token_index = 0;

    for (int i = 0; i < n; i++)
    {
        token *curr;

        if (program[i] == '(')
            curr = create_token(T_OPEN_PARAN);
        else if (program[i] == ')')
            curr = create_token(T_CLOSE_PARAN);
        else if (program[i] == '+')
            curr = create_token(T_PLUS);
        else if (program[i] == '-')
            curr = create_token(T_MINUS);
        else if (program[i] == '*')
            curr = create_token(T_STAR);
        else if (program[i] == '/')
            curr = create_token(T_SLASH);
        else if (program[i] == ' ')
            continue;
        else if (isdigit(program[i]))
        {
            int start = i;
            int end = i;
            while (isdigit(program[++i]))
                end = i;

            i--;
            curr = create_token(T_NUMERIC);
            curr->value.i = str2int(&program[start], end - start + 1);
        }
        else if (isalpha(program[i]))
        {
            int start = i;
            int end = i;
            while (isalpha(program[++i])){
                end = i;
            }
            i--;
            char word[end - start + 2];

            strncpy(word, &program[start], end - start + 1);
            word[end - start + 1] = '\0';
            if (strcmp(word, "print") == 0)
            {
                curr = create_token(T_PRINT);
            }
            else if (strcmp(word, "let") == 0)
            {
                curr = create_token(T_LET);
            }
            else if (strcmp(word, "function") == 0)
            {
                curr = create_token(T_FUNCTION);
            }
            else
            {
                curr = create_token(T_IDENTIFIER);
                char *str = malloc(sizeof(char) * end - start);
                strncpy(str, &program[start], end - start + 1);
                curr->value.str = str;
            }
        }
        else if (program[i] == '"')
        {
            i++;
            int start = i;
            int end = i;
            while (is_string_char(program[i])){
                end = i;
                i++;
            }
            if(program[i] != '"'){
                printe("Strings must end with `\"`!");
                continue;
            }
            
            char word[end - start + 2];

            strncpy(word, &program[start], end - start + 1);
            word[end - start + 1] = '\0';
            
            curr = create_token(T_STRING);
            char *str = malloc(sizeof(char) * end - start);
            strncpy(str, &program[start], end - start + 1);
            curr->value.str = str;
        }

        tokens[last_token_index++] = curr;
    }

    stack s;
    expression *program;

    s.i = 0;

    for (int i = 0; i < last_token_index; i++)
    {
        token *t = tokens[i];

        if (t->type == T_OPEN_PARAN)
        {
            token *t = tokens[++i];
            enum expression_type type;
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
            else
            {
                type = E_CALL;
                num = 2;

                i--;
            }

            expression *exp = create_expression(type, num);
            push(&s, exp);
        }
        else if (t->type == T_NUMERIC || t->type == T_STRING)
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

    enviroment env;
    env.i = 0;
    expression_value value = eval(program, env);

    printf("result of the program is :");
    if (value.type == V_INT)
        printf("%d", value.val.i);
    else if (value.type == V_STRING)
        printf("%s", value.val.str);
    else if (value.type == V_NULL)
        printf("null");
}