#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
  //initializing
  int fd, result;
  size_t size;
  char resstring[14];
  char name[] = "file.fifo"; //name of FIFO file 
  (void)umask(0);

  if (mknod(name, S_IFIFO | 0666, 0) < 0) {
    printf("file is not working\n");
    exit(-1);
  }
  printf("FIFO file is created now\n");
  printf("Now reading from file.fifo\n");

  if ((fd = open(name, O_RDONLY)) < 0) {
    printf("Can\'t open file.FIFO for reading\n");
    exit(-1);
  }
  //getting size
  size = read(fd, resstring, 14);
  if (size < 0) {
    printf("Can\'t read string\n");
    exit(-1);
  }
  printf("Read from file.fifo: %s\n", resstring);
  printf("Closing fd\n");
  close(fd);
  return 0;
}
