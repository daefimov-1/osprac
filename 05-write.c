#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  //initializing
  int fd, result;
  size_t size; 
  char resstring[14];
  char name[]="file.fifo";//name of FIFO file 
  (void)umask(0);

  printf("Writing data to file.fifo\n");

  if((fd = open(name, O_WRONLY)) < 0){
    printf("Can\'t open FIFO for writing\n");
    exit(-1);
  }
  size = write(fd, "Hello world!", 14);
  printf("Succesful writing to fifo\n");
  if(size != 14) {
    printf("Can\'t write all file.fifo data\n");
    exit(-1);
  }
  printf("Closing fd\n");
  close(fd);  
  return 0;
}

