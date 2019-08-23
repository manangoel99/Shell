#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

char** SplitCommand(char* command_str);

char* getCommands();