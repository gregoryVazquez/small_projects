#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include<stdlib.h>

char *handler_msg = "\nProg. recv. SIGINT...\n";

void catch_ctrl_c(int signo) {
	write(STDERR_FILENO, handler_msg, strlen(handler_msg));
	sleep(1);
	fprintf(stderr, "Leaving...\n");
	exit(EXIT_FAILURE);
}

void main(void) 
{
	struct sigaction act;
	act.sa_handler = catch_ctrl_c;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGINT, &act, NULL);

	while(1) {
		fprintf(stdout, "Waiting for SIGINT....\n");
		sleep(9);
	}
}

