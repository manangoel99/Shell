#include "getCommand.h"
#define ll long long
#define KEY_UP 72

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

char** SplitCommands(char* overall_str) {
    char** tokens = (char**)malloc(sizeof(char*) * 5120);
    char* token = strtok(overall_str, ";");

    ll i = 0;

    while (token != NULL) {
        tokens[i] = (char*)malloc(sizeof(char) * 512);
        tokens[i] = token;
        token = strtok(NULL, ";");
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
        // printf("%c %d\t", character, character);
        if (character == '\n' || character == EOF) {
            // printf("%c\nLOLOL\n", character);
            command_str[pos] = '\0';
            break;
        }
        if (character == 27) {
            command_str[pos] = '~';
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