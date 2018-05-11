#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "main.h"

int main(int argc, char **argv) {
    if (argc > 1){
        run_interpreter(argv[1]);
    } else {
        apush_loop();
    }
    return EXIT_SUCCESS;
}

void apush_loop() {
    char *line;
    char **args;
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

int apush_launch(char **args) {
    pid_t pid, wpid;
    int status;
    pid = fork();

    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("apush");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("apush");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int apush_execute(char **args) {
    int i;
    if (args[0][0] == '\0') {
        // empty command
        return 1;
    }

    for (i = 0; i < apush_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }
    return apush_launch(args);
}
