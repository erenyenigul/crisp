#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "crisp.h"
#include "scanner.h"
#include "parser.h"

//char *program = "(let f (function x (print (+ (+ \"hello, \" x) \"!\"))) (f \"eren\"))";

void printe(char *msg)
{
    printf("\033[1m\033[31m[Crisp.Exception]\033[0m %s\n", msg);
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
        function *f = eval(exp->exps[0], env).val.func;
      
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
    printf("[Crisp Interpeter]\n");
    printf("*| by erenyenigul\n");
    printf("*| v.1.0.0\n");

    char program_buffer[1000];
    program_buffer[0] = '\0';

    printf(">  ");
    fgets(program_buffer, sizeof(program_buffer), stdin);

    while(program_buffer[0] != '\0'){
        int n = strlen(program_buffer);

        tokens* program_tokens = scan(program_buffer, n);
        expression* program = parse(program_tokens);

        enviroment env;
        env.i = 0;
        expression_value value = eval(program, env);

        if (value.type == V_INT)
            printf("%d\n", value.val.i);
        else if (value.type == V_STRING)
            printf("%s\n", value.val.str);

        printf(">  ");
        fgets(program_buffer, sizeof(program_buffer), stdin);
    }   
}