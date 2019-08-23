#include "shell.h"
#include "run_command.h"

int running_proc_num = 0;
struct p processes[10000];

int run_command(char **args) {

    int background = 0, i = 0, j = 0;

    pid_t pid;

    while(args[i] != NULL) {
        j = 0;
        while(args[i][j] != '\0') {
            if (args[i][j] == '&') 
                background = 1;
            j++;
        }

        i++;
    }

    char **args1 = (char**)malloc(sizeof(char*) * 100);
    
    if (background == 1) {
        for (int k = 0; args[k + 1] != NULL; k++) {
            args1[k] = args[k];
        }
    }
    pid = fork();
    if (pid < 0) {
        perror("Error");
    }
    else if (!pid) {
        if (background == 0) {
            int n = execvp(args[0], args);
            if (n < 0) {
                perror("Error");
            }
        }
        else {
            int n = execvp(args[0], args1);
            if (n < 0) {
                perror("Error");
            }
        }
    }
    int state;
    if (background == 0) {
        pid_t w = waitpid(pid, &state, WUNTRACED);
        // while(1) {
            // if (!(WIFEXITED(state) || WIFSIGNALED(state))) {
                // w = waitpid(pid, &state, WUNTRACED);
                // printf("%d\n", state);
                // continue;
            // }
            // break;
        // }
    }
    else {
        processes[running_proc_num].pname = (char*)malloc(sizeof(args[0]));
        processes[running_proc_num].pname = args[0];
        // strcpy(processes[running_proc_num].pname, args[0]);
        processes[running_proc_num++].pid = pid;
    }

}