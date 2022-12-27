#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "scanner.h"
#include "crisp.h"

token *create_token(token_type type)
{
    token *t = malloc(sizeof(token));
    t->type = type;

    return t;
}

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

bool is_string_char(char c){
    return isascii(c) && c != '"';
}

tokens* scan(char* program, int n){
    tokens* program_tokens = malloc(sizeof(tokens));

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

        program_tokens->tokens[last_token_index++] = curr;
    }
    program_tokens->n = last_token_index;

    return program_tokens;
}