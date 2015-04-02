#include <unistd.h>
#include <string.h>

#include "cmd.h"

int main() {
	int const DEFAULT = 255;
	char b[DEFAULT], h[DEFAULT];

	do {
		read(0, b, B_SIZE);
	
		if(strcmp(b, "ls\n") == 0) 
			run_ls();

		else if(strcmp(b, "pwd\n") == 0)
			run_pwd();

		else if(strcmp(b, "history\n") == 0)
			run_history();
		else {
			// Do something
		}	
	}
	while(strcmp(b, "exit\n")) ;


	return 0;
}
