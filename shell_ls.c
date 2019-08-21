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

char filetype(mode_t mode) {
    char c;
    switch(mode & __S_IFMT) {
        case __S_IFBLK:
            c = 'b';
            break;
        case __S_IFCHR:
            c = 'c';
            break;
        case __S_IFDIR:
            c = 'd';
            break;
        case __S_IFIFO:
            c = 'p';
            break;
        case __S_IFLNK:
            c = 'l';
            break;
        case __S_IFREG:
            c = '-';
            break;
        case __S_IFSOCK:
            c = 's';
            break;
        default:
            c = '?';
            break;
        }
    return (c);
}

char* getMonth(int mon) {
    if (mon == 0) {
        return "January";
    }
    else if (mon == 1) {
        return "February";
    }
    else if (mon == 2) {
        return "March";
    }
    else if (mon == 3) {
        return "April";
    }
    else if (mon == 4) {
        return "May";
    }
    else if (mon == 5) {
        return "June";
    }
    else if (mon == 6) {
        return "July";
    }
    else if (mon == 7) {
        return "August";
    }
    else if (mon == 8) {
        return "September";
    }
    else if (mon == 9) {
        return "October";
    }
    else if (mon == 10) {
        return "November";
    }
    else if (mon == 11) {
        return "December";
    }
}

void printInfo(char* name) {
    struct stat f_stat;
    // lstat()
    lstat(name, &f_stat);   

    fprintf(stdout, "%c", filetype(f_stat.st_mode));

    fprintf(stdout, (f_stat.st_mode & S_IRUSR) ? "r":"-");
    fprintf(stdout, (f_stat.st_mode & S_IWUSR) ? "w":"-");
    fprintf(stdout, (f_stat.st_mode & S_IXUSR) ? "x":"-");

    fprintf(stdout, (f_stat.st_mode & S_IRGRP) ? "r":"-");
    fprintf(stdout, (f_stat.st_mode & S_IWGRP) ? "w":"-");
    fprintf(stdout, (f_stat.st_mode & S_IXGRP) ? "x":"-");

    fprintf(stdout, (f_stat.st_mode & S_IROTH) ? "r":"-");
    fprintf(stdout, (f_stat.st_mode & S_IWOTH) ? "w":"-");
    fprintf(stdout, (f_stat.st_mode & S_IXOTH) ? "x\t":"-\t");

    fprintf(stdout, "%d\t", f_stat.st_nlink);

    struct passwd *own = getpwuid(f_stat.st_uid);
    struct group *grp = getgrgid(f_stat.st_gid);

    fprintf(stdout, "%s\t", own->pw_name);
    fprintf(stdout, "%s\t", grp->gr_name);
    fprintf(stdout, "%ld\t", f_stat.st_size);

    struct tm* tim;

    tim = localtime(&f_stat.st_mtime);
    fprintf(stdout, "%s\t", getMonth(tim->tm_mon));
    fprintf(stdout, "%d:", tim->tm_mday);
    fprintf(stdout, "%d\t", tim->tm_hour);
    if (tim->tm_min < 10) {
        fprintf(stdout, "0");
    }
    fprintf(stdout, "%d\t", tim->tm_min);

}

int printDetails(char* name, int flag_a, int flag_l) {
    struct stat f;
    int s = stat(name, &f);

    if (s < 0) {
        fprintf(stderr, "File or Directory Does Not exist\n");
        return 0;
    }

    if (S_ISDIR(f.st_mode)) {
        struct dirent *directory;
        DIR* directories = opendir(name);

        directory = readdir(directories);

        char* filepath = (char*)malloc(8192);

        while (directory) {
            char* filename = directory->d_name;

            if (filename[0] != '.' || flag_a == 1) {
                if (flag_l == 1) {
                    sprintf(filepath, "%s/%s", name, filename);
                    printInfo(filepath);
                }
                printf("%s\n", filename);

            }

            directory = readdir(directories);
        }
        closedir(directories);

    }
    else {
        if (flag_l == 1) {
            printInfo(name);
        }
        fprintf(stdout, "%s\n", name);
    }
}

int shell_ls(char** args, char* root) {
    int flag_a = 0, flag_l = 0;
    int file_read = 0;

    for (int i = 1; args[i] != NULL; i++) {
        if (args[i][0] == '-' && !file_read) {
            int j = 0;

            while (args[i][j] != '\0') {
                if (args[i][j] == 'a') {
                    flag_a = 1;
                }
                if (args[i][j] == 'l') {
                    flag_l = 1;
                }
                j++;
            }

            if (strlen(args[i]) == 1) {
                file_read = 1;
                // perror("Error");
                fprintf(stderr, "File or Directory Does Not exist\n");
            }
        }

        else {
            file_read = 1;
            printDetails(args[i], flag_a, flag_l);
        }

    }   

    if (file_read == 0) {
        printDetails(".", flag_a, flag_l);
    }

    // printf("%d\t%d\n", flag_a, flag_l);

}
