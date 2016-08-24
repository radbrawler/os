#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int commands = 0;

struct command {
	char *arguments[64];
	int argument_count;
} command_list[50];

void init() {
	commands = 0;

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

	char *commands[50], *end_str;
	commands[0] = strtok_r(line, "|", &end_str);

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
	int i = 0, j = 0;
	for (i=0; i<50; i++) {
		if (command_list[i].argument_count != 0) {
			printf("%d\n", command_list[i].argument_count);
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
			printf("*** In first ***\n");
			if (execlp(argv[0], argv[0], argv[1], NULL) == -1) {
				printf("*** ERROR: execlp failed ***\n");
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
		while (wait(&status) != pid)       /* wait for completion  */
               ;
		//printf("Child Complete\n");
	}
	//printf("In the end\n");
}

void sh_execute1(){
     int i=0;
     pid_t pid;
     pid=fork();
     int status;

    printf("in pipe execution\n");
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
			printf("*** In second ***\n");
			if (execlp(command_list[0].arguments[0], command_list[0].arguments[0], NULL) == -1) {
				printf("*** ERROR: execlp failed ***\n");
				exit(1);
			}
		}	
	
	} else { 
	        // parent process
		while (wait(&status) != pid)       /* wait for completion  */
               ;

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
	// init();
	char line[1024];	// whole input line 
	char *argv[64];		// command line arguments will be here
	
	// sh_bash(argv);

	do {
		init();
		printf("Shell => ");
		fgets(line, sizeof(line), stdin);
		parse1(line);
		print_all_commands();

		// if(strcmp(argv[1],"<") == 0) {
		// 	sh_input(argv);
		// }
		
		while (commands > 1) {
			sh_execute(command_list[commands].arguments);
		}
		sh_execute(command_list[commands].arguments);
		
	}while(strcmp(command_list[0].arguments[0], "exit"));
}
