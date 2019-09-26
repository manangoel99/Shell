#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

#include "pathManip.h"
#include "getCommand.h"
#include "shell_echo.h"
#include "shell_pwd.h"
#include "shell_cd.h"
#include "shell_ls.h"
#include "shell_nightswatch.h"
#include "shell_history.h"
#include "shell_pinfo.h"
#include "shell_setenv.h"
#include "run_command.h"
#include "shell_jobs.h"

#define ll long long

char *command_arr[] = {
    "cd",
    "exit",
    "pwd",
    "echo", 
    "ls",
    "quit",
    "pinfo",
    "history",
    "nightswatch",
    "setenv",
    "unsetenv",
    "jobs",
    "overkill",
    "fg",
    "kjob",
    "bg",
};

struct p processes[10000];
int running_proc_num;
pid_t current_running_proc = -1;
pid_t shell_pid = -1;

void sigintHandler (int sig_num)
{
	signal(SIGINT, sigintHandler);
    if (current_running_proc != -1)
        printf("\nKilled %d\n", current_running_proc);
	fflush(stdout);
}

void sigtstpHandler(int sig_num) 
{
    // p = itoa current_running_proc
    if (current_running_proc != -1) {
        char* p = (char*)malloc(100);
        sprintf(p, "%d", current_running_proc);

        char* proc = (char*)malloc(1000);
        sprintf(proc, "/proc/%s/stat", p);

        FILE* stat_file = fopen(proc, "r");

        if (stat_file == NULL) {
            fprintf(stderr, "Error reading %s", proc);
        }
        int p_id;
    
        char status, *name = (char*)malloc(1000);
        fscanf(stat_file, "%d", &p_id);
        fscanf(stat_file, "%s", name);

        processes[running_proc_num].pname = (char*)malloc(sizeof(name));
        processes[running_proc_num].pname = name;
        processes[running_proc_num].status = 1;
        processes[running_proc_num].print_status = 0;
        processes[running_proc_num].stat = 1;
        processes[running_proc_num++].pid = p_id;
        // printf("%s\n", name);

    }
	signal(SIGTSTP, sigtstpHandler);

}

// void C_Signal(int sig) {
//     if (getpid() != shell_pid) {
//         return;
//     }

//     printf("Caught C\n");

//     if (current_running_proc != -1) {
//         printf("LOL\n%d\t%d\n", current_running_proc, shell_pid);

//         // kill(current_running_proc, SIGINT);
//     }
//     signal(SIGINT, C_Signal);
//     printf("LOLSKI\n");
// }


char* root;

int shell_exit (char **args, char *root);
int shell_quit(char **args, char *root);

int (*functions[])(char**, char*) = {
    &shell_cd, 
    &shell_exit, 
    &shell_pwd, 
    &shell_echo, 
    &shell_ls, 
    &shell_quit, 
    &shell_pinfo, 
    &shell_history, 
    &nightswatch, 
    &run_setenv, 
    &run_unsetenv, 
    &jobs, 
    &overkill, 
    &shell_fg,
    &shell_kjob,
    &shell_bg,
};

int shell_exit(char** args, char *root) {
    exit(1);
}

int shell_quit(char** args, char *root) {
    exit(1);
}


char* PrintShellPrompt(char* root) {
    char* hostname = (char*)malloc(1024);
    char* username = getenv("USER");
    char* cwd = (char*)malloc(8192);
    
    gethostname(hostname, 1024);
    getcwd(cwd, 8192);

    char* path = getPath(cwd, root);

    printf("<%s@%s %s> $ ", username, hostname, path);
}

void shell_loop(void) {
    char** args;
    char* commands;

    int state;

    while(1) {
        current_running_proc = -1;
       
        int la = 0;

        // while (la < running_proc_num) {
        //     int pid, status;
        //     pid = waitpid(processes[la].pid, &status, WNOHANG | WUNTRACED);            
        //     // if (WIFEXITED(status)) {
        //     //     if (processes[la].print_status == 0) {
        //     //         printf("%s\n", processes[la].pname);
        //     //         processes[la].print_status = 1;
        //     //         processes[la].status = 1;
        //     //     }
        //     // }
        //     if (processes[la].pid == pid) {
        //         printf("%s Exited\n", processes[la].pname);
        //         processes[la].print_status = 1;
        //         processes[la].status = 1;

        //     }

        //     la++;
        // }
        int status;
        pid_t pid;

        while (la < running_proc_num) {
            pid_t p = waitpid(processes[la].pid, &status, WNOHANG);
            if (p == processes[la].pid) {
                printf("%s Exited : %d\n", processes[la].pname, processes[la].pid);
            }
            la++;
        }


        PrintShellPrompt(root);
        char* comm = getCommands();
        if (strlen(comm) == 0) {
            continue;
        }

        char** comm_tokens = SplitCommands(comm);

        int k = 0;

        while (comm_tokens[k] != NULL) {

            char* command = (char*)malloc(1000);
            strcpy(command, "");

            int l = 0;
            int flag1 = 0;
            char** comm_toks = (char**)malloc(1000); 
            comm_toks = SplitCommand(comm_tokens[k]);

            while(comm_toks[l] != NULL) {
                if (strcmp(comm_toks[l], "|") == 0 || strcmp(comm_toks[l], ">") == 0 || strcmp(comm_toks[l], ">>") == 0 || strcmp(comm_toks[l], "<") == 0) {
                    flag1 = 1;
                }
                strcat(command, comm_toks[l++]);
                strcat(command, " ");
            }
            command[strlen(command) - 1] = '\0';

            int i = 0;
            int flag = 0;
            while (i < sizeof(command_arr) / sizeof(char*)) {
                if (strcmp(command_arr[i], comm_toks[0]) == 0 && flag1 == 0) {
                    int num = (*functions[i])(comm_toks, root);
                    flag = 1;
                }
                i++;
            }
            if (flag == 0 || flag1 == 1){
                int num = run_command(comm_toks, root);
            }


            char* temp = (char*)malloc(sizeof(root) + 100);

            strcpy(temp, root);


            strcat(temp, "/.history");
            char str[1000];        
            FILE* hist_file = fopen(temp, "r");
            int size = 0;
            int last_num;
            char* token;
            if (hist_file != NULL) {
                while (fgets(str, 1000, hist_file) != NULL);


                fseek(hist_file, 0, SEEK_END);
                size = ftell(hist_file);
                // last_num;
                token = strtok(str, " ");
                last_num = atoi(token);
                fclose(hist_file);

            }

            if (size == 0) {
                last_num = 0;
            }

            FILE* hist_file_write = fopen(temp, "a");
            fprintf(hist_file_write, "%d %s\n", last_num + 1, command);
            fclose(hist_file_write);
            free(command);
            free(temp);
            k++;
        }
        
    }
}

int main(void) {

    signal(SIGINT, sigintHandler);
	signal(SIGTSTP, sigtstpHandler);
    shell_pid = getpid();

    root = getenv("PWD");
    shell_loop();
    return 0;
}