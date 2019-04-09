/* Filename: meetup.c
* Name: Hannah Raju
* Date: 27 June 2018
* ID: V00797276
* Details: CSC 360 Assignment 2
*/


/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "meetup.h"
#include "resource.h"


/* General References
* - https://docs.oracle.com/cd/E19455-01/806-5257/sync-42602/index.html
* - https://stackoverflow.com/questions/6331301/implementing-an-n-process-
*	barrier-using-semaphores
- https://www.youtube.com/watch?v=fCFndi19yNs
- pseudocode from the Little Book of Semaphores

*/

/*
 * Declarations for barrier shared variables -- plus concurrency-control
 * variables -- must START here.
 */
 

sem_t mutex;
sem_t bar1;
sem_t bar2;
sem_t arr;

int count;
int pshared;
int ret;
int mut_val;
int bar1_val;
int bar2_val;
int arr_val;
 
int num;
int option; 
resource_t codeword;



void initialize_meetup(int n, int mf) {
    char label[100];
    int i;

    if (n < 1) {
        fprintf(stderr, "Who are you kidding?\n");
        fprintf(stderr, "A meetup size of %d??\n", n);
        exit(1);
    }

    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */
	 
	 // Initialize num & option
	 num = n;
	 option = mf;
	 
	 // Initialize resource
	 init_resource(&codeword, "CODEWORD");
	 
	 // Initialize semaphores
	 pshared = 0;
	 
	 mut_val = 1;
	 bar1_val = 0;
	 bar2_val = 1;
	 arr_val = n;
	 
	 ret = sem_init(&mutex, pshared, mut_val);
	 if(ret != 0){
		 fprintf(stderr, "Error initializing semaphore: 'mutex'\n");
		 exit(1);
	 }
	 
	 ret = sem_init(&bar1, pshared, bar1_val);
	 if(ret != 0){
		 fprintf(stderr, "Error initializing semaphore: 'bar1'\n");
		 exit(1);
	 }
	 
	 ret = sem_init(&bar2, pshared, bar2_val);
	 if(ret != 0){
		 fprintf(stderr, "Error initializing semaphore: 'bar2\n");
		 exit(1);
	 }
	 
	 ret = sem_init(&arr, pshared, arr_val);
	 if(ret != 0){
		 fprintf(stderr, "Error initializing semaphore: 'arr\n");
		 exit(1);
	 }
	 
	 // Initialize count
	 count = 0;
	 
}


void join_meetup(char *value, int len) {
	
	
	// need to wait twice : once for all to arrive, and once to read resource
	// sem b1 for arriving
	// sem b2 for leaving
	
	// block anything more than n
	
	sem_wait(&arr);
	
	sem_wait(&mutex);
	

	count++;

	// MEETFIRST
	if(option ==1 && count ==  1){
		
		write_resource(&codeword, value, len);
	}
	
	
	// MEETLAST
	if(option ==0 && count == num){
		write_resource(&codeword, value, len);
	}
	
	// last thread to enter locks 2nd barrier in prep for leaving and 
	// unlocks the first barrer as all N are in
	if(count == num){
		
		sem_wait(&bar2);
		sem_post(&bar1);
	}

	
	
	sem_post(&mutex);
	
	sem_wait(&bar1);
	sem_post(&bar1);
	
	read_resource(&codeword, value, len);
	
	sem_wait(&mutex);
	
	
	count--;
	

	// last thread to leave
	if(count == 0){
		sem_wait(&bar1);
		sem_post(&bar2);
	}
	sem_post(&mutex);
	

	sem_wait(&bar2);
	sem_post(&bar2);
	
	sem_post(&arr);
	
}
