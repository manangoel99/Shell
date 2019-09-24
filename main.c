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
#include "run_command.h"

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
};

struct p processes[10000];

void sigintHandler (int sig_num)
{
	signal(SIGINT, sigintHandler);
	fflush(stdout);
}

void sigtstpHandler(int sig_num) 
{ 
	signal(SIGTSTP, sigtstpHandler);
}


char* root;

int shell_exit (char **args, char *root);
int shell_quit(char **args, char *root);

int (*functions[])(char**, char*) = {&shell_cd, &shell_exit, &shell_pwd, &shell_echo, &shell_ls, &shell_quit, &shell_pinfo, &shell_history, &nightswatch};

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

        int pid, status;

        while((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
            char* pname;
            // printf("%d\n", pid);

            if (WIFEXITED(status)) {
                int la = 0;
                while (la < running_proc_num) {
                    pname = processes[la].pname;
                    fprintf(stderr, "Process %s:%d exited normally\n", pname, processes[la].pid);

                    processes[la].pid = -1;

                    break;
                    la++;

                }
            }
            if (WIFSIGNALED(status)) {
                int la = 0;
                while (la < running_proc_num) {
                    pname = processes[la].pname;
                    fprintf(stderr, "Process %s:%d exited with signal\n", pname, processes[la].pid);

                    processes[la].pid = -1;

                    break;
                    la++;
                }
            }
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

    root = getenv("PWD");
    shell_loop();
    return 0;
}