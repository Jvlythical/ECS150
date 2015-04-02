#ifndef CMD_H
#define CMD_H
	
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
	void run_history() {

	}

#endif
