#include <stdio.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>

#include "shell.h"

struct p processes[10000];

int jobs(char** args, char* root);
int overkill(char** args, char* root);
int run_fg(char** args, char* root);