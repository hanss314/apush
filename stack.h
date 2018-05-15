struct StackNode;
struct StackNode* newNode(int); 
int isEmpty(struct StackNode*);
void push(struct StackNode**, int);
void push_str(struct StackNode**, char*);
int pop(struct StackNode**);
char* pop_str(struct StackNode**);
int peek(struct StackNode*);

int get_size(struct StackNode*);
struct StackNode* rotate_stack(struct StackNode*, int);
