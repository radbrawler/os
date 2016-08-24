#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
	int i=0;
	for (i=5;i>=0;i--) {
		printf("%d seconds left\n", i);
		sleep(1);
	}
}
