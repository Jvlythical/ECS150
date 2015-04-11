#ifndef _CMD_HELPER_H
#define _CMD_HELPER_H

	void splitInput(char *input, char **buf, int buf_len) {
		char cpy[strlen(input) + 1];
		memcpy(cpy, input, strlen(input) + 1);

		char *tok = strtok(input, "<|>");
		int len = 0, start = 0, i;

		while(tok != NULL) {
			buf[len++] = tok;

			tok = strtok(NULL, "<|>");
			if(tok != NULL) 
				for(i = start; i < strlen(cpy) - start; ++i) {
					if(cpy[i] == '<' || cpy[i] == '>' || cpy[i] == '|') {
						buf[len] = malloc(1);
						buf[len++][0] = cpy[i];
						start = i + 1;
					}
				}
		}

		buf[len++] = NULL;
		
	}

	char * strip(char *str) {
		int end = strlen(str) - 1;

		if(str[0] == ' ' || str[0] == '\n' || str[0] == '\t')
			str = str + 1;
		if(str[end] == ' ' || str[end] == '\n' || str[end] == '\t')
			str[end] = '\0';

		return str;
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

	int checkRedirectOut(char **tokens, int *pos) {
		int next = *pos;
		if(tokens[next] == NULL) 
			return -1;

			if(strcmp(tokens[next], ">") == 0) {
				if(tokens[next + 1] != NULL) {
					int fd = open(strip(tokens[next + 1]), O_CREAT | O_WRONLY, 0751);
					
					if(fd < 0)
						return 0;

					if(dup2(fd, STDOUT_FILENO)) {
						*pos = *pos + 2;
						return 1;
					} else 
						return 0;
				}
			}
	}

	int checkRedirectIn(char **tokens, int *pos) {
		int next = *pos;
		if(tokens[next] == NULL) 
			return -1;

			if(strcmp(tokens[next], "<") == 0) {
				if(tokens[next + 1] != NULL) {
					char buf[999];

					int fd = open(strip(tokens[next + 1]), O_RDONLY);

					if(dup2(fd, STDIN_FILENO)) {
						*pos = *pos + 2;
						return 1;
					} else 
						return 0;

				}
			}
	}

#endif
