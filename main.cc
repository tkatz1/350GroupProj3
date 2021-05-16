#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef pthread_mutex_t semaphore;
semaphore mutex;       //controls access to rc
semaphore db;           //controls access to the database
int rc = 0;                 //# of threads reading

int database = 0;

void * reader(void * arg)
{
  int i = *((int *)arg);

  pthread_mutex_lock(&mutex);             //get exclusive access to rc
    rc = rc + 1;            //one more reader now
    if(rc == 1) pthread_mutex_lock(&db);  //if this is the first reader ...
  pthread_mutex_unlock(&mutex);           //release exclusive access to rc
                            //”i” → thread num [1..10] passed from main

  printf("Reader %d in critical section\n", i);

  printf("Reading database... value is %d\n", database);
  pthread_mutex_lock(&mutex);             //get exclusive access to rc
    rc = rc - 1;              //one reader fewer now
    if(rc == 0) pthread_mutex_unlock(&db);  //if the last reader ..
  pthread_mutex_unlock(&mutex);           //release exclusive access to rc
  printf("Reader %d in non-critical section\n", i);

  return NULL;
}

void * writer(void * arg)
{
  printf("Writer tries to enter the critical section\n");
  pthread_mutex_lock(&db);                  //get exclusive access
    printf("Writer is in critical region\n");
    database = 1;
    printf("Writing to database... value is %d\n", database);
  pthread_mutex_unlock(&db);                //release exclusive access
  printf("Writer is in non-critical region\n");

  return NULL;
}



int main(){
  pthread_t r[9];
  pthread_t w;

  //Create a reader thread
  int args[9];

  args[0] = 0;
  pthread_create(&(r[0]), NULL, reader, &(args[0]));

  //Create a writer thread
  pthread_create(&w, NULL, writer, NULL);


  //Create 8 reader threads
  for (int i = 1; i < 9; i++){
    args[i] = i;
    pthread_create(&(r[i]), NULL, reader, &(args[i]));
  }

  //join threads
  pthread_join(w, NULL);
  for (int j = 0; j < 9; j++){
    pthread_join(r[j], NULL);
  }

}
