#include <unistd.h>

int main() {

	write(1, "yo", 2);
	char *args[] = {"grep", "y", NULL};
	execvp("grep", args);
	close(0);
}
