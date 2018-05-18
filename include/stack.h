#ifndef STACK
#define STACK

union Data{
    int i;
    char* s;
};


struct StackNode;
struct StackNode* newNode(int); 

int isEmpty(struct StackNode*);

void push(struct StackNode**, union Data, bool is_int);
void push_int(struct StackNode**, int);
void push_str(struct StackNode**, char*);

union Data pop(struct StackNode**);
int pop_int(struct StackNode**);
char* pop_str(struct StackNode**);

bool top_int(struct StackNode*);

union Data peek(struct StackNode*);
int peek_int(struct StackNode*);
char* peek_str(struct StackNode*);

int get_size(struct StackNode*);
struct StackNode* rotate_stack(struct StackNode*, int);

#endif
