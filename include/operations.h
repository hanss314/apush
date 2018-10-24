#include "expression.h"
#include "interpreter.h"
#define NUM_OPS 21

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
AObject apush_bool(AObject*, int);
AObject apush_bnot(AObject*, int);
AObject apush_band(AObject*, int);
AObject apush_bor(AObject*, int);
AObject apush_bxor(AObject*, int);

const char* op_names[NUM_OPS] = {
    //0 Basic operations
    "shell",
    "define",
    "$",
    "nop",
    //4 Flow control
    "if",
    "while",
    //6 Arith
    "+",
    "-",
    "*",
    "/",
    "%",
    //11 Comparison
    "=",
    "<",
    ">",
    //14 SKI
    "id",
    "sub",
    //16 Boolean
    "bool",
    "!",
    "&&",
    "||",
    "^",
    //21 END
};

AObject (*op_funcs[NUM_OPS]) (AObject*, int) = {
    //0 Basic ops
    &apush_code,
    &apush_def,
    &apush_ref,
    &apush_nop,
    //4 Control
    &apush_if,
    &apush_while,
    //6 Arith
    &apush_add,
    &apush_diff,
    &apush_mul,
    &apush_div,
    &apush_mod,
    //11 Comp
    &apush_eq,
    &apush_lt,
    &apush_gt,
    //14 SKI
    &apush_id,
    &apush_sub,
    //16 Boolean
    &apush_bool,
    &apush_bnot,
    &apush_band,
    &apush_bor,
    &apush_bxor,
    //21 END

};

#endif

