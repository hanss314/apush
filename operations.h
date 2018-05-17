#include "stack.h"

void apush_add(struct StackNode**);
void apush_sub(struct StackNode**);
void apush_prod(struct StackNode**);
void apush_div(struct StackNode**);
void apush_pow(struct StackNode**);
void apush_mod(struct StackNode**);
void apush_add(struct StackNode**);
void apush_abs(struct StackNode**);

void apush_and(struct StackNode**);
void apush_or(struct StackNode**);
void apush_xor(struct StackNode**);
void apush_comp(struct StackNode**);
void apush_shiftl(struct StackNode**);
void apush_shiftr(struct StackNode**);

void apush_eq(struct StackNode**);
void apush_neq(struct StackNode**);
void apush_leq(struct StackNode**);
void apush_ltn(struct StackNode**);
void apush_geq(struct StackNode**);
void apush_gtn(struct StackNode**);

void apush_itoc(struct StackNode**);

void apush_set(struct StackNode**);
void apush_get(struct StackNode**);

void apush_rot(struct StackNode**);
void apush_print_int(struct StackNode**);
void apush_eval(struct StackNode**);
void apush_drop(struct StackNode**);
void apush_dup(struct StackNode**);
void apush_cond(struct StackNode**);
void apush_swap(struct StackNode**);

char* op_str[];
void (*op_func[]) (struct StackNode **);
int apush_num_ops();

