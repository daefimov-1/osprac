#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
  //initialization
  
  char pathname[] = "07-1write.c";
  int shmid;
  key_t key;
  char *source_code;
  int file_size = 0;
  int *file_size_ptr;
  FILE *file;
  file = fopen(pathname, "r");

  fseek(file, 0L, SEEK_END);
  file_size = ftell(file);
  rewind(file);

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can not generate key! \n");
    exit(-1);
  }

  if ((shmid = shmget(key, sizeof(int) + file_size * sizeof(char), 0666|IPC_CREAT)) < 0) {
    //shared memory doesn't create
    printf("Shared memory cannot create! \n");
    exit(-1);
  }
  if ((file_size_ptr = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Shared memory cannot attach! \n");
    exit(-1);
    //exit
  }
  *file_size_ptr = file_size;
  source_code = (char*)(file_size_ptr + 1);

  for (int i = 0; i < file_size; i++)
    source_code[i] = fgetc(file);

  fclose(file);


  if (shmdt(file_size_ptr) < 0) {
    printf("Shared memory cannot detach! \n");
    exit(-1);
    //exit
  }
  return 0; //end
}
