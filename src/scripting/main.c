#include <stdio.h>
#include <stdlib.h>

#include "expression.h"
#include "interpreter.h"

int main(int argc, char* argv[]){
    if (argc < 2){
        return 0;
    }
    FILE *fp = fopen(argv[1], "r");
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
    int len=0, j=0;
    AObject* a = lex_code(contents, &len, &j, fsize);
    print(a, len);
    run_code(a, len);
    deleteObjs(a, len);
    free(contents);

}