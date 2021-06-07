#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <stdio.h>

bool can_send = true;
int receiver_pid;
int value_sent;

void receiver_signal_handler(int nsig) {
	can_send = true;
}

void send_value() {
	int bits_count = sizeof(int) * 8;
	for (int i = 0; i < bits_count; ++i) {		
		while (!can_send);
		
		// Iterating over separate bits and sending them.
		if ((value_sent & (1 << i)) != 0) {
			kill(receiver_pid, SIGUSR1);
		}
		else {
			kill(receiver_pid, SIGUSR2);
		}
		can_send = false;
	}		
	kill(receiver_pid, SIGCHLD);
}

int main() {
	// Getting signal from receiver.
	signal(SIGUSR1, receiver_signal_handler);
	printf("PID of sender: %d\n", (int)getpid());		
	printf("Enter PID:\n");
	if (scanf("%d", &receiver_pid) < 0) {
		printf("An error occurred while reading receiver PID.\n");
		exit(-1);
	}
	
	// Getting value for sending.
	printf("Enter INT:\n");
	if (scanf("%d", &value_sent) < 0) {
		printf("Wrong enter.\n");
		exit(-2);
	}
	
	send_value();
	return 0;
}
