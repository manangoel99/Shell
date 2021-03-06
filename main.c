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
    "cronjob",
};

struct p processes[10000];
int running_proc_num;
struct cron crons[10000];
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
        printf("\n Suspened %d\n", current_running_proc);
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

    }
	signal(SIGTSTP, sigtstpHandler);

}


int check_up(char* command) {
    int i = 0;

    while (command[i] != '\0') {
        if (i % 3 == 0 && command[i] != '~') {
            return -1;
        }
        if (i % 3 == 1 && command[i] != '[') {
            return -1;
        }
        if (i % 3 == 2 && command[i] != 'A') {
            return -1;
        } 
        i++;
    }

    int count = 0;
    const char* temp = command;
    while (temp = strstr(temp, "~[A")) {
        count++;
        temp++;
    }
    return count;
}

char* get_nth_command(int n, char* root) {
    char* hist_file_path = (char*)malloc(1000);
    strcpy(hist_file_path, root);
    strcat(hist_file_path, "/.history");

    FILE* hist_file = fopen(hist_file_path, "r");

    char** commands = (char**)malloc(1000*sizeof(char*));
    int i = 0;
    int length = 0;

    commands[i] = (char*)malloc(1000);
    while (fgets(commands[i], 1000, hist_file) != NULL) {
        i++;
        commands[i] = (char*)malloc(1000);
    }

    int j = i - 1;
    char* tok = strtok(commands[j - n + 1], " ");
    tok = strtok(NULL, " ");
    char* comm = (char*)malloc(1000);
    strcpy(comm, "");
    while (tok != NULL) {
        strcat(comm, tok);
        strcat (comm, " ");
        tok = strtok(NULL, " ");
    }

    length = strlen(comm);
    comm [length - 2] = '\0';

    return comm;
}


char* root;

int shell_exit (char **args, char *root);
int shell_quit(char **args, char *root);
int cronjob(char **args, char* root);

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
    &cronjob,
};

int shell_exit(char** args, char *root) {
    exit(1);
}

int shell_quit(char** args, char *root) {
    exit(1);
}


char* PrintShellPrompt(char* root, char* prev_command) {
    char* hostname = (char*)malloc(1024);
    char* username = getenv("USER");
    char* cwd = (char*)malloc(8192);
    
    gethostname(hostname, 1024);
    getcwd(cwd, 8192);

    char* path = getPath(cwd, root);

    printf("<%s@%s %s> $ %s ", username, hostname, path, prev_command);
}

int cronjob(char** args, char* root) {
    int i = 0;
    char* comm = (char*)malloc(200);
    strcpy(comm, "");
    int time_interval, time_period;
    int flag_c = 0;
    int flag_p = 0;
    int flag_t = 0;

    while (args[i] != NULL) {
        if (strcmp(args[i], "-c") == 0) {
            int j = i + 1;
            while (args[j] != NULL) {

                if (strcmp(args[j], "-p") == 0) {
                    break;
                }
                if (strcmp(args[j], "-t") == 0) {
                    break;
                }
                strcat(comm, args[j]);
                strcat(comm, " ");
                j++;
            }
            comm[strlen(comm) - 1] = '\0';
            flag_c = 1;
        }
        if (strcmp(args[i], "-p") == 0) {
            flag_p = 1;
            time_period = atoi(args[i + 1]);
        }
        if (strcmp(args[i], "-t") == 0) {
            flag_t = 1;
            time_interval = atoi(args[i + 1]);
        }
        i++;
    }

    if (flag_p == 0 || flag_c == 0 || flag_t == 0) {
        fprintf(stderr, "Not Enough Arguments\n");
        return -1;
    }
    struct cron c;
    time_t n;
    time(&n);
    c.pname = comm;
    c.command = comm;
    // printf("%s\n", c.command);
    c.time_interval = time_interval;
    c.time_limit = time_period;
    c.start_time = n;
    pid_t pid = fork();
    if (pid == 0) {
        char** comm_toks = (char**)malloc(1000);
        comm_toks = SplitCommand(c.command);
        int d = 0;
        int up_press = check_up(c.command);
        int flag1 = 0;
        if (up_press > 0) {
            c.command = get_nth_command(up_press, root);
            PrintShellPrompt(root, c.command);
            printf("\n");
            comm_toks = SplitCommand(c.command);
            int l = 0;
            while(comm_toks[l] != NULL) {
                if (strcmp(comm_toks[l], "|") == 0 || strcmp(comm_toks[l], ">") == 0 || strcmp(comm_toks[l], ">>") == 0 || strcmp(comm_toks[l], "<") == 0) {
                    flag1 = 1;
                }
                l++;
            }
        }
        int l = 0;
        while(comm_toks[l] != NULL) {
            l++;
        }
        while (1) {
            time_t now;
            time(&now);

            if (now - c.start_time > c.time_limit) {
                printf("\n");
                PrintShellPrompt(root, "");
                exit(1);
            }
            else {
                if ((now - c.start_time) % c.time_interval == 0) {
                    printf("\n");
                    int i = 0;
                    int flag = 0;
                    while (i < sizeof(command_arr) / sizeof(char*)) {
                        if (strcmp(command_arr[i], comm_toks[0]) == 0 && flag1 == 0) {
                            // printf("HAHAH\n");
                            int num = (*functions[i])(comm_toks, root);
                            flag = 1;
                        }
                        i++;
                    }
                    if (flag == 0 || flag1 == 1){
                        int num = run_command(comm_toks, root);
                    }
                }

            }
            sleep(1);

        }
    }
    else {
        pid_t w = waitpid(-1, NULL, WNOHANG);
        // chdir(current_working_dir);
    }
    return 1;
}

void shell_loop(void) {
    char** args;
    char* commands;

    int state;

    while(1) {
        current_running_proc = -1;
       
        int la = 0;

        int status;
        pid_t pid;

        while (la < running_proc_num) {
            pid_t p = waitpid(processes[la].pid, &status, WNOHANG);
            if (p == processes[la].pid) {
                printf("%s Exited : %d\n", processes[la].pname, processes[la].pid);
            }
            la++;
        }


        PrintShellPrompt(root, "");
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
            // printf("%s\n", command);

            int up_press = check_up(command);
            // printf("%d\n", up_press);

            if (up_press > 0) {
                command = get_nth_command(up_press, root);
                PrintShellPrompt(root, command);
                printf("\n");
                comm_toks = SplitCommand(command);
                l = 0;
                while(comm_toks[l] != NULL) {

                    if (strcmp(comm_toks[l], "|") == 0 || strcmp(comm_toks[l], ">") == 0 || strcmp(comm_toks[l], ">>") == 0 || strcmp(comm_toks[l], "<") == 0) {
                        flag1 = 1;
                    }
                    l++;
                }
            
            }

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
        // chdir(current_working_dir);
    }
}

int main(void) {

    signal(SIGINT, sigintHandler);
	signal(SIGTSTP, sigtstpHandler);
    // current_working_dir = (char*)malloc(1000);
    // getcwd(current_working_dir, 250);
    shell_pid = getpid();

    root = getenv("PWD");
    shell_loop();
    return 0;
}