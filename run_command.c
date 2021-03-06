#include "shell.h"
#include "run_command.h"
#include "shell_ls.h"
#include "shell_echo.h"

int running_proc_num = 0;
struct p processes[10000];
pid_t current_running_proc;

char** redirectCode(char** args, int* isin_p, int* isout_p, int* infd_p, int* outfd_p, int* pos_p);

int run_command(char **args,char* root) {

    int background = 0, i = 0, j = 0;

    pid_t pid;

    while(args[i] != NULL) {
        j = 0;
        while(args[i][j] != '\0') {
            if (args[i][j] == '&') 
                background = 1;
            j++;
        }

        i++;
    }

    char **args1 = (char**)malloc(sizeof(char*) * 100);
    
    if (background == 1) {
        for (int k = 0; args[k + 1] != NULL; k++) {
            args1[k] = args[k];
        }
        args = args1;
    }
    pid = fork();
    current_running_proc = pid;

    if (pid < 0) {
        perror("Error");
    }
    else if (pid == 0) {
        if (background == 1) {
            if (setpgid(0, 0) == 0) {}
            else {
                perror("Can't Start Background Process");
                exit(1);
            }
        }
        int pipe_flag = 0;
        char*** all_args = (char***)malloc(sizeof(char**) * 128);

        int args_pos = 0, position = 0, init_position = 0;
        int i = 0, j = 0, k = 0, ll;

        while (args[i] != NULL) {
            if (strcmp(args[i], "|") == 0) {
                pipe_flag = 1;

                ll = position - init_position;
                char** kwargs = (char**)malloc(sizeof(char*) * 128);

                j = init_position;
                while (j < init_position + ll) {
                    kwargs[k++] = args[j++];
                }
                kwargs[k++] = 0;
                all_args[args_pos++] = kwargs;
                init_position = position + 1;
            }
            position++;
            i++;
        }
        ll = position - init_position;
        char** kwargs = (char**)malloc(sizeof(char*) * 512);
        j = init_position;
        k = 0;
        while (j < init_position + ll) {
            kwargs[k++] = args[j++];
        }
        kwargs[k++] = 0;
        all_args[args_pos++] = kwargs;
        
        if (pipe_flag == 0) {

            int isin_p, isout_p, infd_p, outfd_p, pos_p;

            char** kwargs = redirectCode(args, &isin_p, &isout_p, &infd_p, &outfd_p, &pos_p);

            if (isin_p) {
                dup2(infd_p, 0);
            }
            if (isout_p) {
                dup2(outfd_p, 1);
            }
            
            if (strcmp(kwargs[0], "ls") == 0) {
                shell_ls(kwargs, root);
            }
            else if (strcmp(kwargs[0], "echo") == 0) {
                shell_echo(kwargs, root);
            }
            else {
                if (execvp(kwargs[0], kwargs) == -1) {
                    perror("Piping Error");
                }
            }

            

            if (isin_p) {
                close(infd_p);
            }
            if (isout_p) {
                close(outfd_p);
            }
        }
        else {
            int i = 0;
            while (i < args_pos - 1) {
                int act_pipe[2];
                int status = pipe2(act_pipe, 0);
                    pid_t np = fork();

                    if (np == 0) {
                        dup2(act_pipe[1], 1);

                        int isin_p, isout_p, infd_p, outfd_p, pos_p;

                        char** kwargs = redirectCode(all_args[i], &isin_p, &isout_p, &infd_p, &outfd_p, &pos_p);

                        if (isin_p) {
                            dup2(infd_p, 0);
                        }
                        if (isout_p) {
                            dup2(outfd_p, 1);
                        }

                        if (strcmp(kwargs[0], "ls") == 0) {
                            shell_ls(kwargs, root);
                        }
                        else if (strcmp(kwargs[0], "echo") == 0) {
                            shell_echo(kwargs, root);
                        }
                        else {
                            if (execvp(kwargs[0], kwargs) == -1) {
                                perror("Piping Error");
                            }
                        }
                        if (isin_p) {
                            close(infd_p);
                        }
                        if (isout_p) {
                            close(outfd_p);
                        }
                        abort();

                    }
                    dup2(act_pipe[0], 0);
                    close(act_pipe[1]);

                i++;
            }
            int isin_p, isout_p, infd_p, outfd_p, pos_p;

            char** kwargs = redirectCode(all_args[i], &isin_p, &isout_p, &infd_p, &outfd_p, &pos_p);
            
            if (isin_p) {
                dup2(infd_p, 0);
            }
            if (isout_p) {
                dup2(outfd_p, 1);
            }
            
            if (strcmp(kwargs[0], "ls") == 0) {
                shell_ls(kwargs, root);
            }
            else if (strcmp(kwargs[0], "echo") == 0) {
                shell_echo(kwargs, root);
            }
            else {
                if (execvp(kwargs[0], kwargs) < 0) {
                    perror("Error");
                }
            }
            if (isin_p) {
                close(infd_p);
            }
            if (isout_p) {
                close(outfd_p);
            }


        }
        for (int d = 0; d < args_pos; d++) {
            free(all_args[d]);
        }
        free(all_args);
        exit(1);

    }
    else if (pid > 0) {
        int state;
        if (background == 0) {
            pid_t w = waitpid(-1, NULL, WUNTRACED);
        }
        else {
            char* command = (char*)malloc(1280);
            strcpy(command, "");

            int i = 0;
            while(args[i] != NULL) {
                strcat(command, args[i]);
                strcat(command, " ");
                i++;
            }

            processes[running_proc_num].pname = (char*)malloc(sizeof(command));
            processes[running_proc_num].pname = command;
            processes[running_proc_num].status = 0;
            processes[running_proc_num].print_status = 0;
            processes[running_proc_num].stat = 1;
            // strcpy(processes[running_proc_num].pname, args[0]);
            processes[running_proc_num++].pid = pid;
            kill(pid, SIGTTIN);
            kill(pid, SIGCONT);
        }
    }
    

}

char** redirectCode(char** args, int* isin_p, int* isout_p, int* infd_p, int* outfd_p, int* pos_p) {
    int isin = 0, isout = 0, infd = 0, outfd = 1;

    char** kwargs = (char**)malloc(sizeof(char*) * 128);

    int position = 0, kwargs_position = 0, i, counter;

    while (args[i] != NULL) {
        i++;
        counter++;
    }

    i = 0;
    
    while (args[i] != NULL) {
        if (strcmp(args[i], "<") == 0) {
            isin = 1;
            if (counter > i + 1) {
                infd = open(args[++i], O_RDONLY);
                if (infd < 0) {
                    perror("File Not Found");
                    exit(1);
                }
            }
            else {
                perror("No input file");
                exit(1);
            }
        }
        else if (strcmp(args[i], ">") == 0) {
            isout = 1;
            if (counter > i + 1) {
                outfd = open(args[++i], O_WRONLY | O_TRUNC | O_CREAT, 0644);
            }
            else {
                perror("Output file not given");
                exit(1);
            }
        }
        else if (strcmp(args[i], ">>") == 0) {
            isout = 1;
            if (counter > i + 1) {
                outfd = open(args[++i], O_WRONLY | O_APPEND | O_CREAT, 0644);
            }
            else {
                perror("Output file not given");
                exit(1);
            }
        }
        else {

            kwargs[kwargs_position++] = args[i];

        }
        i++;
        position++;
    }

    *isin_p = isin;
    *isout_p = isout;
    *infd_p = infd;
    *outfd_p = outfd;
    *pos_p = position;

    return kwargs;
}