#if defined(linux) || defined(__APPLE__)
# include <unistd.h>
#elif defined(_WIN32)
# include <windows.h>   
#endif
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crisp.h"
#include "list.h"
#include "scanner.h"
#include "parser.h"

#define LANG_VERSION "alpha 1.0.1"

/*
bool expect(expression *exp, ...)
{
    va_list valist;

    va_start(valist, exp->filled);

    for (int i = 0; i < exp->filled; i++)
    {
    if(va_arg(valist, expression_value_type) != exp->exps[i]->type &&
va_arg(valist, expression_value_type) != V_ANY) return false; puts("lol");
    }
    va_end(valist);

    return true;
}*/
void print_expression_value(expression_value value, bool print_null)
{
    switch (value.type)
    {
    case V_INT:
    {
        printf("%d", value.val.i);
        break;
    }
    case V_STRING:
    {
        printf("%s", value.val.str);
        break;
    }
    case V_NULL:
    {
        if(print_null) printf("null");
        break;
    }
    case V_BOOL:
    {
        if (value.val.boolean)
            printf("true");
        else
            printf("false");
        break;
    }
    case V_FILE:
    {
        printf("<file %d>", value.val.file->_file);
        break;
    }
    case V_FUNCTION:
    {
        printf("<function>");
        break;
    }
    }
}

char* getcurrentdir(){
    char* buffer = malloc(1024);
    #if defined(linux) || defined(__APPLE__)
    getcwd(buffer, 1024);
    #elif defined(_WIN32)
    GetCurrentDirectory(1024, buffer);
    #endif
    return buffer;
}

void printloc(char *program, int line)
{
    int i = 0;
    int current_line = 1;

    while (current_line <= line)
    {
        if (line - current_line < 3)
            printf("\n\033[1m%3d\033[0m| ", current_line);
        while (program[i] != '\0' && program[i] != '\n')
        {
            if (line - current_line < 3)
            {
                printf("%c", program[i]);
            }
            i++;
        }

        i++;
        current_line++;
    }
    printf(" \033[0;33m\033[1m<--\033[0m\n");
}

void printe(char *msg, int line)
{
    printf("\033[31;1m[Crisp Exception]\033[0m At line "
           "\033[31m\033[1m%d\033[0m : %s\n\n",
           line, msg);
}

