/*

@author Hasan Kamal

*/

#include <stdio.h>
#include <string.h>

#define MAX_NUMBER_ARGUMENT_PER_COMMAND 100
#define MAX_NUMBER_COMMAND_PER_INPUT 100

#define MAX_COMMAND_LENS 100
#define MAX_ARGUMENT_LEN 100

#define FILE_NAME_LEN 100

typedef struct{
	char command_name[MAX_COMMAND_LENS];
	char *command_args[MAX_NUMBER_ARGUMENT_PER_COMMAND+1];

	int num_args;
} command_entry;

typedef struct{
	command_entry commands[MAX_NUMBER_COMMAND_PER_INPUT];

	char outfile[FILE_NAME_LEN];
	int has_outfile;

	char infile[FILE_NAME_LEN];
	int has_infile;

	int num_commands;
} input_table;

input_table table;

void trim_string(char *str){
	char *copy = strdup(str);

	while(*copy==' ')
		copy++;

	strcpy(str, copy);

	char *end = str + strlen(str)-1;
	while(*end==' ')
		end--;

	*(end+1) = '\0';
}

void parse_input(char *input){

	char *t = strdup(input);
	t = strtok(input, "|");
	int num_com = 0;
	while(t){
		strcpy(table.commands[num_com].command_name, t);
		num_com++;
		t=strtok(NULL, "|");
	}

	table.num_commands = num_com;
	table.has_outfile = 0;
	table.has_infile = 0;

	char *last_command = strdup(table.commands[table.num_commands-1].command_name);
	char *last_command_2 = strdup(last_command);
	
	//search for < in last_command(
	int has_lt = 0;
	for(char *ch = last_command; *ch!='\0'; ch++){
		if(*ch=='<'){
			has_lt = 1;
			break;
		}
	}

	if(has_lt){
		table.has_infile = 1;

		t = strtok(last_command, "<>");
		strcpy(table.commands[table.num_commands-1].command_name, t);
		t = strtok(NULL, "<>");

		strcpy(table.infile, t);
	}

	//search for > in last_command(
	int has_gt = 0;
	for(char *ch = last_command_2; *ch!='\0'; ch++){
		if(*ch=='>'){
			has_gt = 1;
			break;
		}
	}

	if(has_gt){

		table.has_outfile = 1;

		t = strtok(last_command_2, "<>");	
		strcpy(table.commands[table.num_commands-1].command_name, t);
		if(table.has_infile)
			t = strtok(NULL, "<>");
		t = strtok(NULL, "<>");

		strcpy(table.outfile, t);
	}

	//trim strings
	trim_string(table.infile);
	trim_string(table.outfile);
	for(int i=0; i<table.num_commands; i++)
		trim_string(table.commands[i].command_name);

	//parse commands
	for(int i=0; i<table.num_commands; i++){

		//count num_spaces
		int num_spaces = 0;
		for(int j=0; j<strlen(table.commands[i].command_name); j++)
			if(table.commands[i].command_name[j]==' ')
				num_spaces++;

		table.commands[i].num_args = num_spaces;

		//tokenize according to space
		char *copy = strdup(table.commands[i].command_name);
		char *t = strtok(copy, " ");
		int x=0;
		while(t){

			if(x==0){
				strcpy(table.commands[i].command_name, t);
				//strcpy(table.commands[i].command_args[0], t);
				table.commands[i].command_args[0] = strdup(t);
			}else{
				//strcpy(table.commands[i].command_args[x], t);
				table.commands[i].command_args[x] = strdup(t);
			}

			t = strtok(NULL, " ");
			x++;
		}

	}

	//make each command_arg array NULL terminated
	for(int i=0; i<table.num_commands; i++){
		table.commands[i].command_args[table.commands[i].num_args+1] = NULL;
	}
}

void show_input_table(){
	for(int i=0; i<table.num_commands; i++){
		printf("%d: %s(num_args:%d)\n", i, table.commands[i].command_name, table.commands[i].num_args);

		for(int j=0; j<table.commands[i].num_args+2; j++){
			printf("\t%s\n", table.commands[i].command_args[j]);
		}
	}

	if(table.has_outfile)
		printf("outfile: %s\n", table.outfile);
	if(table.has_infile)
		printf("infile: %s\n", table.infile);
}