#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "stack.h"


// A structure to represent a stack
struct StackNode
{
    struct StackNode* next;
    union Data data;
    bool is_int;    
};
 
struct StackNode* newNode(int data)
{
    struct StackNode* stackNode =
              (struct StackNode*) malloc(sizeof(struct StackNode));
    stackNode->data.i = data;
    stackNode->is_int = true;
    stackNode->next = NULL;
    return stackNode;
}

struct StackNode* newStrNode(char* string){
    struct StackNode* stackNode = (struct StackNode*) malloc(sizeof(struct StackNode));
    stackNode->data.s = string;
    stackNode->next = NULL;
    return stackNode;
}

int isEmpty(struct StackNode *root)
{
    return !root;
}

void push(struct StackNode** root, union Data data){
    struct StackNode* stackNode = newNode(data.i);
    stackNode->data = data;
    stackNode->next = *root;
    *root = stackNode;
}

void push_int(struct StackNode** root, int data)
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

union Data pop(struct StackNode** root){
    if (isEmpty(*root)){
        union Data data;
        data.i = 0;
        return data;
    }
    struct StackNode* temp = *root;
    *root = (*root)->next;
    union Data popped = temp->data;
    free(temp);
 
    return popped;

}

int pop_int(struct StackNode** root)
{
    return pop(root).i;
}

char* pop_str(struct StackNode** root){
    if (isEmpty(*root)) return "";
    struct StackNode* temp = *root;
    *root = (*root)->next;
    char* popped = temp->data.s;
    free(temp);

    return popped;
}

union Data peek(struct StackNode* root)
{
    if (isEmpty(root)){
        union Data data;
        data.i = 0;
        return data;
    }
    return root->data;
}

int peek_int(struct StackNode* root)
{
    if (isEmpty(root))
        return INT_MIN;
    return root->data.i;
}

char* peek_str(struct StackNode* root){
    if (isEmpty(root)) return "";
    return root->data.s;
}

bool top_int(struct StackNode* root){
    if (isEmpty(root)) return false;
    return root->is_int;
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

