//  3420ICT System Programming
//  SID: S2193948
//  Author: Chun Hung Wong (Grimes)
//  Program: sema.c
//  Purpose: Use the semaphores to create a thread for synchronisation

/*** Preprocessor ***/
#include <stdio.h>    //Standard Input/Output library eg. getchar, gets, putchar, puts, sprintf
#include <unistd.h>   //eg. sleep
#include <pthread.h>  //eg. pthread_create, pthread_join, pthread_exit
#include "sema.h"



/*** Function declartion (Prototype) ***/
void procure(Semaphore *semaphore);
void vacate(Semaphore *semaphore);
int initialiser();
int semDestructor();

/*** Function declaration (prototype) ***/
void *printline(void *string);

Semaphore sem, *semPtr;


int rt;   // for debugging

/*
 *  Fucntion: main()
 *  Description: This function is create threads to read string and print string by using semphore for controlling the sequence
 *  Parameter:  None
 *  Return: int 0 if success
 */
int main ()
{

    /*** Local variables ***/
    int rt; // for catching error
    // Semaphore sem;
    pthread_t t_child;

    //Start: initialiser
    rt = initialiser();   //debugger:
    if(rt != 0) { perror("Main: couldn't initailise the initaliser"); }               // error message:
    // else { printf("Main: Successfully create initialiser\n");}  // debugger:

    printf("Initialised semaphore value is: %d\n", sem.value);  // debugger:

    procure(semPtr);
    printf("Main: locking the mutex now\n");

    /*** Create the child thread ***/
    rt = pthread_create(&t_child, NULL, printline, (void *) &sem);            //create child thread
    if(rt != 0) { perror("Main: Couldn't create a thread!!!"); }                  // error message:

    sleep(2);
    printf("Main: printing in the main thread\n");
    vacate(&sem);

    // procure(&sem);
    /*** Testing ***/
    // procure(&sem);
    // printf("sematest: locking now\n");
    // vacate(&sem);
    /*** Testing end ***/


    //At the End: destructor
    rt = destructor();    //debugger:
    if(rt != 0) { perror("Main: couldn't use destructor"); }               // error message:
    else { printf("Main: Successfully destroy related stuffs\n"); }  // debugger:


    return 0;
}

/*
 *  Fucntion: initialiser
 *  Description: This function is for initialise the semaphore, mutex and the condition variable
 *  Parameter:  None
 *  Return: int 0 if success
 */
int initialiser()
{

  /*** Intialize Pointer ***/
  semPtr = &sem;

  /*** Initialize semaphore value ***/
  sem.value = 1;    // 1 is correct, wait is 0
  // printf("sem.value is : %d\n", sem.value);   //debugger:

  /*** Initialize mutex ***/
  rt = pthread_mutex_init(&sem.mutex, NULL);
  if(rt != 0) { perror("Initialiser: couldn't initialise a mutex\n"); }                     // error message:
  // else { printf("initialiser: Successfully initialise a mutex\n");}                         // debugger:

  /*** Initialize condition variable ***/
  rt = pthread_cond_init(&sem.condition, NULL);
  if(rt != 0) { perror("Initialiser: couldn't initialise a condition variable\n"); }        // error message:
  // else { printf("initialiser: Successfully initialise a condition variable\n");}            // debugger:

  return 0; //return 0 if success
}

/*
 *  Fucntion: destructor
 *  Description: Cleans up semaphores, mutex, condition variable, and release memory allocation
 *  Parameter:  None
 *  Return: int 0 if success
 */
int destructor()
{

  /*** destruct the mutex ***/
  rt = pthread_mutex_destroy(&sem.mutex);
  if(rt != 0) { perror("destructor: couldn't destruct a mutex\n"); }                         // error message:
  // else { printf("destructor: Successfully destruct a mutex\n");}                             // debugger:

  /*** destruct the condition variable ***/
  rt = pthread_cond_destroy(&sem.condition);
  if(rt != 0) { perror("destructor: couldn't destruct a condition variable\n"); }            // error message:
  // else { printf("destructor: Successfully destruct a condition variable\n");}                             // debugger:

  return 0; //return 0 if success
}

/*
 *  Fucntion: procure (Same function as semWait())
 *  Description:
 *  Parameter:  Semaphore.
 *  Return: None
 */
void procure(Semaphore *semaphore)
{
  //   begin_critical_section(semaphore);  // make the following concurrency-safe
  //   while (semaphore->value <= 0)
	// wait_for_vacate(semaphore);     // wait for signal from vacate()
  //   semaphore->value--;                 // claim the Semaphore
  //   end_critical_section(semaphore);

  rt = pthread_mutex_lock(&semaphore->mutex);
  printf("procure: locking\n");
  printf("procure: pre value is: %d\n", semaphore->value);

  while(&semaphore->value <= 0) {
    printf("procure loop: locking\n");
    pthread_cond_wait(&semaphore->condition, &semaphore->mutex);
  }
  semaphore->value--;

  rt = pthread_mutex_unlock(&semaphore->mutex);
  printf("procure: post value is: %d\n", semaphore->value);

}


/*
 *  Fucntion: vacate (Same function as semSignal())
 *  Description:
 *  Parameter:  Semaphore.
 *  Return: None
 */
void vacate(Semaphore *semaphore)
{
//     begin_critical_section(semaphore);  // make the following concurrency-safe
//     semaphore->value++;                 // release the Semaphore
//     signal_vacate(semaphore);           // signal anyone waiting on this
//     end_critical_section(semaphore);

  printf("vacate: pre value is: %d\n", semaphore->value);   //debugger:
  // pthread_mutex_lock(&semaphore->mutex);
  semaphore->value++;
  pthread_cond_signal(&semaphore->condition);
  pthread_mutex_unlock(&semaphore->mutex);
  printf("vacate: post value is: %d\n", semaphore->value);    //debugger:

}



 void *printline (void *sem)
 {
   // int rt;
   Semaphore *sPtr = sem;    // type casting: void * back to correct type of struct

   procure(sPtr);     //problem here: can't not block this thread
   printf("Child: value is :%d\n", sPtr->value);
   printf("Child: printing in the child thread\n");
  //  vacate(sPtr);
  //  pthread_mutex_unlock(&sPtr->mutex);
   return NULL;
 }