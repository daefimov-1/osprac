#include <sys/types.h>
#include <signal.h>

int main() {
	// Ignore SIGINT
	signal(SIGINT, SIG_IGN);
        // Ignore SIGQUIT
	signal(SIGQUIT, SIG_IGN);
	// Loop is in progress, wait.
	while (1);
	return 0;
}
