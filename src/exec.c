#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "builtins.h"

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
            if (execvp(command[0], command) == -1) {
                perror("apush");
            }
            exit(EXIT_FAILURE);
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

    return apush_launch(args);
}
