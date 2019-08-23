#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>

int nightswatch(char** args, char* root) {
    int args_len = 0;
    int i = 0;
    while(args[i] != NULL) {
        args_len++;
        i++;
    }

    int start_time = time(NULL);

    if (args_len == 1) {
        fprintf(stderr, "nightswatch: Use nightswatch [options] <valid command>\nHere valid commands are 'interrupt' and 'dirty'\n");
        return -1;
    }

    int num_secs = 2;

    int opt_index = 0;

    if (args[1] != NULL && strcmp(args[1], "-n") == 0) {
        num_secs = atoi(args[2]);
        opt_index = 3;
    }
    else {
        opt_index = 1;
    }
    if (args[opt_index] != NULL) {
		printf("%s\n", args[opt_index]);
	    WINDOW* curr = initscr();
	    WINDOW * win; 

		int flag_interrupt = 0, flag_dirty = 0;

		if (strcmp(args[opt_index], "interrupt") == 0) {
			flag_interrupt = 1;
		}

		if (strcmp(args[opt_index], "dirty") == 0) {
			flag_dirty = 1;
		}

	    win = newwin(1280,720,1,1);

	    noecho();
	    keypad(win, TRUE);
	    curs_set(0);
	    nodelay(win,1);

	    int start_time = time(NULL) , prev_time = time(NULL);
	    
	    size_t cpu_size;
	    char *buffer;
	    char* heading = (char*)malloc(200);

		char* file_path;

		if (flag_interrupt == 1) {

			sprintf(heading, "NIGHTSWATCH INTERRUPT: %ds\n", num_secs);
			file_path = (char*)malloc(sizeof("/proc/interrupts"));
        	strcpy(file_path, "/proc/interrupts");

		}

		if (flag_dirty == 1) {
			sprintf(heading, "NIGHTSWATCH DIRTY: %ds\n", num_secs);
			file_path = (char*)malloc(sizeof("/proc/meminfo"));
        	strcpy(file_path, "/proc/meminfo");
		}

        int line_nums = 5;

	    char *cpuinfo;

	    FILE* fd = fopen(file_path,"r");
	    getline(&cpuinfo, &cpu_size, fd);
	    fclose(fd);

		mvwaddstr(win, 1, 5, heading);
	    mvwaddstr(win, 3, 5,cpuinfo);

	    for (int z = 0;;z++)
	    {
	    	int current_time = time(NULL);
	    	
	    	if((current_time - start_time) % num_secs == 0 && current_time != prev_time)
	    	{ 
	    		char *data;
	    		FILE* fd1 = fopen(file_path, "r");
				prev_time = current_time;
				line_nums++;
	    		size_t nowbufsize = 0;

	    		fseek(fd1, 0, SEEK_SET);
	    		// getline(&data, &nowbufsize, fd1);		
	    		// getline(&data, &nowbufsize, fd1);
				int m = 0;
				int times = 0;

				if (flag_interrupt == 1)
					times = 3;
				else if (flag_dirty == 1)
					times = 17;

				while (m < times) {
		    		getline(&data, &nowbufsize, fd1);
					m++;
				}
		
	    		mvwaddstr(win, line_nums, 5, data);
	    		fclose(fd1);

	    	}
	    	if(line_nums > 30) {
	    		line_nums = 5;
	    	}
			mvwaddstr(win, 1, 5, heading);
	    	mvwaddstr(win, 3, 5,cpuinfo);
	    	wrefresh(win);
			if(wgetch(win) == 'q') {
	    		break;
	    	}
	    }
	    noecho();
	    curs_set(1);
	    delwin(win);
	    endwin();
        return 1;
    }

    else {
        fprintf(stderr, "nightswatch: Use nightswatch [options] <valid command>\nHere valid commands are 'interrupt' and 'dirty'\n");
        return -1;
    }

}