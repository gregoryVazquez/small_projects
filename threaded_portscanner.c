#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<ctype.h>
#include<string.h>
#include<pthread.h>

#define er_handle(msg) do { perror(msg); exit(EXIT_FAILURE); } while(0)

#define BUFSIZE 30
#define WELL_KNOWN_PORTS 1 
#define REGISTERED_PORTS 2
#define ALL_PORTS 4


struct scan_info {
	unsigned int start;
	unsigned int middle;
	unsigned int end;
	char ip[BUFSIZE];
};
typedef struct scan_info scan_info_t;


void __set_flags(scan_info_t*, int);
void __scan(scan_info_t*);
void run_scan(char*, int);


int main(int argc, char *argv[]) {
	run_scan(argv[1], atoi(argv[2]));
	return 0;
}


void run_scan(char* var, int ports) {
	scan_info_t scaninfo, scaninfo_two;
	pthread_t tid_one, tid_two;
	//set ports to scan
	__set_flags(&scaninfo, ports);
	
	if (strlen(var) > BUFSIZE) er_handle("input invalid...");
	
	strcpy(scaninfo.ip, var);
	strcpy(scaninfo_two.ip, scaninfo.ip);
	scaninfo_two.start = scaninfo.middle;
	scaninfo_two.middle = 0;
	scaninfo_two.end = scaninfo.end;
	scaninfo.end = scaninfo.middle;
	
	
	if (pthread_create(&tid_one, NULL, &__scan, &scaninfo) != 0)
		er_handle("creating thread one");
	if (pthread_create(&tid_two, NULL, &__scan, &scaninfo_two) != 0)
		er_handle("creating thread two");
	
	pthread_join(tid_one, NULL);
	pthread_join(tid_two, NULL);
	
	return;
}


void __set_flags(scan_info_t* scaninfo, int ports) {
	
	switch(ports) {
	
		case WELL_KNOWN_PORTS:
			scaninfo->start = 1;
			scaninfo->middle = 511;
			scaninfo->end = 1023;
			printf("WELL_KNOWN_PORTS %d - %d selected for scanning\n",scaninfo->start, scaninfo->end);
			break;
		case REGISTERED_PORTS:
			scaninfo->start = 1024;
			scaninfo->middle = 24245;
			scaninfo->end = 49515;
			printf("REGISTERED_PORTS %d - %d selected for scanning\n",scaninfo->start, scaninfo->end);
			break;
		case WELL_KNOWN_PORTS|REGISTERED_PORTS:
			scaninfo->start = 1;
			scaninfo->middle = 24757;
			scaninfo->end = 49515;
			printf("WELL_KNOWN_PORTS|REGISTERED_PORTS %d - %d selected for scanning\n",scaninfo->start, scaninfo->end);
			break;	
		case ALL_PORTS:
			scaninfo->start = 1;
			scaninfo->middle = 32767;
			scaninfo->end = 65535;
			printf("ALL_PORTS %d - %d selected for scanning\n", scaninfo->start, scaninfo->end);
			break;
	}
	return;
}


void __scan(scan_info_t* scaninfo) {
	struct hostent *host_addr_info;
	struct in_addr *addr;
	struct sockaddr_in tgt_addr;
	struct timeval timeout;
	int skt;


	if (isalpha(scaninfo->ip[1])) {
		host_addr_info = gethostbyname(scaninfo->ip);
        	if (host_addr_info == NULL) er_handle("In host_addr_info");
		else {
			addr = (struct in_addr*) host_addr_info->h_addr;
			tgt_addr.sin_addr = *addr;
		}		
    	} else {
		inet_pton(AF_INET, scaninfo->ip, &(tgt_addr.sin_addr)); 
	}
	
	timeout.tv_sec = 1;
	timeout.tv_usec = 5;
	tgt_addr.sin_family = AF_INET;

	while(scaninfo->start < scaninfo->end) {
		if ((skt = socket(PF_INET, SOCK_STREAM, 0)) == -1)
			er_handle("in socket");
		setsockopt(skt, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));	
		tgt_addr.sin_port = htons(scaninfo->start);
		memset(&(tgt_addr.sin_zero), '\0', 8);
		

		if (connect(skt, (struct sockaddr*)&tgt_addr, sizeof(struct sockaddr)) == -1 ) {
			//
		} else {
			printf("[*] Port %d is open!\n", scaninfo->start);
		}
		
		close(skt);
		scaninfo->start++;
	}
	return;
}
