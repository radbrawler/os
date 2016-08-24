#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int tfd = open("temp.txt", O_WRONLY | O_APPEND | O_CREAT, 0777);
	printf("tfd = %d STDIN_FILENO = %d\n", tfd, STDIN_FILENO);

	dup2(tfd, 1);

	printf("tfd = %d STDIN_FILENO = %d\n", tfd, STDIN_FILENO);

	return 0;
}