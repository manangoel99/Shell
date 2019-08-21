#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "pathManip.c"
#include "getCommand.c"
#include "utils.c"
#include "shell_echo.c"
#include "shell_pwd.c"
#include "shell_cd.c"
#include "shell_ls.c"

#define ll long long

char *command_arr[] = {
    "cd",
    "exit",
    "pwd",
    "echo", 
    "ls",
    "quit"
};


char* root;
// int shell_cd (char **args);
int shell_exit (char **args, char *root);
int shell_quit(char **args, char *root);
// int shell_ls (char **args);

int (*functions[])(char**, char*) = {&shell_cd, &shell_exit, &shell_pwd, &shell_echo, &shell_ls, &shell_quit};

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
        PrintShellPrompt(root);
        char* comm = getCommands();
        char** comm_tokens = SplitCommand(comm);

        int i = 0;
        while (i < sizeof(command_arr) / sizeof(char*)) {
            if (strcmp(command_arr[i], comm_tokens[0]) == 0) {
                break;
            }
            i++;
        }
        int num = (*functions[i])(comm_tokens, root);

    }
}

int main(void) {
    root = getenv("PWD");
    shell_loop();
    return 0;
}