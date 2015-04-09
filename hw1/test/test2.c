#include <unistd.h>
#include <fcntl.h>

int main() {
	int fd = open("input", O_APPEND);
	
	
	//char *args[] = {"cat", "input", NULL};

	dup2(fd, 1);
	puts("hi");
	//execvp("ls", args);

	return 0;
}
