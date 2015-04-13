#ifndef _HELPER_H
#define _HELPER_H
	
	int h_size = 255, h_count = 0;
	char *h_stack[255];
	char str[] = ".";

	int *num[10];

void setPrompt() {
	run_pwd(0);
}

	
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
		
		push_h(cmd);

		// Route to command
		if(strcmp(tok, "ls") == 0) {
			if ((tok = strtok(NULL, " \n\t")) != NULL) {
				run_ls(tok);
			}
			else {
				run_ls(str);
			}
		}
<<<<<<< HEAD
		else if(strcmp(tok, "pwd") == 0)
			run_pwd(1);
		else if(strcmp(tok, "history") == 0)	
=======
		else if(strcmp(tok, "pwd") == 0) {
			run_pwd();
		}
		else if(strcmp(tok, "history") == 0) {
>>>>>>> fff05678ade88c6e51fd7a75f4b0139a2338fb3d
			run_history(h_stack, h_count);
		}
		else if (strcmp(tok, "cd") == 0) {
			tok = strtok(NULL, " \n\t");
			run_cd(tok);
		}
<<<<<<< HEAD
		else if (strcmp(tok, "exit") == 0) 
			exit(0);
=======
>>>>>>> fff05678ade88c6e51fd7a75f4b0139a2338fb3d
		else {
			run_file(cmd);
		}
		
	}

	void handle_enter(char k, char *b, int *b_pos) {
		// Terminate the string
		b[*b_pos] = '\0';
<<<<<<< HEAD
		write(STDOUT_FILENO, NEXT_LINE, strlen(NEXT_LINE));

		route_to_cmd(b);
		//write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
		setPrompt();
		
=======
		write(STDIN_FILENO, NEXT_LINE, strlen(PROMPT));

		route_to_cmd(b);

		write(STDIN_FILENO, PROMPT, strlen(PROMPT));

>>>>>>> fff05678ade88c6e51fd7a75f4b0139a2338fb3d
		// Reset buffer
		*b_pos = 0;
	}

	void handle_backspace(int *b_pos) {
<<<<<<< HEAD
		if(*b_pos == 0) {
			write(STDOUT_FILENO, "\a", 1);
			return;
		}

		write(STDOUT_FILENO, DELETE_CHAR, strlen(DELETE_CHAR));
=======
		char tmp;

		write(STDIN_FILENO, DELETE_CHAR, strlen(DELETE_CHAR));
>>>>>>> fff05678ade88c6e51fd7a75f4b0139a2338fb3d
		*b_pos -= 1;
	}

	void handle_arrows(char direction, int *b_pos, char *b, int *counter) {

		int length = 0;

		//length = strlen(h_stack[h_count-1]);
		//length = strlen(b);
		
		if (*counter != 0) {
			length = strlen(h_stack[h_count]);
			while (length > 0) {
				write(1, DELETE_CHAR, strlen(DELETE_CHAR));
				length--;
			}
			*counter += 1;
		}

//			length = strlen(h_stack[h_count-1]);

		if (direction == cA) {	//up
			if (h_count == 1) {
				write(1, h_stack[0], strlen(h_stack[0]));
			}
			else if (h_count == 0) {

			}

			else {
				
				h_count--;	
				write(1, h_stack[h_count], strlen(h_stack[h_count]));
			}
//			write(1, DELETE_CHAR, strlen(DELETE_CHAR));
		}

		else if (direction == cB) { //down
		   
			if (h_count == 0) {
				
			}
			else if (h_stack[h_count] == NULL) {
				write(1, h_stack[h_count-1], strlen(h_stack[h_count-1]));
			}
			else {
				h_count++;
				write(1, h_stack[h_count-1], strlen(h_stack[h_count-1]));
			}				

		}

		*counter += 1;

	}


#endif
