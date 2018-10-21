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

AObject apush_mul(AObject* args, int len){
    run_exprs(args, 0, len);
    int acc = 1;
    char* endptr;
    for (int i=0; i<len; i++) {
        acc *= strtoimax(args[i].value, &endptr, 0);
    }

    int clen = (int)(ceil(log10((double)abs(acc)))) + 3;
    if (acc == 0) clen = 2;
    char str[clen];
    sprintf(str, "%d", acc);
    return createValue(str);
}

AObject apush_div(AObject* args, int len){
    run_exprs(args, 0, len);
    char* endptr;
    int acc = strtoimax(args[0].value, &endptr, 0);
    for (int i=1; i<len; i++) {
        acc /= strtoimax(args[i].value, &endptr, 0);
    }

    int clen = (int)(ceil(log10((double)abs(acc)))) + 3;
    if (acc == 0) clen = 2;
    char str[clen];
    sprintf(str, "%d", acc);
    return createValue(str);
}

AObject apush_mod(AObject* args, int len){
    run_exprs(args, 0, len);
    char* endptr;
    int acc = strtoimax(args[0].value, &endptr, 0);
    for (int i=1; i<len; i++) {
        acc %= strtoimax(args[i].value, &endptr, 0);
    }

    int clen = (int)(ceil(log10((double)abs(acc)))) + 3;
    if (acc == 0) clen = 2;
    char str[clen];
    sprintf(str, "%d", acc);
    return createValue(str);
}

AObject apush_eq(AObject* args, int len){
    run_exprs(args, 0, 2);
    if (strcmp(args[0].value, args[1].value) == 0){
        return createValue("1");
    } else {
        return createValue("0");
    }
}

AObject apush_gt(AObject* args, int len){
    run_exprs(args, 0, 2);
    if (strcmp(args[0].value, args[1].value) > 0){
        return createValue("1");
    } else {
        return createValue("0");
    }
}

AObject apush_lt(AObject* args, int len){
    run_exprs(args, 0, 2);
    if (strcmp(args[0].value, args[1].value) < 0){
        return createValue("1");
    } else {
        return createValue("0");
    }
}

AObject apush_id(AObject* args, int len){
    if (args[0].is_expr){
        return args[0];
    } else {
        return createValue(args[0].value);
    }
}
AObject apush_sub(AObject* args, int len){
    AObject x = dupObj(args[0]), y = dupObj(args[1]),
            z1 = dupObj(args[2]), z2 = dupObj(args[2]);
    AObject* l = malloc(2*sizeof(AObject)); l[0] = x; l[1] = z1;
    AObject* r = malloc(2*sizeof(AObject)); l[0] = y; l[1] = z2;
    AObject left = createExp(2, l), right = createExp(2, r);
    AObject* nodes = malloc(2 * sizeof(AObject)); nodes[0] = left; nodes[1] = right;
    return createExp(2, nodes);
}