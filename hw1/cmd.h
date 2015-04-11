#ifndef _CMD_H
#define _CMD_H

	#include "cmd_helper.h"

	// LS
	void run_ls() {
	}

	// PWD
	void run_pwd() {
		char b[255];

		// Get dir path
		getcwd(b, 255);

		// Print out path
		write(0, b, strlen(b));
		write(0, "\n", 1);
	}

	// HISTORY
	void run_history(char **s, int size) {
		int i = 0;
		
		for(; i < size; ++i) {
			write(0, s[i], strlen(s[i]));
			write(0, "\n", 1);
		}
	}

	// CD
	int run_cd(char *path) {
		int status;

		if(path == NULL || strcmp(path, "~") == 0)
			status = chdir("/root");
		else
			status = chdir(path);

		return status;
	}
	
	// RUN FILE
	int run_file(char *cmd) {
		int fd[2], pipe_set = 0;
		int i = 0, len = 16;
		int background_flag = 0, debug_flag = 1;
		char *tokens[len];
		
		// Tokenize the input
		splitInput(cmd, tokens, len);

		// Check if cmd should run in background
		background_flag = isBackgroundCmd(cmd);

		while(1) {
			int argc = 0; 
			char *argv[16], *tok;
			
			cmd = tokens[i++];
			
			// Parse command
			if(cmd == NULL) break;
			if(strcmp(cmd, "|") == 0 )
				continue;

			if(!strcmp(cmd, "<") || !strcmp(cmd, ">")) {
				i += 2;
				continue;
			}
			
			if(pipe_set == 1)
				pipe_set = -1;

			if(tokens[i] != NULL) {
				if(strcmp(tokens[i],"|") == 0) 
					if( pipe(fd)) 
						exit(EXIT_FAILURE);
					
					pipe_set = 1;
			}

			// Get args
			tok = strtok(cmd, " ");
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
			
			// Create new process
			int pid = fork();

			switch(pid) {
				case -1:
					exit(1);
				case 0:

					if(debug_flag) {
						printf("Child running: %s\n", argv[0]);
						printf("%d %d\n", fd[0], fd[1]);
						printf("Pipe status: %d\n", pipe_set);
					}

					// Check for redirecting
					tryRedirectIn(tokens, &i);
			 		tryRedirectOut(tokens, &i);
					
					// Check for piping
					if(pipe_set == -1) {
						pipeFromParent(fd);
						pipe_set = 0;
						
						//write(STDIN_FILENO, "\n", 1);
						//char tmp[255];
						//int len = read(STDIN_FILENO, tmp, 255);
						//write(STDOUT_FILENO, tmp, len);
					}
					
					if(pipe_set == 1) 
						pipeToChild(fd);
					
										
					// Run command
					execvp(argv[0], argv);
					exit(0);
				default:
					if(!background_flag)
						wait(NULL);
					
					if(debug_flag) {
						printf("Parent waiting for PID: %d\n", pid);
						printf("%d %d\n", fd[0], fd[1]);
					}
			}

		}

		return 1;
	}

#endif
