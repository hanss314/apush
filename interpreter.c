#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int run_interpreter(char* file){
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
}
