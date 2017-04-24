/*

@author Hasan Kamal

*/

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>

#include "2015039_Shell1_3.c"
#include "2015039_Shell1_4.c"

#define STR_LEN 100
#define MAX_NUM_ARGUMENTS 100

void hksh_interrupted(int sig){
	// puts("interrupted");
	// exit(0);
}

int main(){

	//set interrupt handler
	struct sigaction action;
	action.sa_handler = hksh_interrupted;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	//sigaction(SIGINT, &action, NULL);
	signal(SIGINT, SIG_IGN);

	restore_history();

	char command[STR_LEN];
	char curr_dir[STR_LEN];
	while(1){

		//get current working directory
		getcwd(curr_dir, STR_LEN);
		
		char *t, *last_dir;
		t = strtok(curr_dir, "/");
		while(t){
			last_dir = t;
			t=strtok(NULL, "/");
		}

		//show prompt
		printf("hksh: %s> ", last_dir);
		fgets(command, STR_LEN, stdin);
		command[strlen(command)-1] = '\0';
		add_entry_history(command);

		//parsing
		parse_input(strdup(command));
		//show_input_table();

		char *cmd = strdup(command);
		cmd = strtok(cmd, " ");

		int is_built_in = 0;
		int i = 0;
		for(i=0; i<NUM_BUILT_IN_COMMANDS; i++){
			if(strcmp(cmd, built_ins[i].command_name)==0){
				is_built_in = 1;
				break;
			}
		}

		if(is_built_in){
		
			//is a built-in command
			(*built_ins[i].func)(command);
		
		}else{

			//open files
			FILE *fin, *fout;
			if(table.has_infile){
				fin = fopen(table.infile, "r");
				if(!fin)
					puts("error opening infile");
			}

			if(table.has_outfile){
				fout = fopen(table.outfile, "w");
				if(!fout)
					puts("error opening outfile");
			}

			//not a built-in command
			// int rc = fork();
			// if(rc<0)
			// 	printf("fork failed\n");
			// else if(rc==0){
			// 	//child process

			// 	if(table.has_infile){
			// 		if(dup2(fileno(fin), 0)==-1)
			// 			puts("error redirecting input file");
			// 	}

			// 	if(table.has_outfile){
			// 		if(dup2(fileno(fout), 1)==-1)
			// 			puts("error redirecting output file");
			// 	}

			// 	command_entry *cmd_entry = &table.commands[table.num_commands-1];
			// 	int success = execvp(cmd_entry->command_args[0], cmd_entry->command_args);
			// 	printf("-hksh command not executable: %s\n", command);
			
			// }else{
			// 	int w = wait(NULL);
			// 	//printf("parent terminated\n");
			// }

			int t_in = (table.has_infile)?fileno(fin):dup(0);
			int t_out = (table.has_outfile)?fileno(fout):dup(1);
			int rc;

			int fd[2*(table.num_commands-1)];
			for(int i=0; i<table.num_commands; i++){

				if(i!=table.num_commands-1){
					pipe(fd + i*2);
				}

				rc = fork();
				if(rc<0)
					printf("fork failed\n");
				else if(rc==0){
					//child process
					dup2(t_in, 0);
					//close(fd[0]);
					if(i==table.num_commands-1)
						dup2(t_out, 1);
					else
						dup2(fd[i*2 + 1], 1);

					command_entry *cmd_entry = &table.commands[i];
					int success = execvp(cmd_entry->command_args[0], cmd_entry->command_args);
					printf("-hksh command not executable: %s\n", command);
				}

				t_in = fd[i*2 + 0];
			}
			
			//parent process to wait on last child
			int pid_status;
			//puts("about to start wait");
			if(rc>0){
				if(waitpid(rc, &pid_status, 0)==-1)
					puts("error waiting for last child");
			}
			//puts("wait completed");

		}
	}

	save_history();

	return 0;
}