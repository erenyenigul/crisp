#ifndef CRISP_H
#define CRISP_H
#include <stdio.h>
#include "parser.h"

void printe(char *msg, int line);
void printloc(char *program, int line);
void error(char *program, char *msg, int line);
expression_value run_module(char *module_path, environment *global);
#endif