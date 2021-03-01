#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main()
{
  //initialization

  int shmid;
  int *array;  
  char pathname[] = "07-1write.c";
  key_t key;
  char *source_code;

  int file_size = 0;
  int *file_size_ptr;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can not generate key! \n");
    exit(-1);
  }

  if ((shmid = shmget(key, sizeof(int) + file_size * sizeof(char), 0)) < 0) {
    //shared memory doesn't create
    printf("Shared memory cannot create! \n");
    exit(-1);
  }
  if ((file_size_ptr = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Shared memory cannot attach! \n");
    exit(-1);
  }
  file_size = *file_size_ptr;
  source_code = (char*)(file_size_ptr + 1);

  for (int i = 0; i < file_size; i++)
    putchar(source_code[i]);

  if (shmdt(file_size_ptr) < 0) {
    printf("Shared memory cannot detach! \n");
    exit(-1);
  }

  if (shmctl(shmid, IPC_RMID, NULL) < 0) {
    printf("Shared memory cannot delete! \n");
    exit(-1);
  }
  return 0; //end
}
