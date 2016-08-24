#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int command_t = 0;

struct command {
	char *arguments[64];
	int argument_count;
} command_list[50];

void init() {
	command_t = 0;

	int i = 0, j = 0;
	for (i=0; i<50; i++) {
		for (j=0; j<64; j++) {
			command_list[i].arguments[j] = '\0';
		}
		command_list[i].argument_count = 0;
	}
}

void parse1(char *line)
{
	int current_command = 0, current_argument = 0, i = 0;

	char *commands[50], *end_str, delimit[]="|\n";

	commands[0] = strtok_r(line, delimit, &end_str);

	while (commands[i] != NULL) {
		char *end_token;
		char *token = strtok_r(commands[i], " ", &end_token);

		while (token != NULL) {

			// if (strcmp(token, "|")!=0 && strcmp(token, ">")!=0 && strcmp(token, "<")!=0 
			// 	&& strcmp(token, ">>")!=0 && strcmp(token, "&")!=0 ) {

			if (strcmp(token, "|")!=0) {
				
				command_list[current_command].arguments[current_argument++] = token;
				command_list[current_command].argument_count = current_argument;
			
			} 
			token = strtok_r(NULL, " ", &end_token);
		}
		current_command++;
		current_argument = 0;
		i++;
		commands[i] = strtok_r(NULL, "|", &end_str);
    }
    command_t = current_command;
}

void parse(char *line, char **argv)
{
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *argv = '\0';                 /* mark the end of argument list  */
}

void print_all_commands() {
	int i = 0, j = 0 , n;
    n= command_list[i].argument_count;
	for (i=0; i<50; i++) {
		if (command_list[i].argument_count != 0) {
			//printf(" %d \n " ,i);
			printf("Argument Count %d\n", command_list[i].argument_count);
			for (j=0; j<64; j++) {
				if (command_list[i].arguments[j] != NULL)
					printf("Command is %s \n", command_list[i].arguments[j]);
			}
		}
	}
}

// void combined_commands(char *line, char **commands)

void sh_execute(char **argv)
{
	pid_t pid;
	int status;
	pid = fork();

	if (pid < 0)
	{
		fprintf(stderr, "Forking error");
		exit(1);
	} else if (pid == 0) { 	// Child process
		if (strcmp (argv[1], "\0") != 0) {
			// printf("*** In first ***\n");
			if (execlp(argv[0], argv[0], argv[1], NULL) == -1) {
				printf("*** ERROR: execlp failed 1***\n");
				exit(1);
			}			
		} else {
			printf("*** In second ***\n");
			if (execlp(argv[0], argv[0], NULL) == -1) {
				printf("*** ERROR: execlp failed ***\n");
				exit(1);
			}
		}	
	
	} else { // parent process
		//while (wait(&status) != pid)       /* wait for completion  */
               ;
		//printf("Child Complete\n");
        printf("In parent\n");
	}
	//printf("In the end\n");
}

void sh_execute1(int t){
     int i=0;
     pid_t pid;
     pid=fork();
     int status;

    // printf("in pipe execution process id = %d \n", getpid() );
	if (pid < 0)
	{
		fprintf(stderr, "Forking error");
		exit(1);
	} else if (pid == 0) { 	// Child process
		if ((command_list[0].argument_count >=2)) {
			// printf("*** In first ***\n");
			if (execlp(command_list[0].arguments[0], command_list[0].arguments[0], command_list[0].arguments[1], NULL) == -1) {
				printf("*** ERROR: execlp failed ***\n");
				exit(1);
			}			
		} else {
			// printf("*** In second *** %s", command_list[0].arguments[1]);
			if (execlp(command_list[0].arguments[0], command_list[0].arguments[0], NULL) == -1) {
				printf("*** ERROR: execlp failed ***\n");
				exit(1);
			}
		}	
	
	} else { // parent process
		// printf("Waiting for Child");
		int ppid = wait(&status);       /* wait for completion  */
		// printf("%d\n", ppid);
	}
}

void sh_execute2(int t, int fd[]){
    int i=0;
    pid_t pid;
    pid=fork();
    int status;
    char buf;

    // printf("in pipe execution process id = %d \n", getpid() );
	if (pid < 0)
	{
		fprintf(stderr, "Forking error");
		exit(1);
	} else if (pid == 0) { 	// Child process
		dup2(fd[1], 1);
		dup2(fd[0], 0);
		if ((command_list[t].argument_count >=2)) {
			// printf("*** In first ***\n");
			if (execlp(command_list[t].arguments[0], command_list[t].arguments[0], command_list[t].arguments[1], NULL) == -1) {
				printf("*** ERROR: execlp failed ***\n");
				exit(1);
			}			
		} else {
			// printf("*** In second *** %s", command_list[0].arguments[1]);
			if (execlp(command_list[t].arguments[0], command_list[t].arguments[0], NULL) == -1) {
				printf("*** ERROR: execlp failed ***\n");
				exit(1);
			}
		}		
	} else { // parent process
		// printf("Waiting for Child");
		int ppid = wait(&status);       /* wait for completion  */
		// printf("%d\n", ppid);
      	// close(fd[1]);

       // 	while (read(fd[0], &buf, 1) > 0)
       // 		write(STDOUT_FILENO, &buf, 1);

       // 	write(STDOUT_FILENO, "\n", 1);
       // 	close(fd[0]);

	}
}

void sh_input(char **argv) {
	pid_t pid;
	int status;
	pid = fork();

	if (pid < 0)
	{
		fprintf(stderr, "Forking error");
		exit(1);
	} else if (pid == 0) { 	// Child process	
		if (execlp("bash", "bash", NULL) == -1) {
			printf("*** ERROR: execlp failed ***\n");
			exit(1);
		}
	} else { // parent process
		while (wait(&status) != pid);       /* wait for completion  */
		//printf("Child Complete\n");
	}
}

void sh_bash(char **argv) {
	char *args[2];
	args[0] = "/bin/sh";
    args[1] = "-i";
    args[2] = NULL;
    char **envp;
    execve(args[0], args, NULL);
}

int main(int argc) 
{
	 //init();

	system("clear");
	char line[1024];	// whole input line 
	char *argv[64];		// command line arguments will be here
	
	// sh_bash(argv);

	do {
		int current_command = 0;
		int fd[2]; 
		char buf;
		init();
		printf("Shell => ");
		fgets(line, sizeof(line), stdin);
		parse1(line);
		// print_all_commands();

		// if(strcmp(argv[1],"<") == 0) {
		// 	sh_input(argv);
		// }
		// sh_execute(argv);
		
		// printf("Value of variable \"command_t\" = %d\n", command_t);

		while (command_t > current_command) {
			// fd[0] is to read and fd[1] is to write
			// http://tldp.org/LDP/lpg/node11.html
			
			pipe(fd);

			sh_execute2(current_command, fd);
			//printf("reached here\n");
			//while(read(fd[0], &buf, 1) > 0){
				//write(fd[1], &buf, 1);
			//}
			//write(fd[1], "\n", 1);
			//close(fd[0]);
			//close(fd[1]);
			current_command++;
			printf("Waiting here\n");
		}
		// int new_fd = dup2(0, fd[0]);
		// int new_fd1 = dup2(1, fd[1]);
		// sh_execute1(command_t);
		
      	close(fd[1]);

       	while (read(fd[0], &buf, 1) > 0)
       		write(STDOUT_FILENO, &buf, 1);

       	write(STDOUT_FILENO, "\n", 1);
       	close(fd[0]);
	}while(strcmp(command_list[0].arguments[0], "exit") != 0);
}
