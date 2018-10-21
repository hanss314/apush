#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>

#include "heap.h"
#include "expression.h"
#include "interpreter.h"

#define abs(x) x<0?-x:x

int to_bool(char* value){
    int len = strlen(value);
    if (len == 0){
        return 0;
    }
    if (len == 1) {
        switch(value[0]){
            case 'f': case 'F':
            case 'n': case 'N':
            case '0':
                return 0;
        }
    }
    return 1;
}

AObject* run_exprs(AObject* objs, int s, int f){
    for (int i=s; i<f; i++) {
        if (objs[i].is_expr) {
            AObject a = objs[i];
            objs[i] = run_expression(a.expr);
            deleteExp(a.expr);
        }
    }
}

AObject apush_def(AObject* args, int len){
    run_exprs(args, 0, 2);
    heap_insert(args[0].value, args[1].value);
    return createValue(args[1].value);
}

AObject apush_ref(AObject* args, int len){
    char *val = lookup(args[0].value);
    return createValue(val);
}

AObject apush_code(AObject* args, int len){
    run_exprs(args, 0, len);
    return run_code(args, len);
}

AObject apush_nop(AObject* args, int len){
    return createValue("");
}

AObject apush_if(AObject* args, int len){
    AObject ret;
    if (to_bool(args[0].value)){
        ret = args[1];
    } else if (len >= 3) {
        ret = args[2];
    }
    if (ret.is_expr){
        return ret;
    } else {
        return createValue(ret.value);
    }
}

AObject apush_while(AObject* args, int len){
    int cond;
    AObject ret, dup;
    if (!args[0].is_expr){
        cond = to_bool(args[0].value);
    } else {
        dup = dupObj(args[0]);
        ret = run_expression(dup.expr);
        cond = to_bool(ret.value);
        deleteExp(dup.expr);
        free(ret.value);
    }
    while (cond){
        for (int i=1; i<len; i++){
            if (args[i].is_expr) {
                dup = dupObj(args[i]);
                ret = run_expression(dup.expr);
                deleteExp(dup.expr);
                free(ret.value);
            }
        }
        if (args[0].is_expr){
            dup = dupObj(args[0]);
            ret = run_expression(dup.expr);
            cond = to_bool(ret.value);
            deleteExp(dup.expr);
            free(ret.value);
        }
    }
    return createValue("");
}

AObject apush_add(AObject* args, int len) {
    run_exprs(args, 0, len);
    int acc = 0;
    char* endptr;
    for (int i=0; i<len; i++) {
        acc += strtoimax(args[i].value, &endptr, 0);
    }

    int clen = (int)(ceil(log10((double)abs(acc)))) + 3;
    if (acc == 0) clen = 2;
    char str[clen];
    sprintf(str, "%d", acc);
    return createValue(str);

}