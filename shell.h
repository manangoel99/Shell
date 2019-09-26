#include <sys/types.h>
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
extern pid_t current_running_proc;
extern pid_t shell_pid;
extern struct p current_proc;
#endif // SHELL_H
