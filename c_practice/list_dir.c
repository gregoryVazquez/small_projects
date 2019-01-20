#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<errno.h>

void main(int argc, char *argv[])
{
	DIR *dirp;
	struct dirent *direntp;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s directory_name\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	if ((dirp = opendir(argv[1])) == NULL) {
		fprintf(stderr, "Couldn't open %s directory: %s\n", argv[1], strerror(errno));
		exit(EXIT_FAILURE);
	}

	while ((direntp = readdir(dirp)) != NULL) 
		printf("%s\n", direntp->d_name);

	closedir(dirp);
	exit(EXIT_SUCCESS);
}

