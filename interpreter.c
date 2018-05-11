#include <unistd.h>

int run_interpreter(char* file){
    char *args[] = {"python3", file, NULL};
    return execvp(args[0], args);    
}
