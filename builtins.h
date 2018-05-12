int apush_cd(char**);
int apush_help(char**);
int apush_exit(char**);
int apush_num_builtins();

char* builtin_str[];
int (*builtin_func[]) (char **);

