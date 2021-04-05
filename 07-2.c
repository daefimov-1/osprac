#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
int a = 0;

void *mythread(void *dummy)
{
  pthread_t mythid;
  mythid = pthread_self();
  a = a + 1;
  printf("Thread %d, Calculation result = %d\n", mythid, a);
  return NULL;
}

int main()
{
  pthread_t firstthid, secondthid, mythid;
  int result;

  result = pthread_create( &firstthid, (pthread_attr_t *)NULL, mythread, NULL);
  if (result != 0) {
    printf ("Error on thread create (second thread), return value = %d\n", result);
    exit(-1);
  }
  printf("Second thread created, thid = %d\n", firstthid);

  
  result = pthread_create( &secondthid, (pthread_attr_t *)NULL, mythread, NULL);
  if (result != 0) {
    printf ("Error on thread create (third thread), return value = %d\n", result);
    exit(-1);
  }
  printf("Third thread created, thid2 = %d\n", secondthid);

  mythid = pthread_self();
  a = a + 1;
  printf("Thread %d, Calculation result = %d\n", mythid, a);
  
  pthread_join(firstthid, (void **)NULL);
  pthread_join(secondthid, (void **)NULL);

  return 0;
}
