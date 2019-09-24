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

int run_setenv(char** args, char* root);
int run_unsetenv(char** args, char* root);