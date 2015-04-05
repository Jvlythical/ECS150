#ifndef _HELPER_H
#define _HELPER_H
	
	int h_size = 255, h_count = 0;
	char *h_stack[255];

// HISTORY
	int resize_h(char **s) {
		return 1;
	}

	void free_h(char **ar, int size) {
		int i = 0;
		
		for(; i < size; ++i) 
			free(ar[i]);
	}

	void push_h(char *cmd) {
		int len = strlen(cmd);

		// Push buffer onto history stack
		char *tmp = (char *) malloc(len);
		memcpy(tmp, cmd, len);
		h_stack[h_count++] = tmp;

		if(h_count == h_size)
			resize_h(h_stack);
	}

// ROUTER
	void route_to_cmd(char *cmd) {
			
		char *tok = strtok(cmd, " \t\n");

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
			//run_file(tok);
		}
		
		push_h(cmd);
	}

	void handle_enter(char k, char *b, int *b_pos) {
		// Terminate the string
		b[*b_pos] = '\0';
		write(STDIN_FILENO, NEXT_LINE, strlen(PROMPT));

		route_to_cmd(b);

		write(STDIN_FILENO, PROMPT, strlen(PROMPT));

		// Reset buffer
		*b_pos = 0;
	}

	void handle_backspace(int *b_pos) {
		write(STDIN_FILENO, DELETE_CHAR, strlen(DELETE_CHAR));
		b_pos -= 1;
	}

#endif
