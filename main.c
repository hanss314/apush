#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "main.h"


void apush_init(){
    if (access("/etc/apushrc", F_OK) != -1){
        run_interpreter("/etc/apushrc");
    }
    char local_rc[128];
    strcpy(local_rc, "");
    strcat(local_rc, getenv("HOME"));
    strcat(local_rc, "/.apushrc");
    if (access(local_rc, F_OK) != -1){
        run_interpreter(local_rc);
    }    
}

int main(int argc, char **argv) {
    apush_init();
    if (argc > 1){
        run_interpreter(argv[1]);
    } else {
        apush_loop();
    }
    return EXIT_SUCCESS;
}

void apush_loop() {
    char *line;
    char ***args;
    int status;
    char cwd[1024];
    do {
        if (getcwd(cwd, sizeof(cwd)) == NULL) strncpy(cwd, "apush", 1024);
        printf("%s>$ ", cwd);
        line = apush_read_line();
        if (strlen(line) > 1){
            args = apush_split_line(line);
            status = apush_execute(args);
            
            free(line);
            free(args);
        } 
    } while(status);
}

char *apush_read_line(void) {
    char *line = NULL;
    ssize_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
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
                
                close(2); 
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
