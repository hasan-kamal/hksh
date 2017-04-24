/*

@author Hasan Kamal

*/

#include <stdio.h>
#include <string.h>
#include "2015039_Shell1_2.c"
#define NUM_BUILT_IN_COMMANDS 4
#define BUILT_IN_COMMAND_STR_LEN 10
#define MAX_HELP_DESCRIPTION_LEN 100

//structure to encapsulate a built-in command
typedef struct{
	char command_name[BUILT_IN_COMMAND_STR_LEN];
	void (*func)(char *command);
	char description_help[MAX_HELP_DESCRIPTION_LEN];
} built_in_command;

void help(char *command);
void cd(char *command);
void history(char *command);
void exit_builtin(char *command);

//list of all the available built-in commands
built_in_command built_ins[NUM_BUILT_IN_COMMANDS] = {
	{"help", &help, "lists commands available in this shell"},
	{"cd", &cd, "change current directory"},
	{"history", &history, "show history of previously entered commands"},
	{"exit", &exit_builtin, "exit the shell"}
};

void help(char *command){
	printf("\nhelp--\n");
	for(int i=0; i<NUM_BUILT_IN_COMMANDS; i++)
		printf("%d-%s : %s\n", i+1, built_ins[i].command_name, built_ins[i].description_help);
	printf("\n");
}

void cd(char *command){
	char *cmd = strdup(command);
	cmd = strtok(cmd, " ");
	cmd = strtok(NULL, " ");

	if(chdir(cmd)==-1){
		printf("error in cd\n");
	}
}

void history(char *command){
	//char *cmd = strdup(command);
	display_history();
}

void exit_builtin(char *command){
	save_history();
	exit(0);
}