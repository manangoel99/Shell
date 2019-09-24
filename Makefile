LDFLAGS=-lncurses

shell: main.o shell_setenv.o shell_cd.o shell_echo.o shell_history.o shell_ls.o shell_nightswatch.o shell_pinfo.o shell_pwd.o getCommand.o pathManip.o run_command.o utils.o
	gcc main.o shell_setenv.o shell_cd.o shell_echo.o shell_history.o shell_ls.o shell_nightswatch.o shell_pinfo.o shell_pwd.o getCommand.o pathManip.o run_command.o utils.o -o shell $(LDFLAGS) 

main.o: main.c
	gcc -c main.c

shell_cd.o: shell_cd.c
	gcc -o shell_cd.o -c shell_cd.c

shell_ls.o: shell_ls.c
	gcc -o shell_ls.o -c shell_ls.c

shell_echo.o: shell_echo.c
	gcc -o shell_echo.o -c shell_echo.c

shell_history.o: shell_history.c
	gcc -o shell_history.o -c shell_history.c

shell_nightswatch.o: shell_nightswatch.c
	gcc -o shell_nightswatch.o -c shell_nightswatch.c

shell_pinfo.o: shell_pinfo.c
	gcc -o shell_pinfo.o -c shell_pinfo.c

shell_pwd.o: shell_pwd.c
	gcc -o shell_pwd.o -c shell_pwd.c

getCommand.o: getCommand.c
	gcc -o getCommand.o -c getCommand.c

pathManip.o: pathManip.c
	gcc -o pathManip.o -c pathManip.c

run_command.o: run_command.c
	gcc -o run_command.o -c run_command.c

utils.o: utils.c
	gcc -o utils.o -c utils.c

shell_setenv.o: shell_setenv.c
	gcc -o shell_setenv.o -c shell_setenv.c

clean:
	rm -rf shell *.o