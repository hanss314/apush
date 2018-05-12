#include "stack.h"
#include "operations.h"

char* op_str[] = {
    "+",
    "-"
};

void (*op_func[]) (struct StackNode **) = {
    &apush_add,
    &apush_sub
};

int apush_num_ops() {
    return sizeof(op_str) / sizeof(char *);
}

void apush_add(struct StackNode** stack){
    push(stack, pop(stack) + pop(stack));
}

void apush_sub(struct StackNode** stack){
    int a = pop(stack);
    push(stack, pop(stack) - a);
}
