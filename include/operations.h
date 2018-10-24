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
    //0
    "shell",
    "define",
    //2
    "$",
    "if",
    "nop",
    "+",
    "while",
    //7
    "*",
    "/",
    "%",
    //10
    "<",
    ">",
    "=",
    //13
    "id",
    "sub",
    "nop",
    //16
};

AObject (*op_funcs[NUM_OPS]) (AObject*, int) = {
    //0
    &apush_code,
    &apush_def,
    //2
    &apush_ref,
    &apush_if,
    &apush_nop,
    &apush_add,
    &apush_while,
    //7
    &apush_mul,
    &apush_div,
    &apush_mod,
    //10
    &apush_lt,
    &apush_gt,
    &apush_eq,
    //13
    &apush_id,
    &apush_sub,
    &apush_nop,
    //16
};

#endif

