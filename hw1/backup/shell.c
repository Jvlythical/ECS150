#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "cmd.h"
#include "helper.h"
#include "noncanmode.h"

int main() {
	const int INIT_SIZE = 255;
	char b[INIT_SIZE], *tok, *cmd;

	do {
		int len = read(0, b, INIT_SIZE);
		b[len] = '\0';
		
		tok = strtok(b, " \t\n");
		route_to_cmd(tok);	
		
		push_h(b, len);	
	}
	while(strcmp(b, "exit")) ;

	free_ar(h_stack, h_count);

	return 0;
}
