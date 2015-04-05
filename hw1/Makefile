shell: shell.o
	gcc -g -Wall -o shell shell.o

shell.o: shell.c noncanmode.h helper.h cmd.h keycode.h
	gcc -g -Wall -c shell.c

clean:
	rm -f shell.o shell
