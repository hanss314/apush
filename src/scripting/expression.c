#include "expression.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static SExpression *allocateExpression()
{
    SExpression *b = (SExpression *)malloc(sizeof(SExpression));

    if (b == NULL) return NULL;

    b->length = 0;
    b->nodes = NULL;

    return b;
}

AObject createValue(char* value) {
    AObject a = {false, NULL, strdup(value)};
    return a;
}

static AObject createExpression(SExpression* expr){
    AObject a = {true, expr, NULL};
    return a;
}

AObject createExp(int length, AObject* nodes)
{
    SExpression *b = allocateExpression();
    b->length = length;
    b->nodes = nodes;
    AObject expression = createExpression(b);
    return expression;
}


void deleteExp(SExpression *b)
{
    if (b == NULL)
        return;
    for (int i=0; i<b->length; i++){
        AObject a = b->nodes[i];
        if (a.is_expr){
            deleteExp(a.expr);
        } else {
            free(a.value);
        }
    }
    free(b->nodes);
    free(b);
}

void deleteObjs(AObject* a, int len){
    for (int i=0; i<len; i++){
        if (a[i].is_expr){
            deleteObjs(a[i].expr->nodes, a[i].expr->length);
            free(a[i].expr);
        } else {
            free(a[i].value);
        }
    }
    free(a);
}

