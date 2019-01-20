#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

#define DEFAULT 2

void main(int argc, char *argv[]) 
{
	double y;
	sigset_t intmask;
	int i, sleep_factor;


        sleep_factor = argc == 2? atoi(argv[1]): DEFAULT;
	// empty sigset_t struct
	sigemptyset(&intmask);
	// add SIGINT to intmask
	sigaddset(&intmask, SIGINT);

	//run loop
	//block SIGINT
	for (;;) {
		sigprocmask(SIG_BLOCK, &intmask, NULL);
		fprintf(stderr, "\n[*] Blocking SIGINT while running calculation...\n");
		for (i = 0; i < sleep_factor; i++) sleep(sleep_factor);
		fprintf(stderr, "[*] Un-blocking SIGINT...\n");
		sigprocmask(SIG_UNBLOCK, &intmask, NULL);
		fprintf(stderr, "[*] Unblocked SIGINT...\n");
		for (i = 0; i < sleep_factor; i++) sleep(sleep_factor);
		fprintf(stderr, "Unblocked calculation finished...\n");
	}

	return(EXIT_SUCCESS);
}
	
