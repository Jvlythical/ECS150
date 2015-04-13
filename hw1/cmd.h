#ifndef _CMD_H
#define _CMD_H

	#include "cmd_helper.h"

	// LS
void run_ls(char *path_input) {
		struct dirent *entry;
		DIR *dp;
		struct stat info;

		static char buffer[10] = {0};
		int i;
			
		dp = opendir(path_input);
				
		while( (entry = readdir(dp)) ) {
						
			if ( stat(entry->d_name, &info) == 0) {
				i = 0;	
				if ( S_ISDIR(info.st_mode) ) buffer[i] = 'd';
				else buffer[i] = '-';
				i++;
				if ((info.st_mode & S_IRUSR) == S_IRUSR) buffer[i] = 'r';
				else buffer[i] = '-';
				i++;
				if ((info.st_mode & S_IWUSR) == S_IWUSR) buffer[i] = 'w';
				else buffer[i] = '-';
				i++;
				if ((info.st_mode & S_IXUSR) == S_IXUSR) buffer[i] = 'x';
				else buffer[i] = '-';
				i++;
				// group permissions
				if ((info.st_mode & S_IRGRP) == S_IRGRP) buffer[i] = 'r';
				else buffer[i] = '-';
				i++;
				if ((info.st_mode & S_IWGRP) == S_IWGRP) buffer[i] = 'w';
				else buffer[i] = '-';
				i++;
				if ((info.st_mode & S_IXGRP) == S_IXGRP) buffer[i] = 'x';
				else buffer[i] = '-';
				i++;
				// other permissions
				if ((info.st_mode & S_IROTH) == S_IROTH) buffer[i] = 'r';
				else buffer[i] = '-';
				i++;
				if ((info.st_mode & S_IWOTH) == S_IWOTH) buffer[i] = 'w';
				else buffer[i] = '-';
				i++;
				if ((info.st_mode & S_IXOTH) == S_IXOTH) buffer[i] = 'x';
				else buffer[i] = '-';

				write(0, buffer, 10);
				write(0, " ", 1);
			}
			puts(entry->d_name);
		}
		closedir(dp);
		
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
		char *tokens[len];
		
		// Tokenize the input
		splitInput(cmd, tokens, len);
		
		while(1) {
			int argc = 0; 
			char *argv[16], *tok;
			
			cmd = tokens[i++];
			if(cmd == NULL) break;
			if(strcmp(cmd, "|") == 0 ) continue;

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
			
			if(pipe_set == 1)
				pipe_set = -1;
			if(tokens[i] != NULL) {
				if(strcmp(tokens[i],"|") == 0) 
					if( pipe(fd) == -1) {
						exit(EXIT_FAILURE);
					}
					
					pipe_set = 1;
			}

			int status = checkRedirectOut(tokens, &i);
							

			// Create new process
			int pid = fork();

			switch(pid) {
				case -1:
					return -1;
				case 0:
					//printf("%d\n", pipe_set);

					if(pipe_set == -1) {
						pipeFromParent(fd);

						//write(0, "\0", 1);
						char tmp[255];
						//int count = read(0, tmp, 255);
						//write(1, tmp, count);
						pipe_set = 0;
					}

					if(pipe_set == 1) 
						pipeToChild(fd);

					execvp(argv[0], argv);


					exit(0);
				default:
					
					waitpid(0, NULL, 0);
			}

		}

		return 1;
	}

#endif
