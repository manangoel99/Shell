#include <string.h>

char* getPath(char* cwd, char* root) {
    int root_size = strlen(root), cwd_size = strlen(cwd);

    char* path = (char*)malloc(8192);

    if (strstr(root, cwd) && root_size != cwd_size) {
        path = cwd;
    }
    else if (strstr(cwd, root) && root_size == cwd_size) {
        path[0] = '~';
        path[1] = '\0';

    }
    else if (strstr(cwd, root) && root_size != cwd_size) {
        path[0] = '~';
        path[1] = '/';
        int i;
        for (i = 0; i < cwd_size - root_size - 1; i++) {
            path[i + 2] = cwd[root_size + i + 1];
        }
        path[i + 2] = '\0';
    }
    else {
        path = cwd;
    }

    return path;
}