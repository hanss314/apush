#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "stack.h"
// A structure to represent a stack
struct StackNode
{
    struct StackNode* next;
    int data;
    char* data_str;
    bool is_int;    
};
 
struct StackNode* newNode(int data)
{
    struct StackNode* stackNode =
              (struct StackNode*) malloc(sizeof(struct StackNode));
    stackNode->data = data;
    stackNode->data_str = "";
    stackNode->is_int = true;
    stackNode->next = NULL;
    return stackNode;
}

struct StackNode* newStrNode(char* string){
    struct StackNode* stackNode = (struct StackNode*) malloc(sizeof(struct StackNode));
    stackNode->data = 0;
    stackNode->data_str = string;
    stackNode->is_int = false;
    stackNode->next = NULL;
    return stackNode;
}

int isEmpty(struct StackNode *root)
{
    return !root;
}
 
void push(struct StackNode** root, int data)
{
    struct StackNode* stackNode = newNode(data);
    stackNode->next = *root;
    *root = stackNode;
}

void push_str(struct StackNode** root, char* data)
{
    struct StackNode* stackNode = newStrNode(data);
    stackNode->next = *root;
    *root = stackNode;
}

int pop(struct StackNode** root)
{
    if (isEmpty(*root))
        return INT_MIN;
    struct StackNode* temp = *root;
    *root = (*root)->next;
    int popped = temp->data;
    free(temp);
 
    return popped;
}

char* pop_str(struct StackNode** root){
    if (isEmpty(*root)) return "";
    struct StackNode* temp = *root;
    *root = (*root)->next;
    char* popped = temp->data_str;
    free(temp);

    return popped;
}

int peek(struct StackNode* root)
{
    if (isEmpty(root))
        return INT_MIN;
    return root->data;
}

int get_size(struct StackNode* root){
    int size = 0;
    struct StackNode* temp = root;
    while(temp){
        temp = temp->next;
        size++;
    }
    free(temp);
    return size; 
}

struct StackNode* rotate_stack(struct StackNode* root, int count){ 
    struct StackNode* newroot = root;
    struct StackNode* prevroot = NULL; 
    for (int i=0; i<count; i++){
        prevroot = newroot;
        newroot = newroot->next;
    } 
    if(prevroot != NULL){
        prevroot->next = NULL;
    }
    struct StackNode* oldend = newroot;
    while (oldend->next != NULL){
        oldend = oldend->next;
    }
    oldend->next = root;
    free(oldend);
    free(root);
    free(prevroot);
    return newroot;
}
/* 
int main()
{
    struct StackNode* root = NULL;
 
    push(&root, 10);
    push(&root, 20);
    push(&root, 30);
 
    printf("%d popped from stack\n", pop(&root));
 
    printf("Top element is %d\n", peek(root));
 
    return 0;
}
*/
