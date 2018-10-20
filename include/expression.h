#include <stdbool.h>

#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__


// Operation types
typedef enum tagEOperationType
{
    eCODE, // children is a list of other ASTs
    eASSIGN, // contains two children
    eREF,
    eVALUE, // contains one child
} EOperationType;

struct aobject;

// expressions representation
typedef struct {
    int length;
    struct aobject* nodes;
} SExpression;

typedef struct aobject {
    bool is_expr;
    SExpression* expr;
    char* value;
}AObject;

AObject createValue(char* value);
AObject createExp(int length, AObject* nodes);
void deleteExp(SExpression *b);
void deleteObjs(AObject* objs, int len);

#endif // __EXPRESSION_H__
