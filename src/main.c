#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "builtins.h"
#include "interpreter.h"
#include "parser.h"

#define VERSION "0.0.1"

void apush_loop();
char* apush_read_line();
int apush_execute(char***);

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
            args = apush_split_line(line, 1);
            status = apush_execute(args);
            
            free(line);
            for(int i=0; args[i] != NULL; i++){
                for(int j=0; args[i][j] != NULL; j++){
                    free(args[i][j]);
                }
                free(args[i]);
            }
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
