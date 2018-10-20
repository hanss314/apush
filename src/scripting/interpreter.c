#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "heap.h"
#include "expression.h"
#include "parser.h"
#include "exec.h"

#define TOK_BUFSIZE 64

char* run_code(AObject*, int);
char* run_expression(SExpression*);

AObject* lex_code(char* s, int* nodes_len, int* position, long str_len){
    bool quote = false, escape = false;
    char quotechar = '\0';
    int bufsize = TOK_BUFSIZE, bufpos = 0;
    int nodesize = TOK_BUFSIZE, nodepos = 0;
    char* buffer = malloc(bufsize * sizeof(char));
    buffer[0] = '\0';
    AObject* nodes = malloc(bufsize * sizeof(AObject));
    int i;
    for (i = *position; i<str_len; i++){
        char c = s[i];
        if (c == '\0') break;
        if (bufpos >= bufsize-2){
            bufsize += TOK_BUFSIZE;
            buffer = realloc(buffer, bufsize * sizeof(char*));
            if (!buffer) {
                fprintf(stderr, "apush: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        if (nodepos >= nodesize-2){
            nodesize += TOK_BUFSIZE;
            nodes = realloc(nodes, nodesize * sizeof(char*));
            if (!nodes) {
                fprintf(stderr, "apush: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        if (escape){
            buffer[bufpos] = c; bufpos++;
            escape = false;
            goto loop_end;
        } else if (quote){
            if (c == quotechar){
                quotechar = '\0'; quote = false;
            } else {
                buffer[bufpos] = c; bufpos++;
            }
            goto loop_end;
        }
        switch (c){
            case '\'':
            case '"':
                quotechar = s[i]; quote = true;
                break;
            case '\\':
                escape = true; break;
            case '\n':
            case ';':
            case ' ':
                if (strlen(buffer) == 0) break;
                nodes[nodepos] = createValue(buffer); nodepos++;
                free(buffer);
                bufpos = 0; bufsize = TOK_BUFSIZE;
                buffer = malloc(bufsize * sizeof(char));
                if (c == '\n' || c == ';'){
                    nodes[nodepos] = createValue(";"); nodepos++;
                }
                break;

            case '(':
                i++;
                int length = 0;
                AObject* exp_list = lex_code(s, &length, &i, str_len);
                AObject child = createExp(length, exp_list);
                nodes[nodepos] = child; nodepos++; break;
            case ')':
                goto lexer_end;

            default:
                buffer[bufpos] = c; bufpos++;
        }
        loop_end:
        buffer[bufpos] = '\0';
    }
    lexer_end:
    if (strlen(buffer) != 0) {
        nodes[nodepos] = createValue(buffer);
        nodepos++;
    }
    *nodes_len = nodepos;
    *position = i;
    free(buffer);
    return nodes;
}

char* run_expression(SExpression* expr){
    for (int i=0; i < expr->length; i++){
        if (expr->nodes[i].is_expr){
            AObject node = expr->nodes[i];
            expr->nodes[i] = createValue(run_expression(node.expr));
            deleteExp(node.expr);
        }
    }
    char* command = expr->nodes[0].value;
    if (strcmp(command, "code") == 0){
        run_code(expr->nodes + 1, expr->length - 1);
    } else if (strcmp(command, "define") == 0){
        heap_insert(expr->nodes[1].value, expr->nodes[2].value);
    } else if (strcmp(command, "$") == 0){
        char *val = lookup(expr->nodes[1].value);
        return val;
    }
    return "";
}

// Returns null terminated string containing the return value of the last command (hopefully)
char* run_code(AObject *code, int len){
    for (int i=0; i<len; i++){
        AObject a = code[i];
        if (!a.is_expr){
            int j = i;
            int size = 2;
            AObject current;
            do {
                current = code[j];
                size += strlen(current.value) + 1;
                j++;
            } while (!current.is_expr && strcmp(current.value, ";") && j<len);
            if (j == len) j++;
            char *buffer = malloc(size);
            strcpy(buffer, "");
            for (int k=i; k<j-1; k++){
                strcat(buffer, code[k].value);
                if (k != j-2) strcat(buffer, " ");
            }
            char ***args = apush_split_line(buffer, 1);
            apush_execute(args);
            free(buffer);

            i = j-1;
        } else {
            run_expression(a.expr);
        }
    }
    return "";
}

void print(AObject *a, int len){
    printf("Expr %d\n", len);
    for (int i=0; i<len; i++){
        if (a[i].is_expr){
            print(a[i].expr->nodes, a[i].expr->length);
        } else {
            printf("%s\n", a[i].value);
        }
    }
    printf("EndExpr\n");
}

void run_interpreter(char* filename){
    FILE *fp = fopen(filename, "r");
    fseek(fp, 0L, SEEK_END);
    long fsize = ftell(fp);
    rewind(fp);
    char *contents = (char*) malloc(fsize);
    fread(contents, fsize, 1, fp);
    fclose(fp);
    int len=0, j=0;
    AObject* a = lex_code(contents, &len, &j, fsize);
    run_code(a, len);
    deleteObjs(a, len);
    free(contents);
}

