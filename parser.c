#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define APUSH_TOK_BUFSIZE 1024
#define APUSH_SING_TOK_SIZE 256
#define APUSH_TOK_DELIM " \t\r\n\a"
#define APUSH_PIPE_CHAR '|'
#define APUSH_QUOTES "'\""

#define APUSH_PIPE_MAX 8

char*** apush_split_line(char *line, int split_pipes) {
    int bufsize = APUSH_TOK_BUFSIZE, toksize = APUSH_SING_TOK_SIZE, position=0, argpos=0;
    int pipesize = APUSH_PIPE_MAX, pipepos = 0;
    char ***pipes = malloc(pipesize * sizeof(char*));
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token = malloc(toksize * sizeof(char*));
    char current;
    bool quote=false, escape=false;
    char quotechar;
    for (int i=0; i < strlen(line); i++) {
        current = line[i];
        if (position >= toksize){
            toksize += APUSH_SING_TOK_SIZE;
            token = realloc(token, toksize * sizeof(char*));
            if (!token) {
                fprintf(stderr, "apush: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        if (argpos >= bufsize){
            bufsize += APUSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "apush: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        if (pipepos >= pipesize){
            pipesize += APUSH_PIPE_MAX;
            pipes = realloc(pipes, pipesize * sizeof(char*));
            if (!pipes) {
                fprintf(stderr, "apush: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        if(escape){
            token[position] = current;
            position++;
            escape = false;
        } else if (current=='\\'){
            escape=true;
        } else if (quote){
            if (current == quotechar){
                quote=false;
                quotechar = '\0';
            } else {
                token[position] = current; position++;
            }
        } else if (strchr(APUSH_QUOTES, current)){
            quote=true;
            quotechar = current;
        } else if (strchr(APUSH_TOK_DELIM, current)){
            if (token[0] != '\0'){
                tokens[argpos] = token; argpos++;
                token = malloc(toksize * sizeof(char*)); position = 0;
            }
        } else if (split_pipes && current == APUSH_PIPE_CHAR){
            tokens[argpos] = NULL;
            
            pipes[pipepos] = tokens; pipepos++;
            tokens =  malloc(bufsize * sizeof(char*)); argpos = 0;
        } else {
            token[position] = current; position++;
        }
    }    
    pipes[pipepos] = tokens;
    pipes[pipepos+1] = NULL;
    return pipes;
}
