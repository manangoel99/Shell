#include "shell_pinfo.h"
#include "pathManip.h"

int shell_pinfo(char** args, char* root) {
    char* pid = (char*)malloc(100);
    if (args[1] == NULL) {
        sprintf(pid, "%d", getpid());
    }
    else {
        pid = args[1];
    }

    char* proc = (char*)malloc(1000);
    sprintf(proc, "/proc/%s/stat", pid);

    FILE* stat_file = fopen(proc, "r");

    if (stat_file == NULL) {
        fprintf(stderr, "Error reading %s", proc);
    }
    int p_id;
    
    char status, *name = (char*)malloc(1000);
    fscanf(stat_file, "%d", &p_id);
    fscanf(stat_file, "%s", name);
    fscanf(stat_file, " %c", &status);

    fprintf(stdout, "pid: %d\n", p_id);
    fprintf(stdout, "Process Status: %c\n", status);
    fclose(stat_file);

    sprintf(proc, "/proc/%s/statm", pid);

    FILE* mem_file = fopen(proc, "r");
    if (mem_file == NULL) {
        fprintf(stderr, "Error reading %s", proc);
    }

    int size;
    fscanf(mem_file, "%d", &size);
    printf("Memory: %d\n", size);

    fclose(mem_file);

    sprintf(proc, "/proc/%s/exe", pid);
    char* path = (char*)malloc(100);
    int poss = readlink(proc, path, 100);

    if (poss == -1) {
        fprintf(stderr, "Executable path -- Not defined in proc\n\n");
    }
    else {
        char* relpath = (char*)malloc(100);
        relpath = getPath(path, root);

        relpath[poss] = '\0';
        fprintf(stdout, "Path : %s\n", relpath);
    }
    free(path);

}