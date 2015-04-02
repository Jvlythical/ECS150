#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "cmd.h"

int resize_stack(char **s) {
	return 1;
}

int main() {
	int const INIT_SIZE = 255;
	int h_count = 0, h_size = INIT_SIZE;
	char b[INIT_SIZE];
	char *h_stack[INIT_SIZE], *cmd;

	do {
		int len = read(0, b, INIT_SIZE);
		b[len] = '\0';

		// Route to command
		if(strcmp(b, "ls\n") == 0) 
			run_ls();
		else if(strcmp(b, "pwd\n") == 0)
			run_pwd();
		else if(strcmp(b, "history\n") == 0)	
			run_history(h_stack, h_count);
		else {
			// Do something
		}
		
		// Push buffer onto history stack
		cmd = (char *) malloc(len);
		memcpy(cmd, b, len);
		h_stack[h_count++] = cmd;

		// Check for resizing
		if(h_count == h_size)
			resize_stack(h_stack);
	}
	while(strcmp(b, "exit\n")) ;


	return 0;
}
