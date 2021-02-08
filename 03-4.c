#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
  pid_t pid = fork();

  if (pid == -1) {
    printf("It is error!");
    exit(-1);
  } else if (pid == 0) {
    
    printf("Now we will launch third program!");
    execl("test.out", "test.out", "1234", NULL);
  } else {   
    printf("It is parent!");
  }
  
}
