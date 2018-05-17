#ifndef HEAP
#define HEAP

#include "stack.h"

struct Heap { /* table entry: */
    struct Heap *next; /* next entry in chain */
    char *name; /* defined name */
    union Data defn; /* replacement text */
    bool is_int;
};
struct Heap* lookup(char*);
struct Heap* heap_insert(char*, union Data, bool);

#endif
