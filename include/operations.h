#include "expression.h"
#include "interpreter.h"
#define NUM_OPS 7

#ifndef APUSH_OPS_H
#define APUSH_OPS_H

AObject apush_code(AObject*, int);
AObject apush_def(AObject*, int);
AObject apush_ref(AObject*, int);
AObject apush_if(AObject*, int);
AObject apush_nop(AObject*, int);
AObject apush_add(AObject*, int);
AObject apush_while(AObject*, int);

const char* op_names[NUM_OPS] = {
    "shell",
    "define",
    "$",
    "if",
    "nop",
    "+",
    "while",
};

AObject (*op_funcs[NUM_OPS]) (AObject*, int) = {
        &apush_code,
        &apush_def,
        &apush_ref,
        &apush_if,
        &apush_nop,
        &apush_add,
        &apush_while,
};

#endif

