#ifndef _CMD_H
#define _CMD_H

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

	int run_cd(char *path) {
		int status;

		if(path == NULL || strcmp(path, "~") == 0)
			status = chdir("/root");
		else
			status = chdir(path);

		return status;
	}

	int run_file(char *cmd) {
		char *argv[16], *tok;
		int argc = 0;
		
		char *path = strtok(cmd, " ");
		
		//if(path != NULL) 
		//		argv[argc++] = path;
				
		do {
			tok = strtok(NULL, " \n\t");
			
			if(tok != NULL) 
				argv[argc++] = tok;
			else {
				argv[argc] = (char *) NULL;
				break;
			}

		} while(1);

		int pid = fork();
		
		if(pid == 0) {
			execvp(path, argv);
			exit(0);
		} else {
			waitpid(pid, NULL, 0);
		}

		return 0;
	}

#endif
