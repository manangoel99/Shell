#if !defined(SHELL_H)
#define SHELL_H

struct p {
    char* pname;
    int pid;
    int status;
    int print_status;
    int stat;
};

extern int running_proc_num;
extern struct p processes[10000];


#endif // SHELL_H
