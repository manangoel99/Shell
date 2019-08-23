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

char filetype(mode_t mode);
char* getMonth(int mon);
void printInfo(char* name);
int printDetails(char* name, int flag_a, int flag_l);
int shell_ls(char** args, char* root);