#include <stdio.h>
#include <stdlib.h>

int shell_history(char** args, char* root) {
    // printf("HAHAH\n");
    char* hist_file_path = (char*)malloc(1000);
    strcpy(hist_file_path, root);
    strcat(hist_file_path, "/.history");

    FILE* hist_file = fopen(hist_file_path, "r");

    if (hist_file == NULL) {
        fprintf(stderr, "History File Couldn't be opened\n");
        return 1;
    }

    char** commands = (char**)malloc(1000*sizeof(char*));
    int i = 0;
    int length = 0;

    commands[i] = (char*)malloc(1000);
    while (fgets(commands[i], 1000, hist_file) != NULL) {
        i++;
        commands[i] = (char*)malloc(1000);
    }

    int j = i - 1;
    int n = 10;

    if (args[1] != NULL && strcmp(args[1], "-n") == 0) {
        n = atoi(args[2]);
    } 

    for (int k = 0; k < n && j >= 0; k++) {
        printf("%s", commands[j--]);
    }


    free(commands);


}