#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "builtins.h"
#include "interpreter.h"
#include "parser.h"
#include "exec.h"

#define VERSION "0.0.1"

void apush_loop();
char* apush_read_line();
int apush_execute(char***);

char* get_cwd() {
    static char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) strncpy(cwd, "apush", 1024);
    return cwd;
}

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

void sigint () {
    printf("\n");
    printf("%s>$ ", get_cwd());
    fflush(stdout);
}

int main(int argc, char **argv) {
    signal(SIGINT, &sigint);
    if (argc > 1){
        if(strcmp(argv[1], "--version") == 0){
            printf("%s\n", VERSION);
            return EXIT_SUCCESS;
        } else {
            run_interpreter(argv[1]);
        }
    } else {         
        apush_init();
        apush_loop();
        printf("We're done.\n");
    }
    return EXIT_SUCCESS;
}

void apush_loop() {
    char *line;
    int status = 1;

    do {
        fflush(stdin);
        fflush(stdout);
        fflush(stderr);
        printf("%s>$ ", get_cwd());
        line = apush_read_line();
        if (line == NULL) {
            status = 0;
            exit(0);
        }
        if (strlen(line) > 0){
            char ***args = apush_split_line(line, 1);
            free(line);
            status = apush_execute(args);
        }
    } while(status);
}

char *apush_read_line(void) {
    char *line = NULL;
    ssize_t bufsize = 0;
    int length = getline(&line, &bufsize, stdin);
    if (length == -1){
        free(line);
        line = NULL;
    } else {
        line = realloc(line, length + 1);
        line[length] = '\0';
    }
    return line;
}
