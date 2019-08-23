#include "getCommand.h"
#define ll long long

char** SplitCommand(char* command_str) {

    char** tokens = (char**)malloc(sizeof(char*) * 512);

    char* token = strtok(command_str, " ");

    ll i = 0;

    while (token != NULL) {
        tokens[i] = (char*)malloc(sizeof(char) * 512);
        tokens[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    tokens[i] = NULL;
    return tokens;

}


char* getCommands() {
    ll str_size = 8192, pos = 0;

    char* command_str = (char*)malloc(sizeof(char) * str_size);

    while(1) {
        char character = getchar();
        if (character == '\n' || character == EOF) {
            command_str[pos] = '\0';
            break;
        }
        else {
            command_str[pos] = character;
        }
        
        pos++;

        if (pos > str_size) {
            str_size += 1024;

            command_str = realloc(command_str, str_size);
        }
    }
    return command_str;
}