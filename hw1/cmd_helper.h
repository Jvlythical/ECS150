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

	void printStdin() {
		char buf[999];

		int len = read(STDIN_FILENO, buf, 999);
		lseek(STDIN_FILENO, 0, SEEK_SET);
		//write(STDOUT_FILENO, buf, len);
		fprintf(stderr, "\n\nPRINTING STDIN: \n");
		fprintf(stderr, buf);
	}

	int tryPiping(char *tok, int pipe_set, int *fd, int *num) {
		if(pipe_set == 1 || *num > -1)
			pipe_set = -1;

		if(tok != NULL) {
			if(strcmp(tok,"|") == 0) 
				//if( pipe(fd)) 
				//	exit(EXIT_FAILURE);
				
				*num = *num + 1;
				pipe_set = 1;
		}

		return pipe_set;
	}
	
	int tryPiping2(char **tokens, int *pos, int* fd, int pipe_set, int *num) {
		int next = *pos;
		if(pipe_set == 1 || *num > -1)
			pipe_set = -1;


		// If no argument, return
		while(tokens[next] != NULL) {
			
			if(strcmp(tokens[next], "|") == 0) {
				pipe_set = tryPiping(tokens[next], pipe_set, fd, num);
				
				while(tokens[next + 1] != NULL) {
					tokens[next] = tokens[next + 1];
					next += 1;
				}

				tokens[next] = NULL;

				return pipe_set;
			}

			next += 1;
		}
		
		return pipe_set;
	}

	void splitInput(char *input, char **buf, int buf_len) {
		char cpy[strlen(input) + 1];
		memcpy(cpy, input, strlen(input) + 1);

		char *tok = strtok(input, "<|>");
		int len = 0, start = 0, i;

		while(tok != NULL) {
			buf[len++] = strip(tok);

			tok = strtok(NULL, "<|>&");
			if(tok != NULL) 
				for(i = start; i < strlen(cpy); ++i) {
					if(cpy[i] == '<' || cpy[i] == '>' || cpy[i] == '|' || cpy[i] == '&') {
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

	int getNumPipes(char **tokens, int **ar) {
		int i = 0;
		int count = 0;
		while(tokens[i] != NULL) {
			if(strcmp(tokens[i++], "|") == 0)
				count++;
		}

		ar = (int**) malloc(sizeof(int *) * count);

		for(i = 0; i < count; ++i) 
			ar[i] = (int*) malloc(sizeof(int) * 2);

		return count;
	}

	void closePipes(int **pipes, int count) {
		int i = 0;
		for(; i < count; ++i) {
			close(pipes[i][0]);
			close(pipes[i][1]);
		}
			
	}

	void marker() {
		return;
	}

	int getNumRedirects(char **tokens) {
		int i = 0;
		int count = 0;

		while(tokens[i] != NULL) {
			if(strcmp(tokens[i], "<") == 0 || strcmp(tokens[i], ">") == 0)
				count++;
			
			i++;
		}

		return count;
	}

#endif
