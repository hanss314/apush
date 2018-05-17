#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "parser.h"
#include "interpreter.h"
#include "stack.h"
#include "operations.h"
#include "heap.h"

char* op_str[] = {
    "+",
    "-",
    "*",
    "/",
    "**",
    "%",
    "abs",

    "&",
    "|",
    "^",
    "~",
    "<<",
    ">>",
    
    "==",
    "!=",
    "<=",
    "<",
    ">=",
    ">",

    "chr",
    "++",
    "ps",

    "set",
    "get",

    "rot",
    "drp",
    "dup",
    "p",
    "eval",
    "if",
    "swp",
};

void (*op_func[]) (struct StackNode **) = {
    &apush_add,
    &apush_sub,
    &apush_prod,
    &apush_div,
    &apush_pow,
    &apush_mod,
    &apush_abs,
    
    &apush_and,     
    &apush_or,
    &apush_xor,
    &apush_comp,
    &apush_shiftl,
    &apush_shiftr,
    
    &apush_eq,
    &apush_neq,
    &apush_leq,
    &apush_ltn,
    &apush_geq,
    &apush_gtn,
    
    &apush_itoc,
    &apush_strcat,
    &apush_print_str,

    &apush_set,
    &apush_get,

    &apush_rot,
    &apush_drop,
    &apush_dup,
    &apush_print_int,
    &apush_eval,
    &apush_cond,
    &apush_swap,
};

int apush_num_ops() {
    return sizeof(op_str) / sizeof(char *);
}


//heap
void apush_set(struct StackNode** stack){
    char* name = pop_str(stack);
    bool is_int = top_int(*stack);
    union Data data = pop(stack);
    heap_insert(name, data, is_int);
}

void apush_get(struct StackNode** stack){
    char* name = pop_str(stack);
    struct Heap* item = lookup(name);
    if (item == NULL) return;
    union Data data = item->defn;
    bool is_int = item->is_int;
    push(stack, data, is_int);
}

//string
void apush_itoc(struct StackNode** stack){
    char c[2] = {(char) pop_int(stack), '\0'};
    push_str(stack, c);
}
void apush_strcat(struct StackNode** stack){
    char* end = pop_str(stack);
    char* start = pop_str(stack);
    char* new = malloc(strlen(end) + strlen(start)+1);
    strcpy(new, start);
    strcat(new, end);
    push_str(stack, new);
}
void apush_print_str(struct StackNode** stack){
    printf(pop_str(stack));
}

//logical
void apush_eq(struct StackNode** stack){
    int a = pop_int(stack);
    push_int(stack, pop_int(stack) == a);
}
void apush_neq(struct StackNode** stack){
    int a = pop_int(stack);
    push_int(stack, pop_int(stack) != a);
}
void apush_leq(struct StackNode** stack){
    int a = pop_int(stack);
    push_int(stack, pop_int(stack) <= a);
}
void apush_geq(struct StackNode** stack){
    int a = pop_int(stack);
    push_int(stack, pop_int(stack) >= a);
}
void apush_ltn(struct StackNode** stack){
    int a = pop_int(stack);
    push_int(stack, pop_int(stack) < a);
}
void apush_gtn(struct StackNode** stack){
    int a = pop_int(stack);
    push_int(stack, pop_int(stack) > a);
}

//bitwise
void apush_and(struct StackNode** stack){
    push_int(stack, pop_int(stack) & pop_int(stack));
}
void apush_or(struct StackNode** stack){
    push_int(stack, pop_int(stack) | pop_int(stack));
}
void apush_xor(struct StackNode** stack){
    push_int(stack, pop_int(stack) ^ pop_int(stack));
}
void apush_comp(struct StackNode** stack){
    push_int(stack, ~pop_int(stack));
}
void apush_shiftl(struct StackNode** stack){
    int a = pop_int(stack);
    push_int(stack, pop_int(stack) << a);
}
void apush_shiftr(struct StackNode** stack){
    int a = pop_int(stack);
    push_int(stack, pop_int(stack) >> a);
}

//arith
void apush_abs(struct StackNode** stack){
    int a = pop_int(stack);
    if (a<0) a = -a;
    push_int(stack, a);
}
void apush_prod(struct StackNode** stack){
    push_int(stack, pop_int(stack) * pop_int(stack));
}

void apush_div(struct StackNode** stack){
    int a = pop_int(stack);
    push_int(stack, pop_int(stack) / a);
}
void apush_pow(struct StackNode** stack){
    int exp = pop_int(stack), base = pop_int(stack), result = 1;
    while(1){
        if (exp & 1) result *= base;
        exp >>= 1;
        if (!exp) break;
        base *= base;
    }
    push_int(stack, result);
}
void apush_mod(struct StackNode** stack){
    int a = pop_int(stack);
    push_int(stack, pop_int(stack) % a);
}
void apush_add(struct StackNode** stack){
    push_int(stack, pop_int(stack) + pop_int(stack));
}
void apush_sub(struct StackNode** stack){
    int a = pop_int(stack);
    push_int(stack, pop_int(stack) - a);
}


//misc stack
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
void apush_swap(struct StackNode** stack){
    bool a_int = top_int(*stack);
    union Data a = pop(stack);
    bool b_int = top_int(*stack);
    union Data b = pop(stack);
    push(stack, a, a_int);
    push(stack, b, b_int);
}
void apush_drop(struct StackNode** stack){
    pop(stack);
}
void apush_dup(struct StackNode** stack){
    bool is_int = top_int(*stack);
    push(stack, peek(*stack), is_int);
}
void apush_cond(struct StackNode** stack){
    bool condition;
    if (top_int(*stack)) condition = peek_int(*stack) != 0;
    else condition = peek_str(*stack)[0] != '\0';
    pop(stack);
    if (condition){
       pop(stack); 
    } else {
        bool is_int = top_int(*stack);
        union Data data = pop(stack);
        pop(stack);
        push(stack, data, is_int);
    }
}

