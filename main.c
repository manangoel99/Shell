#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "pathManip.c"
#include "getCommand.c"
#include "utils.c"

#define ll long long

char *command_arr[] = {
    // "cd",
    "exit",
    // "pwd",
    "echo", 
    // "ls", 
    "quit"
};


char* root;
// int shell_cd (char **args);
int shell_exit (char **args);
int shell_quit(char **args);
// int shell_pwd (char **args);
int shell_echo (char **args);
// int shell_ls (char **args);

int (*functions[])(char**) = {&shell_exit, &shell_echo, &shell_quit};

int shell_exit(char** args) {
    exit(1);
}

int shell_quit(char** args) {
    exit(1);
}

int shell_echo(char** args) {
    for (int i = 1; args[i] !=  NULL; i++) {
        remove_quotes(args[i], '\"');
        printf("%s ", args[i]);
    }
    printf("\n");
    return 1;
}

char* PrintShellPrompt(char* root) {
    char* hostname = (char*)malloc(1024);
    char* username = getenv("USER");
    char* cwd = (char*)malloc(8192);
    
    gethostname(hostname, 1024);
    getcwd(cwd, 8192);

    char* path = getPath(cwd, root);

    printf("%s@%s %s $ ", username, hostname, path);
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
                // printf("%s\n", command_arr[i]);
                break;
            }
            i++;
        }
        int num = (*functions[i])(comm_tokens);

    }
}

int main(void) {
    root = getenv("PWD");
    shell_loop();
    // PrintShellPrompt(root);
}