#include "shell_pwd.h"

int shell_pwd(char** args, char *root) {
    int num_args = 0;

    for (int i = 0; args[i] != NULL; i++) {
        num_args++;
    }

    if (num_args > 1) {
        fprintf(stdout, "Too Many Arguments\n");
        return 0;
    }

    char *cwd1 = (char*) malloc(sizeof(char) * 8192);
    getcwd(cwd1, 8192);
    fprintf(stdout, "%s\n", cwd1);
    free(cwd1);
    return 1;
}