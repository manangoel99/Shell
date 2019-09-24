#include "shell_setenv.h"
#define ll long long

int run_setenv(char** args, char* root) {
    ll count = 0, i = 0;

    while (args[i] != NULL) {
        count++;
        i++;
    }

    if (count >= 4) {
        fprintf(stderr, "Too many arguments\n");
    }
    else if (count <=  1) {
        fprintf(stderr, "Too few arguments\n");
    }
    else if (count == 2) {
        if (setenv(args[1], "", 1) == -1) {
            perror("setenv error");
            return -1;
        }
    }
    else {
        if (setenv(args[1], args[2], 1) == -1) {
            perror("setenv error");
            return -1;
        }
    }
    return 1;
}

int run_unsetenv(char** args, char* root) {
    ll count = 0, i = 0;

    while (args[i] != NULL) {
        count++;
        i++;
    }

    if (count >= 3) {
        fprintf(stderr, "Too many arguments\n");
    }
    else if (count <=  1) {
        fprintf(stderr, "Too few arguments\n");
    }
    else if (count == 2) {
        if (unsetenv(args[1]) == -1) {
            perror("unsetenv error");
            return -1;
        }
    }
    return 1;
}