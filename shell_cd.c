int shell_cd(char **args, char* root);

int shell_cd(char** args, char* root) {
    char* cwd = (char*)malloc(8192);
    getcwd(cwd, 8192);

    if (strcmp(args[1], "..") == 0) {

        char* prev_dir = (char*)malloc(8192);
            
        int length = strlen(cwd);
        int i = length;
        for (i = length - 1; i >=0; i--) {
            if (cwd[i] == '/') {
                break;
            }
        }

        for (int j = 0; j <= i; j++) {
            prev_dir[j] = cwd[j];
        }

        chdir(prev_dir);
        free(prev_dir);
    }

    else if (strcmp(args[1], ".") == 0) {
        chdir(cwd);
    }

    else if (args[1][0] == '.' && args[1][1] == '/') {
        char* act_path = (char*)malloc(8192);
        act_path = cwd;
        strcat(act_path, "/");
        int act_path_length = strlen(act_path);
        for (int i = 2; args[1][i] != '\0'; i++) {
            act_path[act_path_length++] = args[1][i];
        }

        int error = chdir(act_path);

        if (error < 0) {
            perror("r1");
        }

        free(act_path);
    }

    else if (args[1][0] == '.' && args[1][1] == '.' && args[1][2] == '/') {
        char* prev_dir = (char*)malloc(8192);
            
        int length = strlen(cwd);
        int i = length;
        for (i = length - 1; i >=0; i--) {
            if (cwd[i] == '/') {
                break;
            }
        }

        for (int j = 0; j <= i; j++) {
            prev_dir[j] = cwd[j];
        }

        int prev_dir_length = strlen(prev_dir);

        for (int i = 3; args[1][i] != '\0'; i++) {
            prev_dir[prev_dir_length++] = args[1][i];
        }

        int error = chdir(prev_dir);
        if (error < 0) {
            perror("error");
        }
        free(prev_dir);
    }

    else if (args[1][0] == '~' && args[1][1] == '/') {
        char* act_path = (char*)malloc(8192);
        strcat(act_path, root);
        strcat(act_path, "/");

        int act_path_len = strlen(act_path);

        for (int i = 2; args[1][i] != '\0'; i++) {
            act_path[act_path_len++] = args[1][i];
        }

        int error = chdir(act_path);
        if (error < 0) {
            perror("Error");
        }
        free(act_path);
    }


}