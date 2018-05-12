struct StackNode;
struct StackNode* newNode(int); 
int isEmpty(struct StackNode*);
void push(struct StackNode**, int);
int pop(struct StackNode**);
int peek(struct StackNode*);

