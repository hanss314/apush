//
// Created by hanss on 10/20/18.
//

#include "expression.h"

#ifndef APUSH_EXEC_H

AObject* lex_code(char* s, int* nodes_len, int* position, long str_len);
AObject run_code(AObject*, int);
AObject run_expression(SExpression*);
void print(AObject *a, int len);
void run_interpreter(char* filename);
#define APUSH_EXEC_H

#endif //APUSH_EXEC_H
