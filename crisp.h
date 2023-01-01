#ifndef CRISP_H
#define CRISP_H
#include "parser.h"
#include <stdio.h>

typedef struct
{
    char *ids[100];
    expression_value vals[100];
    int i;
} environment;

void printe(char *msg, int line);
void printloc(char *program, int line);
void error(char *program, char *msg, int line);
expression_value run_module(char *module_path, environment *global);
#endif