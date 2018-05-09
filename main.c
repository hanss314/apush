#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define APUSH_TOK_BUFSIZE 256
#define APUSH_TOK_DELIM " \t\r\n\a"

void apush_loop();
char* apush_read_line();
char** apush_split_line(char*);
int apush_execute(char**);

int apush_cd(char **args);
int apush_help(char **args);
int apush_exit(char **args);

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

int main(int argc, char **argv) {
    apush_loop();
    return EXIT_SUCCESS;
}

void apush_loop() {
    char *line;
    char **args;
    int status;

    do {
        printf("apush>$ ");
        line = apush_read_line();
        args = apush_split_line(line);
        status = apush_execute(args);

        free(line);
        free(args);
    } while(status);
}

char *apush_read_line(void) {
    char *line = NULL;
    ssize_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

char **apush_split_line(char *line) {
    int bufsize = APUSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "apush: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, APUSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += APUSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "apush: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, APUSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
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
    printf("Alternative Pathetic/Plagiarized Useless Shell\n");
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

int apush_execute(char **args) {
    int i;

    if (args[0] == NULL) {
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


