//  3420ICT System Programming
//  SID: S2193948
//  Author: Chun Hung Wong (Grimes)
//  Program: main.c
//  Purpose: For execute the entire producer/consumer program


#include <stdio.h>    //Standard Input/Output library eg. getchar, gets, putchar, puts, sprintf
#include <stdlib.h>   //General Utilities library eg. atoi, atof, malloc, realloc
// #include <unistd.h>   //eg. sleep(), execvp(), environ(), pipe()
#include <string.h>   //String handling library eg. strcpy, strcat, strcmp
#include <pthread.h>  //eg. pthread_create, pthread_join, pthread_exit
#include "numGen.h"
#include "sema.h"


#define DEFAULT_MINFILL 0
#define DEFAULT_MAXBUFFER 10

/*** Function declaration (prototype) ***/
void *producer(void *arg);
void *consumer(void *arg);

/*** Global variable ***/
int rt = 0;  // for error debugging
int n = 0;
int printNum = 0;


int main()
{

  pthread_t producerT, consumerT;
  numConstructor(DEFAULT_MINFILL, DEFAULT_MAXBUFFER);                // numConstructor (maxBuf, minFill)


  /*** Create the producer child thread ***/
  rt = pthread_create(&producerT, NULL, producer, (void *) semPtr);              // create producer thread
  if(rt != 0) { perror("Main: Couldn't create the producer thread!!!"); }                  // error message:

  /*** Create the consumer thread ***/
  rt = pthread_create(&consumerT, NULL, consumer, (void *) semPtr);              // create consumer thread
  if(rt != 0) { perror("Main: Couldn't create the consumer thread!!!"); }                  // error message:

  // while loop here for waiting user input a number for print out how many numbers
  char readLine[31];
  char *linePtr = fgets(readLine, 30, stdin);
  if(strcmp(linePtr, "exit\n") == 0) {
    exit(EXIT_SUCCESS);
  }
  while(strcmp(linePtr,"exit\n") != 0) {
    printNum = atoi(linePtr);
    printf("Number of print is : %d\n", printNum);
    linePtr = fgets(readLine, 30, stdin);
  }






  /*** Wait the child thread to join ***/
  // rt = pthread_join(producerT, NULL);    //  wait to other thread to join
  // if (rt != 0) {
  //   perror("Producer thread(s) couldn't join the main thread\n");                 // error message:
  // }
  //
  // rt = pthread_join(consumerT, NULL);    //  wait to other thread to join
  // if (rt != 0) {
  //   perror("Consumer thread(s) couldn't join the main thread\n");                 // error message:
  // }

  numDestructor();       // destroy all the memory allocation

  pthread_exit(NULL);       // The last thing the main should do

  return 0;
}

/*  Function: producer(void *arg);
 *  Description: Generate the random number until maximum buffer is reached
 *  Parameter: pointer of data structure of Semaphore
 *  Return: None
 */
void *producer(void *arg) {
  numGenerator();               // numConstructor
  while(1) {
    procure(&semPtr->numMain);
    if (b.curLevel < b.maxBuf) {     // if current level reach the maximum buffer, signal consumer
      put_buffer();
    } else {
      vacate(&semPtr->numDelay);
    }
    vacate(&semPtr->numMain);
  }

  return NULL;
}
// TO-DO
// procure(&semPtr->numDelay);       // waiting the consumer signal for refilling the random number


/*  Function: numGenerator();
 *  Description: get the ranom number until minimum buffer is reached
 *  Parameter: pointer of data structure of Semaphore
 *  Return: None
 */
void *consumer(void *arg) {

  procure(&semPtr->numDelay);        // waiting for the producer first signal

  while(printNum > 0) {
    procure(&semPtr->numMain);

      if(b.minFill < b.curLevel) {
        printf("consumer: random number is %#06hx\n", b.numPtr[b.indexOut]);
      } else {
        vacate(&semPtr->numDelay);
      }

    vacate(&semPtr->numMain);
    printNum--;
  }

  return NULL;
}

// TO-DO
// vacate(&semPtr->numDelay)        // if minimun fill is reach, signal producer

/*** testing get_buffer ***/
// printf("Main: get_buffer() = %u\n", get_buffer());
// printf("Main: get_buffer() = %u\n", get_buffer());
// printf("Main: get_buffer() = %u\n", get_buffer());
// printf("Main: get_buffer() = %u\n", get_buffer());
// printf("Main: indexIn : %d\n", b.indexIn);          // debugger:
// printf("Main: indexOut : %d\n", b.indexOut);        // debugger:
// printf("Main: curLevel : %d\n", b.curLevel);        // debugger:
/*** end testing get_buffer ***/

/*** testing the member of the array ***/
// for(int i=0; i<b.maxBuf; i++) {
//   printf("Main: b.numPtr[%d] is %d, hex = %#06hx, address = %p\n", i, b.numPtr[i], b.numPtr[i], &b.numPtr[i]);
// }
/*** end testing the member of the array ***/


/*** note only ***/
// printf("65261 in hex is 0x%04x\n", 65261);    // note: format of hex
// printf("65261 in hex is %#06x\n", 65261);     // note: format of hex
/*** end of note ***/
