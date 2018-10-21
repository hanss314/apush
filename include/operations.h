#include "expression.h"
#include "interpreter.h"
#define NUM_OPS 16

#ifndef APUSH_OPS_H
#define APUSH_OPS_H

AObject apush_code(AObject*, int);
AObject apush_def(AObject*, int);
AObject apush_ref(AObject*, int);
AObject apush_if(AObject*, int);
AObject apush_nop(AObject*, int);
AObject apush_add(AObject*, int);
AObject apush_mul(AObject*, int);
AObject apush_while(AObject*, int);
AObject apush_div(AObject*, int);
AObject apush_mod(AObject*, int);
AObject apush_eq(AObject*, int);
AObject apush_gt(AObject*, int);
AObject apush_lt(AObject*, int);
AObject apush_id(AObject*, int);
AObject apush_sub(AObject*, int);
const char* op_names[NUM_OPS] = {
    "shell",
    "define",

    "$",
    "if",
    "nop",
    "+",
    "while",

    "*",
    "/",
    "%"

    "<",
    ">",
    "=",

    "id",
    "nop",
    "nop",
};

AObject (*op_funcs[NUM_OPS]) (AObject*, int) = {
        &apush_code,
        &apush_def,

        &apush_ref,
        &apush_if,
        &apush_nop,
        &apush_add,
        &apush_while,

        &apush_mul,
        &apush_div,
        &apush_mod,

        &apush_gt,
        &apush_lt,
        &apush_eq,
        &apush_id,
        &apush_sub,
        &apush_nop,
};

#endif

