/*

@author Hasan Kamal

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define FILENAME "history_hksh.txt"
#define MAX_COMMAND_LEN 50
#define MAX_PATH_LENGTH 100

char file_path[MAX_PATH_LENGTH];

struct node{
	char *command_entry;
	struct node *next;
};

typedef struct node entry;

entry *front, *tail;

void add_entry_history(char *command){
	entry *new_entry = malloc(sizeof(entry));
	new_entry->command_entry = strdup(command);
	new_entry->next = NULL;

	if(front==NULL){
		front = tail = new_entry;
	}else{
		tail->next = new_entry;
		tail = new_entry;
	}
}

void display_history(){
	entry *t = front;
	int i=1;
	while(t){
		printf("%d: %s\n", i, t->command_entry);
		t = t->next;
		i++;
	}
}

void restore_history(){
	
	front = tail = NULL;

	FILE *file = fopen(FILENAME, "r");
	getcwd(file_path, MAX_PATH_LENGTH);
	if(file==NULL){
		fopen(FILENAME, "w");	
		return;
	}
	
	char input[MAX_COMMAND_LEN];
	while(fgets(input, MAX_COMMAND_LEN, file)){
		input[strlen(input)-1] = '\0';
		add_entry_history(input);
	}

	fclose(file);
}

void save_history(){

	if(chdir(file_path)==-1){
		printf("couldn't save history successfully\n");
	}
	FILE *file = fopen(FILENAME, "w");

	entry *t = front;
	while(t){
		fputs(t->command_entry, file);
		fputs("\n", file);
		t = t->next;
	}

	fclose(file);

	//free each node
	t = front;
	while(t){
		free(t);
		t = t->next;
	}

	front = tail = NULL;
}