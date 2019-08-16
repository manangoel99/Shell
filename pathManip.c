#include <string.h>

char* getPath(char* cwd, char* root) {
    int root_size = strlen(root), cwd_size = strlen(cwd);

    char* path = (char*)malloc(8192);

    if (root_size > cwd_size) {
        path[0] = '~';
        path[1] = '/';

        for (int i = 0; i < root_size - cwd_size - 1; i++) {
            path[i + 2] = cwd[root_size + i + 1];
        }
    }
    else if (root_size == cwd_size) {
        path[0] = '~';
        path[1] = '\0';
    }
    else {
        path = cwd;
    }

    return path;
}