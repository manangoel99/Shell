#include "shell_echo.h"

int shell_echo(char** args, char *root) {
    for (int i = 1; args[i] !=  NULL; i++) {
        remove_quotes(args[i], '\"');
        printf("%s ", args[i]);
    }
    printf("\n");
    return 1;
}