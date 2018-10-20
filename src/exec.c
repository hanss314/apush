#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#include "builtins.h"

int exec_command(char** command){
    int i = 0;
    while(i++ || 1){
        if (command[i] == NULL) break;
        int position = 0;
        if (command[i][0] == '>'){
            position = 1;
        } else if (command[i][1] == '>'){
            position = 2;
        } else if (command[i][0] == '<'){
            char* file;
            int fd;
            if (command[i][position+1] != '\0'){
                file = command[i] + 1;
            } else if (command[i+1] != NULL){
                file = command[i+1];
                free(command[i]);
                command[i] = NULL;
                i++;
            } else if (command[i+1] == NULL){
                fprintf(stderr, "apush: expected file to read from\n");
                exit(EXIT_FAILURE);
            }
            if ((fd = open(file, O_RDONLY)) == -1){
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            continue;
        }
        if (position != 0){
            int fd;
            int source = STDOUT_FILENO;
            if (position == 2) source = (int)command[i][0] - 48;
            if (command[i][position] == '&'){
                if (command[i][position+1] == '\0'){
                    fprintf(stderr, "apush: expected number after '&'\n");
                    exit(EXIT_FAILURE);
                }
                fd = (int)command[i][position+1] - 48;
                free(command[i]);
                command[i] = NULL;
            } else {
                int flags = O_RDWR | O_CREAT;
                int end = position;
                char* file;
                mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
                if (command[i][position] == '>'){
                    flags |= O_APPEND;
                    end++;
                } else {
                    flags |= O_TRUNC;
                }
                if (command[i][end] != '\0'){
                    file = command[i] + end;
                } else if (command[i+1] != NULL){
                    file = command[i+1];
                    free(command[i]);
                    command[i] = NULL;
                    i++;
                } else if (command[i+1] == NULL){
                    fprintf(stderr, "apush: expected file to redirect to\n");
                    exit(EXIT_FAILURE);
                }
                if ((fd = open(file, flags, perms)) == -1){
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                free(command[i]);
                command[i] = NULL;
            }
            //command[i][0] == '&'
            if (source == -10){
                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
            }else{
                dup2(fd, source);
            }
            if (fd>2) close(fd);
        }
    }
    if (execvp(command[0], command) == -1) {
        perror("apush");
    }
    exit(EXIT_FAILURE);
}
int apush_launch(char ***args){
    int status;
    pid_t pid[64], wpid; 
    int fd[64][2];
    char** command;
    int i = 0;
    while(command = args[i]) {
        if (pipe(fd[i]) < 0){
            perror("apush");
            if (i > 0) {
                close(fd[i - 1][0]);
            }
            return 2;
        }
        pid[i] = fork();
        if (pid[i] == 0) {
            // Child process
            // Close pipes we don't need 
            if (args[i+1] != NULL){ 
                close(fd[i][0]);
                
                close(1);
                dup(fd[i][1]);
                close(fd[i][1]);
            }
            if (i > 0){
                close(0); // Also need to redirect stdin if this is not first process
                dup(fd[i-1][0]);
                close(fd[i-1][0]);
            }            
            exec_command(command); 
        } else if (pid[i] < 0) {
            // Error forking
            perror("apush");
            close(fd[i][0]);
            close(fd[i][1]);
            return 1;
        }
        close(fd[i][1]);
        if (i > 0) close(fd[i - 1][0]);
        i++;
    }
    do {
        wpid = waitpid(pid[i-1], &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    free(command);
    return 1;
}
int apush_execute(char ***args) {
    int i;
    if (args[0] == NULL) {
        // empty command
        return 1;
    }

    for (i = 0; i < apush_num_builtins(); i++) {
        if (strcmp(args[0][0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args[0]);
        }
    }

    int status = apush_launch(args);
    for(i=0; args[i] != NULL; i++){
        for(int j=0; args[i][j] != NULL; j++){
            free(args[i][j]);
        }
        free(args[i]);
    }
    free(args);
    return status;
}