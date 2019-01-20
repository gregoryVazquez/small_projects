#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>
#include<limits.h>
#include<signal.h>
#define MILLION 1000000


char astbuf[] = "*";
static void myhandler(int s) {
	write(STDERR_FILENO, astbuf, sizeof(char));
}

// initialize handler for a SIGPROF interupt
void init_timer_interrupt(void) {
	struct sigaction newact;
	newact.sa_handler = myhandler;
	newact.sa_flags = SA_RESTART;
	sigemptyset(&newact.sa_mask);
	sigaction(SIGPROF, &newact, NULL);
}

// set the ITIMER_PROF to two seconds
void setup_interval_timer(void) {
	struct itimerval value;
	value.it_interval.tv_sec = 1;
	value.it_interval.tv_usec = 0;
	value.it_value = value.it_interval;
	setitimer(ITIMER_PROF, &value, NULL);
}

void calculation_to_time() {
	int i;
	for (i = 0; i < 10; i++) raise(SIGPROF);
}




void main(void)
{
	time_t tm = time(NULL);
	struct timeval tpstart, tpend;
	unsigned long difference;
	init_timer_interrupt();
	setup_interval_timer();
	fprintf(stderr, "%s",ctime(&tm));

	fprintf(stderr, "Timing calculation...\n");
	gettimeofday(&tpstart, NULL);
	calculation_to_time();
	gettimeofday(&tpend, NULL);
	difference = MILLION * (tpend.tv_sec - tpstart.tv_sec) +
				tpend.tv_usec - tpstart.tv_usec;
	fprintf(stderr, "Calculation took %ld microseconds.\n", difference);
	exit(0);
}

