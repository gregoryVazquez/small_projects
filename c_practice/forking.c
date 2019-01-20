#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>

pid_t parent_pid;
pid_t child_pid;


void main(void) 
{
	int wstatus;
	if ((child_pid = fork()) == -1) {
		fprintf(stderr, "[!] Error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	} else if ( child_pid == 0 ) { //this is the child process
		parent_pid = getppid();
		fprintf(stderr, "Child:%ld of process %ld executing loop ... \n", (long)getpid(), (long)parent_pid);
		for (int i = 0; i < 10; i++) sleep(1);
		fprintf(stderr, "Child of process finished\n");
	} else { //parent process
		fprintf(stderr, "This is the parent process %ld waiting ..\n", (long)getpid());
		wait(&wstatus);
	}
	printf("This is process %ld exiting\n", (long)getpid());
	exit(EXIT_SUCCESS);
}


