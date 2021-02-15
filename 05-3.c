#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void reverse(char* str, size_t sz);

int main()
{
  int pipe_parent[2]; //array of parents
  int pipe_child[2]; //array of childs
  int result;
  size_t size;
  char resstring[14];

 
  if (pipe(pipe_child) < 0) {
    printf("Can\'t create child pipe\n");
    exit(-1);
  }

  if (pipe(pipe_parent) < 0) {
    printf("Can\'t create parent pipe\n");
    exit(-1);
  }

  result = fork();
  if (result < 0) {
    printf("result < 0\n");
    printf("Can\'t fork child\n");
    printf("Exit out of program\n");
    exit(-1);
  } else if (result > 0) {
    printf("it is parent\n");
    close(pipe_parent[0]); 
    close(pipe_child[1]);
    size = write(pipe_parent[1], "Hello, world!", 14);
    if (size != 14) {
      printf("size != 14\n");
      printf("Can\'t write all string\n");
      printf("Exit out of program\n");
      exit(-1);
    }
    close(pipe_parent[1]);

    size = read(pipe_child[0], resstring, 14);
    if (size != 14) {
      printf("size != 14\n");
      printf("Can\'t read from child\n");
      printf("Exit out of program\n");
      exit(-1);
    }
    printf("Parent tooken out of child: %s\n", resstring);
    printf("Exit out of parent\n");
  } else {    
    close(pipe_parent[1]);
    close(pipe_child[0]);
    size = read(pipe_parent[0], resstring, 14);
    if (size < 0) {
      printf("size < 0\n");
      printf("Can\'t get from parent\n");
      printf("Exit out of program\n");
      exit(-1);
    }
    printf("Tooken out of parent: %s\n", resstring);
    reverse(resstring, 13);
    size = write(pipe_child[1], resstring, 14);
    close(pipe_parent[0]);
    printf("Exit out of child\n");
  }
  return 0;
}

void reverse(char* str, size_t sz)
{
  printf("Reverse is in process\n");
  for (int i = 0; i < sz / 2; i++) {
    char t = str[i];
    str[i] = str[sz - i - 1];
    str[sz - i - 1] = t;
  }
}

