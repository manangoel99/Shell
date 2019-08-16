#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

char *command_arr[] = {
    "cd",
    "exit",
    "pwd",
    "echo", 
    "ls", 
    "quit"
};


char* root;
// // int shell_cd (char **args);
// // int shell_exit (char **args);
// // int shell_quit(char **args);
// // int shell_pwd (char **args);
// // int shell_echo (char **args);
// // int shell_ls (char **args);

// // int (*functions[])(char**) = {&shell_cd, &shell_exit, &shell_pwd, &shell_echo, &shell_ls, &shell_quit};

char* getPath(char* cwd) {
    int root_size = strlen(root), cwd_size = strlen(cwd);

    char* path = (char*)malloc(8192);

    if (root_size > cwd_size) {
        path[0] = '~';
        path[1] = '/';

        for (int i = 0; i < root_size - cwd_size - 1; i++) {
            path[i + 2] = cwd[root_size + i + 1];
        }
    }
    else if (root_size == cwd_size) {
        path[0] = '~';
        path[1] = '\0';
    }
    else {
        path = cwd;
    }

    return path;
}

char* PrintShellPrompt(char* root) {
    char* hostname = (char*)malloc(1024);
    char* username = getenv("USER");
    char* cwd = (char*)malloc(8192);
    
    gethostname(hostname, 1024);
    getcwd(cwd, 8192);

    char* path = getPath(cwd);

    printf("%s@%s %s $", username, hostname, path);
}

int main(void) {
    root = getenv("PWD");
    PrintShellPrompt(root);
}