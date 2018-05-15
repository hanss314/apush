#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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
    "if",
};

void (*op_func[]) (struct StackNode **) = {
    &apush_add,
    &apush_sub,
    &apush_rot,
    &apush_print_int,
    &apush_eval,
    &apush_cond,
};

int apush_num_ops() {
    return sizeof(op_str) / sizeof(char *);
}
void apush_add(struct StackNode** stack){
    push_int(stack, pop_int(stack) + pop_int(stack));
}

void apush_sub(struct StackNode** stack){
    int a = pop_int(stack);
    push_int(stack, pop_int(stack) - a);
}

void apush_rot(struct StackNode** stack){
    int rotations = pop_int(stack);
    int size = get_size(*stack);
    if (size < 2) return;
    rotations %= size;
    struct StackNode* rotated = rotate_stack(*stack, rotations);
    *stack = rotated;
}

void apush_print_int(struct StackNode** stack){
    printf("%d\n", peek_int(*stack));
}

void apush_eval(struct StackNode** stack){
    char* text = pop_str(stack);
    char** code = apush_split_line(text, 0)[0];
    execute_code(code, *stack);
}

void apush_cond(struct StackNode** stack){
    bool condition;
    if (top_int(*stack)) condition = peek_int(*stack) != 0;
    else condition = peek_str(*stack)[0] != '\0';
    pop(stack);
    if (condition){
       pop(stack); 
    } else {
        union Data data = pop(stack);
        pop(stack);
        push(stack, data);
    }
    //printf("hello %d\n", peek_int(*stack));
}
