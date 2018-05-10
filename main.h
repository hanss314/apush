void apush_loop();
char* apush_read_line();
char** apush_split_line(char*);
int apush_execute(char**);

int apush_cd(char **args);
int apush_help(char **args);
int apush_exit(char **args);
int apush_num_builtins();

char *builtin_str[];
int (*builtin_func[]) (char **);
