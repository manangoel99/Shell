#include "shell_jobs.h"

struct p processes[10000];
int running_proc_num;
pid_t current_running_proc;

int jobs(char** args, char* root) {
    int la = 0;
    while (la < running_proc_num)
    {
        // printf("%d\n", la);
        // fprintf(stdout, "%s %d\n", processes[la].pname, processes[la].pid);
        // fprintf(stdout, "\[%d\] ")
        if (processes[la].status == 1) {
            fprintf(stdout, "[%d] Stopped %s [%d]\n", la + 1, processes[la].pname, processes[la].pid);
        }
        if (processes[la].status == 0) {
            fprintf(stdout, "[%d] Running %s [%d]\n", la + 1, processes[la].pname, processes[la].pid);
        }
        if (processes[la].status == 2) {
            fprintf(stdout, "[%d] Moved %s [%d] To Foreground\n", la + 1, processes[la].pname, processes[la].pid);
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

int shell_fg(char** args, char* root) {
    int count = 0, i = 0;
    while (args[i] != NULL) {
        i++;
        count++;
    }

    if (count >= 3) {
        fprintf(stderr, "fg: Too many Arguments\n");
        return -1;
    }
    else if (count <= 1) {
        fprintf(stderr, "fg: Too few Arguments\n");
        return -1;
    }
    else {
        int num = atoi(args[1]);

        if (num > running_proc_num) {
            perror("Process doesn't exist");
            return -1;
        }
        else {
            // printf("%d %d %s\n", num, running_proc_num, processes[num - 1].pname);
            kill(processes[num - 1].pid, SIGCONT);
            current_running_proc = processes[num - 1].pid;
            waitpid(-1, NULL, WUNTRACED);
            processes[num - 1].status = 2;
        }

    }
}

int shell_kjob(char** args, char* root) {
    int num_args = 0, i = 0;

    while (args[i] != NULL) {
        i++;
        num_args++;
    }

    if (num_args >= 4) {
        fprintf(stderr, "kjob: Too many Arguments\n");
        return -1;
    }
    else if (num_args <= 2) {
        fprintf(stderr, "kjob: Too few arguments\n");
        return -1;
    }
    else {
        int jno = atoi(args[1]);
        int sig = atoi(args[2]);

        if (jno > running_proc_num) {
            perror("Process doesn't exist");
            return -1;
        }
        else {
            kill(processes[jno - 1].pid, sig);
        }

    }
    return 1;

}

int shell_bg(char** args, char* root) {
    int count = 0, i = 0;

    while (args[i] != NULL) {
        i++;
        count++;
    }

    if (count >= 3) {
        fprintf(stderr, "bg: Too many Arguments\n");
        return -1;
    }
    else if (count <= 1) {
        fprintf(stderr, "bg: Too few Arguments\n");
        return -1;
    }
    else {
        int jno = atoi(args[1]);

        if (jno > running_proc_num) {
            perror("Process Doesn't Exist");
            return -1;
        }
        else {
            kill(processes[jno - 1].pid, SIGTTIN);
            kill(processes[jno - 1].pid, SIGCONT);

            processes[jno - 1].status = 0;
            processes[jno - 1].print_status = 0;

        }
    }
    return 1;
}