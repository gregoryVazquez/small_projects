#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>

void main(int argc, char **argv)
{
	char *string = argv[1];
	write(STDOUT_FILENO, string, strlen(string));
}


