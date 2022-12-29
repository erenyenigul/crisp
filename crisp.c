#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>
#include "crisp.h"
#include "scanner.h"
#include "parser.h"

bool expect(expression *exp, ...)
{
    va_list valist;

    va_start(valist, exp->filled);

    for (int i = 0; i < exp->filled; i++)
    {
        if(va_arg(valist, expression_value_type) != exp->exps[i]->type && va_arg(valist, expression_value_type) != V_ANY)
            return false;
        puts("lol");
    }
    va_end(valist);

    return true;
}

void printe(char *msg)
{
    printf("\033[1m\033[31m[Crisp.Exception]\033[0m %s\n", msg);
}

char *read_file_as_string(FILE *f)
{
    char *buffer = 0;
    long length;

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }

    return buffer;
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
        //if(!expect(exp, V_INT, V_INT) && !expect(exp, V_STRING, V_STRING))
        //    printe("Expected same types for + operation.");

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
        //if(!expect(exp, V_INT, V_INT) || !expect(exp, V_STRING, V_STRING))
        //    printe("Expected integers for - operation.");

        res.val.i = eval(exp->exps[0], env).val.i - eval(exp->exps[1], env).val.i;
        res.type = V_INT;
        break;

    case E_MULTIPLICATION:
        //if(!expect(exp, V_INT, V_INT))
        //    printe("Expected integers for * operation.");

        res.val.i = eval(exp->exps[0], env).val.i * eval(exp->exps[1], env).val.i;
        res.type = V_INT;
        break;

    case E_DIVISON:
    {
        //if(!expect(exp, V_INT, V_INT))
        //    printe("Expected integers for / operation.");
        
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
       // if(!expect(exp, V_IDENTIFIER, V_ANY, V_ANY))
       //     printe("Expected an identifier in let expression.");
        
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
    case E_READ:
    {
        if (exp->filled == 0)
        {
            char *read_buffer = malloc(sizeof(char) * 1000);
            fgets(read_buffer, sizeof(char) * 1000, stdin);

            res.type = V_STRING;
            res.val.str = read_buffer;
        }
        else if (exp->filled == 1)
        {
            FILE *f = eval(exp->exps[0], env).val.file;
            char *file_content = read_file_as_string(f);

            res.type = V_STRING;
            res.val.str = file_content;
        }
        break;
    }
    case E_FOR:
    {
        char *id = exp->exps[0]->value.val.str;
        int n = eval(exp->exps[1], env).val.i;

        for (int i = 0; i < n; i++)
        {
            expression_value val;
            val.type = V_INT;
            val.val.i = i;

            env.ids[env.i] = id;
            env.vals[env.i] = val;
            env.i++;
            res = eval(exp->exps[2], env);
        }

        break;
    }
    case E_OPEN:
    {
        char *file_name = eval(exp->exps[0], env).val.str;
        char *mode = eval(exp->exps[1], env).val.str;

        FILE *f = fopen(file_name, mode);

        res.type = V_FILE;
        res.val.file = f;
        break;
    }
    case E_WRITE:
    {
        FILE *f = eval(exp->exps[0], env).val.file;
        char *content = eval(exp->exps[1], env).val.str;

        fprintf(f, "%s", content);
        res.type = V_NULL;

        break;
    }
    case E_IF:
    {
        bool boolean = eval(exp->exps[0], env).val.boolean;

        if(boolean){
            res = eval(exp->exps[1], env);
        }else{
            res.type = V_NULL;
        }
        break;
    }
    case E_CLOSE:
    {
        FILE *f = eval(exp->exps[0], env).val.file;
        fclose(f);

        res.type = V_NULL;
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

    while (program_buffer[0] != '\0')
    {
        int n = strlen(program_buffer);

        tokens *program_tokens = scan(program_buffer, n);
        expression *program = parse(program_tokens);

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