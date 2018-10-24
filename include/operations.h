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
AObject apush_diff(AObject*, int);
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
    //0
    "shell",
    "define",
    "$",
    "nop",
    //4
    "if",
    "while",
    //6
    "+",
    "-",
    "*",
    "/",
    "%",
    //11
    "=",
    "<",
    ">",
    //14
    "id",
    "sub",
    //16
};

AObject (*op_funcs[NUM_OPS]) (AObject*, int) = {
    //0
    &apush_code,
    &apush_def,
    &apush_ref,
    &apush_nop,
    //4
    &apush_if,
    &apush_while,
    //6
    &apush_add,
    &apush_diff,
    &apush_mul,
    &apush_div,
    &apush_mod,
    //11
    &apush_eq,
    &apush_lt,
    &apush_gt,
    //14
    &apush_id,
    &apush_sub,
    //16
};

#endif

