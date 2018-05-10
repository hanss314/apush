#include <stdio.h>
#include <unistd.h>
#include "builtins.h"

char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &apush_cd,
    &apush_help,
    &apush_exit
};

int apush_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int apush_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "apush: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("apush");
        }
    }
    return 1;
}

int apush_help(char **args) {
    int i;
    printf("Alternative Pathetic Useless Shell\n");
    printf("Use this like a normal shell\n");
    printf("Built-in commands:\n");

    for (i = 0; i < apush_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the \"man\" command for information on other programs.\n");
    return 1;
}

int apush_exit(char **args) {
  return 0;
}
