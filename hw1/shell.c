#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "cmd.h"

int resize_stack(char **s) {
	return 1;
}

void free_ar(char **ar, int size) {
	int i = 0;
	
	for(; i < size; ++i) 
		free(ar[i]);
}

int main() {
	int const INIT_SIZE = 255;
	int h_count = 0, h_size = INIT_SIZE;
	char b[INIT_SIZE], *tok;
	char *h_stack[INIT_SIZE], *cmd;

	do {
		int len = read(0, b, INIT_SIZE);
		b[len] = '\0';

		tok = strtok(b, " \t\n");

		// Route to command
		if(strcmp(tok, "ls") == 0) 
			run_ls();
		else if(strcmp(tok, "pwd") == 0)
			run_pwd();
		else if(strcmp(tok, "history") == 0)	
			run_history(h_stack, h_count);
		else if (strcmp(tok, "cd") == 0) {
			tok = strtok(NULL, " \n\t");
			run_cd(tok);
		} else {
			run_file(tok);
		}
		
		// Push buffer onto history stack
		cmd = (char *) malloc(len);
		memcpy(cmd, b, len);
		h_stack[h_count++] = cmd;

		// Check for resizing
		if(h_count == h_size)
			resize_stack(h_stack);
	}
	while(strcmp(b, "exit")) ;

	free_ar(h_stack, h_count);

	return 0;
}
