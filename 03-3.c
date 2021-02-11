#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
  printf("length of arr = %d", argc);
  printf("\nargv elements:");
  for(int i = 0; i < argc; i++){       
    puts(argv[i]); 
    printf("\n");   
  }
  printf("\n");
  
  printf("\nenvp elements:");
  int i = 0;
  while (envp[i] != NULL)
    printf("%s\n", envp[i++]);
  printf("\n");
  
}
