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

void printe(char *msg);
#endif