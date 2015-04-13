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
	void run_pwd(int mode) {
		char b[255];
		// Get dir path
			getcwd(b, 255);

		if(mode == 1) {
			
			// Print out path
			write(0, b, strlen(b));
			write(0, "\n", 1);
		} else {
			if(strlen(b) > 16) {
				int i = strlen(b) - 1, pos = 0;
				char prompt[255];

				for(; i >= 0; --i) {
					if(b[i] == '/') {
						prompt[pos++] = '/';

						prompt[pos++] = '.';
						prompt[pos++] = '.';
						prompt[pos++] = '.';

						prompt[pos++] = '/';

						break;
					}
					prompt[pos++] = b[i];
				}

				int len = pos;

				for(i = 0; i < len / 2; i++) {
					char tmp = prompt[i];
					prompt[i] = prompt[len - 1 - i];
					prompt[len - 1 - i] = tmp;
				}
				
				prompt[pos++] = '\0';

				write(0, prompt, strlen(prompt));
				write(0, "> ", 2);
			} else {
				// Print out path
				write(0, b, strlen(b));
				write(0, "> ", 2);
			}
		}
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
		char b[255];

		// Get dir path
		getcwd(b, 255);


		if(path == NULL || strcmp(path, "~") == 0)
			status = chdir("/root");
		else {
			//int length = strlen(b);
			//b[strlen(b) + 1] = '\0';
			//b[strlen(b)] = '/';
			//memcpy(b + strlen(b), path, strlen(path));
			//b[length + strlen(path) - 1]  = '\0';
			//write(STDOUT_FILENO, b, strlen(b));
			status = chdir(path);
		}

		return status;
	}
	
	// RUN FILE
	int run_file(char *cmd) {
		int fd[2], pipe_flag = 0;
		int i = 0, len = 16, n;
		int background_flag = 0, debug_flag = 0;
		char *tokens[len];

		int pipe_num = -1;
		int pipe_count = 0;
		int *pipes[10];
		int **test;
		int redirect_count = 0;
		
		// Check if cmd should run in background
		background_flag = isBackgroundCmd(cmd);
		// Tokenize the input
		splitInput(cmd, tokens, len);
		pipe_count = getNumPipes(tokens, test);
		redirect_count = pipe_count + getNumRedirects(tokens);
		
		for( n = 0; n < pipe_count; n++) {
			pipes[n] = malloc(sizeof(int) *2);
			pipe(pipes[n]);
		}
 
		while(1) {
			int argc = 0; 
			char *argv[16], *tok;
			
			cmd = tokens[i++];
			
			// Parse command
			if(cmd == NULL) break;
			if(strcmp(cmd, "|") == 0 ) continue;
			if(!strcmp(cmd, "<") || !strcmp(cmd, ">")) {
				if(pipe_flag == 0)
					//i += 2;
				continue;
			}
		
		//	pipe_flag = tryPiping(tokens[i], pipe_flag, fd);

			pipe_flag = tryPiping2(tokens, &i, pipes[pipe_num + 1], pipe_flag, &pipe_num);

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
			
			marker();

			// Create new process
			int pid = fork();

			switch(pid) {
				case -1:
					exit(1);
				case 0:
					if( i > 0)
						wait(getpid() - 1, NULL, 0);

					// Check for redirecting
					tryRedirectIn(tokens, &i);
			 		tryRedirectOut(tokens, &i);
				
					if(debug_flag) {
						fprintf(stderr, "\nPIPE NUM: %d\n", pipe_num);
						fprintf(stderr, "PIPE FLAG: %d\n", pipe_flag);
						fprintf(stderr, "PIPE COUNT: %d\n\n", pipe_count);
					}
					
					// Check for piping
					if(pipe_flag == -1 || (pipe_num == pipe_count - 1 && pipe_count > 1 )) {

						if(pipe_num > 0 && pipe_flag != -1) {
							pipeFromParent(pipes[pipe_num - 1]);
							
							//printStdin();
							if(debug_flag) 
								fprintf(stderr, "*PIPING FROM PIPE: %d\n\n", pipe_num - 1);

						} else {

							//fprintf(stderr, "%d %d\n", pipes[pipe_num - 1][0], pipes[pipe_num - 1][1]);
							//fprintf(stderr, "%d %d\n", pipes[pipe_num][0], pipes[pipe_num][1]);
							pipeFromParent(pipes[pipe_num]);

							if(debug_flag)
								fprintf(stderr, "PIPING FROM PIPE: %d\n\n", pipe_num);

							pipe_flag = 0;
						}
						
						printStdin();	
					}
					
					if(pipe_flag == 1) {
						if(debug_flag)
							fprintf(stderr, "PIPING STDOUT TO PIPE: %d\n\n ", pipe_num);
						
						pipeToChild(pipes[pipe_num]);
					}
	
					if(debug_flag) 
						fprintf(stderr, "Child running: %s %d\n", argv[0], getpid());

					// Run command
					execvp(argv[0], argv);
					exit(0);
			}

		}
		
		if(pipe_flag) {
			
			//close(pipes[0][0]);
			//close(pipes[0][1]);
			closePipes(pipes, pipe_count);
		}

		if(!background_flag)
			wait(NULL);
		wait(NULL);
		int c = 0;
		for(; c < redirect_count; c++)
			wait(NULL);

		return 1;
	}

#endif
