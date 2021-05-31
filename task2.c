#include <signal.h>
#include <stdio.h>

void myOwn_handler(int nsig) {
	if (nsig == SIGINT) {
		printf("Signal: %d. Ctrl+C is ignored.\n", nsig);
	}
	else if (nsig == SIGQUIT) {
		printf("Signal: %d. Ctrl+4 is ignored.\n", nsig);
	}
}

int main() {	
	signal(SIGINT, myOwn_handler);
	signal(SIGQUIT, myOwn_handler);	
	while (1);
	return 0;
}
