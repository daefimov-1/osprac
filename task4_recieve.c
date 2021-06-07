#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int result_number = 0;
bool received_value = false;
int sender_pid;
int current_bit_index = 0;

// Each notifies the sender.
void 1_handler_with_bit(int nsig) {
	result_number |= (1 << current_bit_index++);
	kill(sender_pid, SIGUSR1);
}

void 2_handler_without_bit(int nsig) {
	current_bit_index++;
	kill(sender_pid, SIGUSR1);
}

void chld_handler(int nsig) {
	received_value = true;
}

int main() {
	printf("PID of receiver: %d\n", (int)getpid());
	
	printf("Enter the PID for sender:\n");	
	scanf("%d", &sender_pid);
	
	printf("Wait.\n");
		
	signal(SIGUSR1, 1_handler_with_bit);
	signal(SIGUSR2, 2_handler_without_bit);
	signal(SIGCHLD, chld_handler); //handler for stream ending
	
	// Waiting value
	while (!received_value);
	
	printf("Received: %d\n", result_number);
	return 0;
}
