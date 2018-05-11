void apush_loop();
char* apush_read_line();
char*** apush_split_line(char*);
int apush_execute(char***);

int apush_cd(char**);
int apush_help(char**);
int apush_exit(char**);
int apush_num_builtins();

char *builtin_str[];
int (*builtin_func[]) (char **);

int run_interpreter(char*);
