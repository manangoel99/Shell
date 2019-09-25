#include "shell_jobs.h"

struct p processes[10000];
int running_proc_num;

int jobs(char** args, char* root) {
    int la = 0;
    while (la < running_proc_num)
    {
        // printf("%d\n", la);
        // fprintf(stdout, "%s %d\n", processes[la].pname, processes[la].pid);
        // fprintf(stdout, "\[%d\] ")
        if (processes[la].status == 1) {
            fprintf(stdout, "\[%d\] Stopped %s \[%d\]\n", la + 1, processes[la].pname, processes[la].pid);
        }
        if (processes[la].status == 0) {
            fprintf(stdout, "\[%d\] Running %s \[%d\]\n", la + 1, processes[la].pname, processes[la].pid);
        }
        la++;
    }
    return 1;    
}

int overkill(char** args, char* root) {
    int la = 0;

    while(la < running_proc_num) {
        if (processes[la].status == 0) {
            int stat = kill(processes[la].pid, SIGINT);

            if (stat < 0) {
                perror("Error in Killing process");
                return -1;
            }
            else {
                fprintf(stdout, "Killed - %s %d\n", processes[la].pname, processes[la].pid);
            }

        }
        la++;
    }
    return 1;
}