#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>

#include <dirent.h> //for ls
#include <sys/stat.h> //for ls
#include <sys/types.h> //for ls

#include "noncanmode.h"
#include "keycode.h"
#include "cmd.h"
#include "helper.h"

int main(int argc, char *argv[]) {
    struct termios SavedTermAttributes;
    int INIT_SIZE = 255, b_pos = 0;
		char b[INIT_SIZE], RXChar;
		int x = 0;
    
    SetNonCanonicalMode(STDIN_FILENO, &SavedTermAttributes);

    write(STDIN_FILENO, PROMPT, strlen(PROMPT));

    do {
			// Get keystroke
			read(STDIN_FILENO, &RXChar, 1);

			// CTRL-D
			if(RXChar == CTRL_D) 
				break;
				
			// ENTER	
			if(RXChar == ENTER) {
				handle_enter(RXChar, b, &b_pos);
				continue;
			} 
			
			// BACKSPACE
			if(RXChar == BACKSPACE) {
				handle_backspace(&b_pos);		
				continue;
			}
			
			// UP/DOWN ARROWS
			if(RXChar == ESC) {
				read(STDIN_FILENO, &RXChar, 1);
				if(RXChar == LB) {
					read(STDIN_FILENO, &RXChar, 1);
					handle_arrows(RXChar, &b_pos, b, &x);
					x++;
				}
				continue;
			}
			
			// Else display keystroke
			write(STDIN_FILENO, &RXChar, 1);
			b[b_pos++] = RXChar;

    } while(1);
    
    ResetCanonicalMode(STDIN_FILENO, &SavedTermAttributes);

    return 0;
}