void error(char *program, char *msg, int line)
{
    printe(msg, line);
    printloc(program, line);

    exit(1);
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

expression_value eval(expression *exp, environment env, environment *global, char *program)
{
    expression_value res;
    res.type = V_NULL;

    switch (exp->type)
    {
    case E_MODULE:
    {
        for (int i = 0; i < exp->exps->size; i++)
        {
            res = eval(get_list(exp->exps, i), env, global, program);
        }
        break;
    }
    case E_BEGIN:
    {
        for (int i = 0; i < exp->exps->size; i++)
        {
            res = eval(get_list(exp->exps, i), env, global, program);
        }
        break;
    }
    case E_IMPORT:
    {
        char* cwd = getcurrentdir();
        
        for(int i=0; i<exp->exps->size; i++){
            char *module_path = ((expression *)get_list(exp->exps, i))->value.val.str;
            strcat(cwd, "/");
            strcat(cwd, module_path);
        }
        strcat(cwd, ".cr");
                
        printf("%s\n", cwd);
        
        FILE *f = fopen(cwd, "r");
        if(f == NULL) error(program, "Module not found", exp->line);
    
        char *program_buffer = read_file_as_string(f);
        fclose(f);

        run_module(program_buffer, global);

        free(cwd);
        break;
    }
    case E_CONST:
        res.val = exp->value.val;
        res.type = exp->value.type;
        break;
    case E_AND:
    {
        bool val = eval(get_list(exp->exps, 0), env, global, program).val.boolean;
        for (int i = 1; i < exp->exps->size; i++)
        {
            val = val && eval(get_list(exp->exps, i), env, global, program).val.boolean;
        }

        res.val.boolean = val;
        res.type = V_BOOL;
        break;
    }
    case E_OR:
    {
        bool val = eval(get_list(exp->exps, 0), env, global, program).val.boolean;
        for (int i = 1; i < exp->exps->size; i++)
        {
            val = val || eval(get_list(exp->exps, i), env, global, program).val.boolean;
        }

        res.val.boolean = val;
        res.type = V_BOOL;
        break;
    }
    case E_LESS:
    {
        int val1 = eval(get_list(exp->exps, 0), env, global, program).val.i;
        int val2 = eval(get_list(exp->exps, 1), env, global, program).val.i;

        res.val.boolean = val1 < val2;
        res.type = V_BOOL;
        break;
    }
    case E_LESS_EQUAL:
    {
        int val1 = eval(get_list(exp->exps, 0), env, global, program).val.i;
        int val2 = eval(get_list(exp->exps, 1), env, global, program).val.i;

        res.val.boolean = val1 <= val2;
        res.type = V_BOOL;
        break;
    }
    case E_GREATER:
    {
        int val1 = eval(get_list(exp->exps, 0), env, global, program).val.i;
        int val2 = eval(get_list(exp->exps, 1), env, global, program).val.i;

        res.val.boolean = val1 > val2;
        res.type = V_BOOL;
        break;
    }
    case E_GREATER_EQUAL:
    {
        int val1 = eval(get_list(exp->exps, 0), env, global, program).val.i;
        int val2 = eval(get_list(exp->exps, 1), env, global, program).val.i;

        res.val.boolean = val1 >= val2;
        res.type = V_BOOL;
        break;
    }
    case E_IDENTIFIER:
    {
        bool flag = false;
        for (int i = env.i - 1; i >= 0; i--)
        {
            if (strcmp(exp->value.val.str, env.ids[i]) == 0)
            {
                res = env.vals[i];
                flag = true;
                break;
            }
        }
        if (flag)
            break;
        for (int i = global->i - 1; i >= 0; i--)
        {
            if (strcmp(exp->value.val.str, global->ids[i]) == 0)
            {
                res = global->vals[i];
                flag = true;
                break;
            }
        }
        if (flag)
            break;

        printf("%s\n", exp->value.val.str);
        error(program, "Undefined identifier.", exp->line);
        break;
    }
    case E_ADDITION:
    {
        expression_value val1 =
            eval(get_list(exp->exps, 0), env, global, program);
        if (val1.type == V_INT)
        {
            int sum = val1.val.i;
            for (int j = 1; j < exp->exps->size; j++)
            {
                sum += eval(get_list(exp->exps, j), env, global, program).val.i;
            }

            res.val.i = sum;
            res.type = V_INT;
        }
        else if (val1.type == V_STRING)
        {
            int len = strlen(val1.val.str);
            char *concat_str = malloc((len + 1) * sizeof(char));

            strcpy(concat_str, val1.val.str);
            for (int j = 1; j < exp->exps->size; j++)
            {
                expression_value val2 =
                    eval(get_list(exp->exps, j), env, global, program);

                len += strlen(val2.val.str);
                concat_str = realloc(concat_str, (len + 1) * sizeof(char));

                strcat(concat_str, val2.val.str);
            }

            res.val.str = concat_str;
            res.type = V_STRING;
        }
        break;
    }

    case E_SUBTRACTION:
    {
        expression_value val1 = eval(get_list(exp->exps, 0), env, global, program);

        int substraction = val1.val.i;
        for (int j = 1; j < exp->exps->size; j++)
        {
            substraction -= eval(get_list(exp->exps, j), env, global, program).val.i;
        }

        res.val.i = substraction;
        res.type = V_INT;
        break;
    }

    case E_MULTIPLICATION:
    {
        expression_value val1 = eval(get_list(exp->exps, 0), env, global, program);

        int mult = val1.val.i;
        for (int j = 1; j < exp->exps->size; j++)
        {
            mult *= eval(get_list(exp->exps, j), env, global, program).val.i;
        }

        res.val.i = mult;
        res.type = V_INT;
        break;
    }

    case E_DIVISON:
    {
        // if(!expect(exp, V_INT, V_INT))
        //     printe("Expected integers for /
        //     operation.");

        int divisor = eval(get_list(exp->exps, 1), env, global, program).val.i;

        if (divisor == 0)
        {
            error(program,
                  "You are trying to "
                  "divide by 0!",
                  exp->line);
            printloc(program, exp->line);

            res.type = V_NULL;
            break;
        }

        res.val.i =
            eval(get_list(exp->exps, 0), env, global, program).val.i / divisor;
        res.type = V_INT;
        break;
    }
    case E_PRINT:
    {
        for (int j = 0; j < exp->exps->size; j++)
        {
            expression_value value =
                eval(get_list(exp->exps, j), env, global, program);

            print_expression_value(value, true);
        }
        printf("\n");

        res.type = V_NULL;
        break;
    }
    case E_LET:
    {
        expression *id_exp = (expression *)get_list(exp->exps, 0);
        expression* eval_exp = (expression*) get_list(exp->exps, 1);
        expression* in_exp = (expression*) get_list(exp->exps, 2);

        // Expected fields
        if (id_exp == NULL || id_exp->type != E_IDENTIFIER) error(program, "Expected identifier.", exp->line);   
        if(eval_exp == NULL) error(program, "Expected expression.", exp->line);
        if(in_exp == NULL) error(program, "Expected an expression where your identifier can be used.", exp->line);
        // End of expected fiels

        char *id = (id_exp)->value.val.str;
        
        expression_value val = eval(eval_exp, env, global, program);

        char *cpy = malloc(sizeof(char) * (strlen(id) + 1));

        strcpy(cpy, id);
        env.ids[env.i] = cpy;

        env.vals[env.i] = val;
        env.i++;

        expression_value in =
            eval(in_exp, env, global, program);
        res = in;
        break;
    }
    case E_DEFINE:
    {
        expression *id_exp = (expression *)get_list(exp->exps, 0);
        expression* eval_exp = (expression*) get_list(exp->exps, 1);
        
        // Expected fields
        if (id_exp == NULL || id_exp->type != E_IDENTIFIER) error(program, "Expected identifier.", exp->line);   
        if(eval_exp == NULL) error(program, "Expected expression.", exp->line);
        // End of expected fiels
        
        char *id = (id_exp)->value.val.str;
        expression_value val = eval(eval_exp, env, global, program);

        char *cpy = malloc(sizeof(char) * (strlen(id) + 1));

        strcpy(cpy, id);
        global->ids[global->i] = cpy;

        global->vals[global->i] = val;
        global->i++;

        res.type = V_NULL;
        break;
    }
    case E_FUNCTION:
    {
        function *f = malloc(sizeof(function));
        f->ids = create_list(1, sizeof(char *));

        for (int i = 0; i < exp->exps->size - 1; i++)
        {
            char *id = ((expression *)get_list(exp->exps, i))->value.val.str;
            char *cpy = malloc(sizeof(char) * (strlen(id) + 1));
            strcpy(cpy, id);

            add_list(f->ids, &cpy);
        }

        f->body = (expression *) get_list(exp->exps, exp->exps->size - 1);        
        f->env = env;

        res.val.func = f;
        res.type = V_FUNCTION;
        break;
    }
    case E_CALL:
    {
        function *f = eval(get_list(exp->exps, 0), env, global, program).val.func;

        if (f->ids->size != exp->exps->size - 1)
            error(program, "Wrong number of arguments.", exp->line);

        environment f_env = f->env;
        
        for (int i = 1; i < exp->exps->size; i++)
        {
            f_env.ids[f_env.i] = *((char **)get_list(f->ids, i - 1));
            f_env.vals[f_env.i] = eval(get_list(exp->exps, i), env, global, program);
            f_env.i++;
        }

        res = eval(f->body, f_env, global, program);
        break;
    }
    case E_READ:
    {
        if (exp->exps->size == 0)
        {
            char *read_buffer = malloc(sizeof(char) * 1000);
            fgets(read_buffer, sizeof(char) * 1000, stdin);

            res.type = V_STRING;
            res.val.str = read_buffer;
        }
        else if (exp->exps->size == 1)
        {
            FILE *f =
                eval(get_list(exp->exps, 0), env, global, program).val.file;
            char *file_content = read_file_as_string(f);

            res.type = V_STRING;
            res.val.str = file_content;
        }
        break;
    }
    case E_FOR:
    {
        char *id =
            ((expression *)get_list(exp->exps, 0))->value.val.str;
        int n = eval(get_list(exp->exps, 1), env, global, program).val.i;

        for (int i = 0; i < n; i++)
        {
            expression_value val;
            val.type = V_INT;
            val.val.i = i;

            env.ids[env.i] = id;
            env.vals[env.i] = val;
            env.i++;
            res = eval(get_list(exp->exps, 2), env, global, program);
        }

        break;
    }
    case E_OPEN:
    {
        char *file_name =
            eval(get_list(exp->exps, 0), env, global, program).val.str;
        char *mode = eval(get_list(exp->exps, 1), env, global, program).val.str;

        FILE *f = fopen(file_name, mode);

        res.type = V_FILE;
        res.val.file = f;
        break;
    }
    case E_WRITE:
    {
        FILE *f = eval(get_list(exp->exps, 0), env, global, program).val.file;
        char *content =
            eval(get_list(exp->exps, 1), env, global, program).val.str;

        fprintf(f, "%s", content);
        res.type = V_NULL;

        break;
    }
    case E_IF:
    {
        bool boolean = eval(get_list(exp->exps, 0), env, global, program).val.boolean;

        if (boolean)
        {
            res = eval(get_list(exp->exps, 1), env, global, program);
        }
        else
        {
            if (exp->exps->size == 3)
            {
                res = eval(get_list(exp->exps, 2), env, global, program);
            }
            else
            {
                res.type = V_NULL;
            }
        }
        break;
    }
    case E_CLOSE:
    {
        FILE *f = eval(get_list(exp->exps, 0), env, global, program).val.file;
        fclose(f);

        res.type = V_NULL;
        break;
    }
    case E_NEGATION:
    {
        bool boolean =
            eval(get_list(exp->exps, 0), env, global, program).val.boolean;

        res.val.boolean = !boolean;
        res.type = V_BOOL;
        
        break;
    }
    case E_EQUAL:
    {
        expression_value val1 =
            eval(get_list(exp->exps, 0), env, global, program);
        expression_value val2 =
            eval(get_list(exp->exps, 1), env, global, program);

        if (val1.type != val2.type)
        {
            res.val.boolean = false;
        }

        if (val1.type == V_INT)
            res.val.boolean = val1.val.i == val2.val.i;
        else if (val1.type == V_STRING)
            res.val.boolean =
                strcmp(val1.val.str, val2.val.str) == 0;
        else if (val1.type == V_BOOL)
            res.val.boolean = val1.val.boolean == val2.val.boolean;
        else if (val1.type == V_NULL)
            res.val.boolean = true;

        res.type = V_BOOL;
        break;
    }
    default:
        res.type = V_NULL;
        break;
    }

    return res;
}

expression_value run_module(char *program_buffer, environment *global)
{
    tokens *program_tokens = scan(program_buffer, strlen(program_buffer));
    expression *program = parse(program_tokens, program_buffer);

    environment env;
    env.i = 0;

    expression_value value = eval(program, env, global, program_buffer);

    return value;
}

expression_value run_with_global_env(char *program_buffer, int n, environment *global)
{
    tokens *program_tokens = scan(program_buffer, n);
    expression *program = parse(program_tokens, program_buffer);

    environment env;
    env.i = 0;

    expression_value value = eval(program, env, global, program_buffer);

    return value;
}

expression_value run(char *program_buffer, int n)
{
    environment global;
    global.i = 0;
  
    return run_with_global_env(program_buffer, n, &global);
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("\033[1;41;37m C \033[1;42m R \033[1;43m I \033[1;44m S \033[1;45m P \033[0m| %s\n\n", LANG_VERSION);

        char program_buffer[1000];
        program_buffer[0] = '\0';

        printf(">  ");
        fgets(program_buffer, sizeof(program_buffer), stdin);

        environment global;
        global.i = 0;
    
        while (program_buffer[0] != '\0')
        {
            int n = strlen(program_buffer);
            expression_value value = run_with_global_env(program_buffer, n, &global);

            print_expression_value(value, false);

            printf("\n>  ");
            fgets(program_buffer, sizeof(program_buffer), stdin);
        }
    }
    else if (argc == 2)
    {
        FILE *f = fopen(argv[1], "r");
        char *program_buffer = read_file_as_string(f);

        run(program_buffer, strlen(program_buffer));
        fclose(f);
    }
    else
    {
        printf("Usage: crisp [file]\n");
    }
}