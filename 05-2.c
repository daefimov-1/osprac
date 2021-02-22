#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>

int result = 0;

int main() {
  //initializing
  int fd[2];
  size_t size = 1;
  char* resstring = malloc(1 * sizeof(char));
  if(pipe(fd) < 0) {
    printf("Can\'t create pipe\n");
    exit(-1); //exit
  }
  printf("fcntl is in progress\n");
  fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK);
  
  //Counting
  while (size == 1) {
    printf("write... ");
    size = write(fd[1], resstring, 1);
    result++;
  }
  printf("\nOutput of bytes is %d \n", result);
  return 0;
}
