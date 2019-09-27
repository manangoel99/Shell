#include <sys/types.h>
#include <time.h>
#if !defined(SHELL_H)
#define SHELL_H

struct p {
    char* pname;
    int pid;
    int status;
    int print_status;
    int stat;
};

struct cron {
    char* pname;
    char* command;
    time_t time_interval;
    time_t time_limit;
    time_t start_time;
};

extern char* current_working_dir;

// extern int shm_fd;
extern void* dir_ptr;

extern int running_proc_num;
extern struct p processes[10000];
extern pid_t current_running_proc;
extern pid_t shell_pid;
extern struct p current_proc;
extern struct cron crons[10000];
#endif // SHELL_H
