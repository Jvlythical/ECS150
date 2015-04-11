#ifndef _CMD_HELPER_H
#define _CMD_HELPER_H

	char * strip(char *str) {
		int end = strlen(str) - 1;
		
		if(str[end] == ' ' || str[end] == '\n' || str[end] == '\t')
			str[end] = '\0';
		
		if(str[0] == ' ' || str[0] == '\n' || str[0] == '\t') 
			str = str + 1;
		

		return str;
	}
	void splitInput(char *input, char **buf, int buf_len) {
		char cpy[strlen(input) + 1];
		memcpy(cpy, input, strlen(input) + 1);

		char *tok = strtok(input, "<|>");
		int len = 0, start = 0, i;

		while(tok != NULL) {
			buf[len++] = strip(tok);

			tok = strtok(NULL, "<|>");
			if(tok != NULL) 
				for(i = start; i < strlen(cpy); ++i) {
					if(cpy[i] == '<' || cpy[i] == '>' || cpy[i] == '|') {
						buf[len] = malloc(1);
						buf[len++][0] = cpy[i];
						start = i + 1;
						break;
					}
				}
		}

		buf[len++] = NULL;
		
	}


	inline char** getCmdArgs(char *cmd) {
		char cpy[strlen(cmd) + 1];
		memcpy(cpy, cmd, strlen(cmd) + 1);
		int argc = 0;

		char **argv = (char**) malloc(16 * sizeof(char *));
		
		char* tok = strtok(cpy, " ");
		argv[argc++] = tok;

		do {
			tok = strtok(NULL, " \n\t");
			
			if(tok != NULL) 
				argv[argc++] = tok;
			else {
				argv[argc] = (char *) NULL;
				break;
			}

		} while(1);

		return argv;
	}

	void pipeFromParent(int *fd) {
		// Close the write end
		close(fd[1]);

		// Copy the read end into child's stdin
		dup2(fd[0], STDIN_FILENO);

		// Close the read end
		close(fd[0]);
	}

	void pipeToChild(int *fd) {
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}

	void tryRedirectOut(char **tokens, int *pos) {
		int next = *pos;

		if(tokens[next] == NULL) 
			return;

		if(strcmp(tokens[next], ">") == 0) {
			if(tokens[next + 1] != NULL) {
				int fd = open(strip(tokens[next + 1]), O_CREAT | O_WRONLY, 0644);
				
				if(fd < 0)
					exit(1);

				if(dup2(fd, STDOUT_FILENO) == -1) 
					exit(1);
			}
		}

	}

	void tryRedirectIn(char **tokens, int *pos) {
		int next = *pos;
		
		// If no argument, return
		if(tokens[next] == NULL) 
			return;

		if(strcmp(tokens[next], "<") == 0) {
			if(tokens[next + 1] != NULL) {
				int fd = open(strip(tokens[next + 1]), O_RDONLY);
				
				// If file could not be read
				if(fd < 0)
					exit(1);
				
				// If dup2 fails
				if(dup2(fd, STDIN_FILENO) == -1) 
					exit(1);
			}
		}
	
		next += 2;
		tryRedirectOut(tokens, &next);
	}

	int isBackgroundCmd(char *cmd) {
		int i = strlen(cmd) - 1;

		for(; i >= 0; --i) 
			if(cmd[i] == '&') {
				cmd[i] = '\0';
				return 1;
			}

		return 0;
	}

#endif
