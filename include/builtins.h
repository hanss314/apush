#define NUM_BUILTINS 3

int apush_cd(char**);
int apush_help(char**);
int apush_exit(char**);
int apush_num_builtins();

const char* builtin_str[NUM_BUILTINS];
int (*builtin_func[NUM_BUILTINS]) (char **);

