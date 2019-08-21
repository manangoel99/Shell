int shell_cd(char **args, char* root);

int shell_cd(char** args, char* root) {

    char* token = strtok(args[1], "/");

    if (args[1][0] == '/') {
        int error = chdir(args[1]);
        if (error < 0) {
            perror("Error");
        }
        // printf("%s\n", token);
    }
    else {
        while (token != NULL) { 
            char* cwd = (char*)malloc(8192);
            getcwd(cwd, 8192);

            if (strcmp(token, "~") == 0) {
                chdir(root);
            }

            else if (strcmp(token, "..") == 0) {
                char* prev_dir = (char*)malloc(8192);

                int length = strlen(cwd);
                int i = length;
                for (i = length - 1; i >=0; i--) {
                    if (cwd[i] == '/') {
                        break;
                    }
                }

                for (int j = 0; j < i; j++) {
                    prev_dir[j] = cwd[j];
                }
                prev_dir[i] = '\0';

                int error = chdir(prev_dir);
                if (error < 0) {
                    perror("Error");
                }
                free(prev_dir);
            }

            else if (strcmp(token, ".") == 0) {
                chdir(cwd);
            }

            else {
                char* dir = (char*)malloc(8192);
                strcat(dir, cwd);
                strcat(dir, "/");
                strcat(dir, token);
                int error = chdir(dir);

                if (error < 0) {
                    perror("Error");
                }

            }

            free(cwd);
            token = strtok(NULL, "/"); 
        } 

    }

    
}