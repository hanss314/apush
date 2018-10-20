#ifndef HEAP
#define HEAP

struct Heap { /* table entry: */
    struct Heap *next; /* next entry in chain */
    char *name; /* defined name */
    char *defn; /* replacement text */
};
char* lookup(char*);
struct Heap* heap_insert(char*, char*);

#endif
