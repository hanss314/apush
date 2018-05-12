#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parser.h"
#include "stack.h"
#include "operations.h"

char** read_to_tokens(char* filename){ 
    FILE *fp = fopen(filename, "r");  
    if (fp == NULL){
        printf("apush: No such file");
        exit(1);
    }
    fseek(fp, 0L, SEEK_END);
    long fsize = ftell(fp);
    rewind(fp);
    char *contents = (char*) malloc(fsize); 
    fread(contents, fsize, 1, fp);
    fclose(fp);
    return apush_split_line(contents, 0)[0];
}

struct StackNode* execute_code(char** code){
    struct StackNode* stack;
    int p = 0;
    while(1){
        char* instruction = code[p];
        bool instr_found = false;
        if (instruction == NULL) break;
        for (int i = 0; i < apush_num_ops(); i++) {
            if (strcmp(instruction, op_str[i]) == 0) {
                (*op_func[i])(&stack); 
                instr_found = true; break;
            }
        }
        if(!instr_found){
            push(&stack, atoi(instruction));
        }
        p++;
    }
    return stack;
}

int run_interpreter(char* file){
    char** code = read_to_tokens(file);
    struct StackNode* stack = execute_code(code);
    return 0;
    /*
    pid_t pid, wpid;
    int status;
    pid = fork();
    if (pid<0){
        perror("apush");
        return -1;
    } else if(pid == 0){
        char *args[] = {"python3", file, NULL};
        if (execvp(args[0], args) == -1){
            //perror("apush");
        }
        exit(EXIT_FAILURE);
    }
    do {
        wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    return 1;
    */
}
