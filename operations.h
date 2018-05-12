#include "stack.h"

void apush_add(struct StackNode**);
void apush_sub(struct StackNode**);

char* op_str[];
void (*op_func[]) (struct StackNode **);

int apush_num_ops();
