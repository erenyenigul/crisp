#ifndef CRISP_H
#define CRISP_H
#include <stdio.h>
#include "parser.h"

typedef struct
{
    char *ids[100];
    expression_value vals[100];
    int i;
} enviroment;

void printe(char *msg, int line);
void printloc(char *program, int line);
void error(char*msg, char *program, int line);
#endif