#ifndef _CMD_H
#define _CMD_H

	#include "cmd_helper.h"

	// LS
void run_ls(char *path_input) {
		struct dirent *entry;
		DIR *dp;
		struct stat info;

		static char local_buff[16] = {0};
		int i;
			
		dp = opendir(path_input);
				
		while( (entry = readdir(dp)) ) {
						
			if ( stat(entry->d_name, &info) == 0) {
//				pwrite(0, info.st_mode, 100, 0);
//				fprintf(stdout, "%s", info.st_mode)
//				write(0, info.st_mode,  20);				
//				printf("%lo", info.st_mode);
				i = 0;	
				if ( S_ISDIR(info.st_mode) ) local_buff[i] = 'd';
				else local_buff[i] = '-';
				i++;
				if ((info.st_mode & S_IRUSR) == S_IRUSR) local_buff[i] = 'r';
				else local_buff[i] = '-';
				i++;
				if ((info.st_mode & S_IWUSR) == S_IWUSR) local_buff[i] = 'w';
				else local_buff[i] = '-';
				i++;
				if ((info.st_mode & S_IXUSR) == S_IXUSR) local_buff[i] = 'x';
				else local_buff[i] = '-';
				i++;
				// group permissions
				if ((info.st_mode & S_IRGRP) == S_IRGRP) local_buff[i] = 'r';
				else local_buff[i] = '-';
				i++;
				if ((info.st_mode & S_IWGRP) == S_IWGRP) local_buff[i] = 'w';
				else local_buff[i] = '-';
				i++;
				if ((info.st_mode & S_IXGRP) == S_IXGRP) local_buff[i] = 'x';
				else local_buff[i] = '-';
				i++;
				// other permissions
				if ((info.st_mode & S_IROTH) == S_IROTH) local_buff[i] = 'r';
				else local_buff[i] = '-';
				i++;
				if ((info.st_mode & S_IWOTH) == S_IWOTH) local_buff[i] = 'w';
				else local_buff[i] = '-';
				i++;
				if ((info.st_mode & S_IXOTH) == S_IXOTH) local_buff[i] = 'x';
				else local_buff[i] = '-';

				write(0, local_buff, 9);
				write(0, " ", 1);
			}
			puts(entry->d_name);
//			write(0, entry->d_name, 255);
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
		int fd[2], pipe_flag = 0;
		int i = 0, len = 16;
		int background_flag = 0, debug_flag = 0;
		char *tokens[len];
		
		// Check if cmd should run in background
		background_flag = isBackgroundCmd(cmd);
		// Tokenize the input
		splitInput(cmd, tokens, len);

		while(1) {
			int argc = 0; 
			char *argv[16], *tok;
			
			cmd = tokens[i++];
			
			// Parse command
			if(cmd == NULL) break;
			if(strcmp(cmd, "|") == 0 ) continue;
			if(!strcmp(cmd, "<") || !strcmp(cmd, ">")) {
				i += 2;
				continue;
			}
		
			pipe_flag = tryPiping(tokens[i], pipe_flag, fd);

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
						printf("Pipe status: %d\n", pipe_flag);
					}

					// Check for redirecting
					tryRedirectIn(tokens, &i);
			 		tryRedirectOut(tokens, &i);
					
					// Check for piping
					if(pipe_flag == -1) {
						pipeFromParent(fd);
						pipe_flag = 0;
					}
					
					if(pipe_flag == 1) 
						pipeToChild(fd);
										
					// Run command
					execvp(argv[0], argv);
					exit(0);
										
					if(debug_flag) {
						printf("Background_flag: %d\n", background_flag );
						printf("Parent waiting for PID: %d\n", pid);
						printf("%d %d\n", fd[0], fd[1]);
					}
			}

		}

		close(fd[0]);
		close(fd[1]);

		if(!background_flag)
			wait(NULL);


		return 1;
	}

#endif
