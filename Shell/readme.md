command for compilation :make -f makefile
makefile has command to compile the c files. 
headers.h contains all the header files related to the code.
main.c contains main functions.
loop.c contains the main loop of the execution of shell.
exec.c contains all the functions used in the while loop of loop.c
loop.h has the functions used in exec.c
some minor functions which are used by functions in exec.c are written in exec.c which are not listed in loop.h
exec.c has
1)proc fuction which handle pipes,input output redirections and processes
2)bgtofg function bring the background process to foreground.
3)fgtobg make the stopped background process run.
4)jobs print the background process,it pids and numbering according to their time of opening.
5)killbgs refer to overkill command which kills all background processes. 