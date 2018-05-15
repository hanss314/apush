#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "interpreter.h"
#include "stack.h"
#include "operations.h"

char* op_str[] = {
    "+",
    "-",
    "rot",
    "p",
    "eval",
};

void (*op_func[]) (struct StackNode **) = {
    &apush_add,
    &apush_sub,
    &apush_rot,
    &apush_print_int,
    &apush_eval,
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

void apush_rot(struct StackNode** stack){
    int rotations = pop(stack);
    int size = get_size(*stack);
    if (size < 2) return;
    rotations %= size;
    struct StackNode* rotated = rotate_stack(*stack, rotations);
    *stack = rotated;
}

void apush_print_int(struct StackNode** stack){
    printf("%d\n", peek(*stack));
}

void apush_eval(struct StackNode** stack){
    char* text = pop_str(stack);
    char** code = apush_split_line(text, 0)[0];
    execute_code(code, *stack);
}
