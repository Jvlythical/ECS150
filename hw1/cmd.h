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
		int argc = 0, len = 16, i = 0;
		char *argv[16], *tok, *tokens[len];
		
		// Tokenize the input
		splitInput(cmd, tokens, len);
		
		while(1) {
			cmd = tokens[i];

			if(cmd == NULL)
				break;

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

			//argv = getCmdArgs(cmd);

			// Create new process
			int pid = fork();

			switch(pid) {
				case -1:
					exit(1);
				case 0:
					execvp(argv[0], argv);
					exit(0);
				default:
					waitpid(0, NULL, 0);
			}

			++i;
		}

		return 1;
	}

#endif
