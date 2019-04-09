/* Filename: rw.c 
* Name: Hannah Raju
* Date: 27 June 2018
* ID: V00797276
* Details: CSC 360 Assignment 2
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "rw.h"
#include "resource.h"

/* General References:
- https://stackoverflow.com/questions/23400097/c-confused-on-how-to-initialize-and-
implement-a-pthread-mutex-and-condition-vari
- https://linux.die.net/man/3/pthread_cond_init
*/


/*
 * Declarations for reader-writer shared variables -- plus concurrency-control
 * variables -- must START here.
 */
 

// Resource & Buffer
static resource_t data;

// Mutex
pthread_mutex_t m;
int m_status;

pthread_cond_t cond_r;
pthread_cond_t cond_w;

int reader;
int writer;
int active_writer;
	

void initialize_readers_writer() {
    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */
	 
	 // Initialize resource
	 init_resource(&data, "DATA");
	 
	// Declare and initialize mutex
	// Ref: 02_concurrency, slide 42
	m_status = pthread_mutex_init(&m, NULL);
	
	if(m_status != 0){
		fprintf(stderr, "Error creating mutex 'm'\n");
		exit(1);
	}
	

	
	// Initialize condition variables 
	// Ref: 02_concurrency, slide 62
	reader = pthread_cond_init(&cond_r, NULL);
	if(reader != 0){
		fprintf(stderr, "Could not initialize 'cond_r'\n");
		exit(1);
	}
	
	writer = pthread_cond_init(&cond_w, NULL);
	if(writer != 0){
		fprintf(stderr, "Could not initialize 'cond_w'\n");
		exit(1);
	}
	
	active_writer = 0;
}


// REFERENCE: 02_concurrency, slide 69
void rw_read(char *value, int len) {

	pthread_mutex_lock(&m);
	while(!(writer == 0)){
		pthread_cond_wait(&cond_r, &m);
	}
	reader++;
	
	pthread_mutex_unlock(&m);
	
	// read
	read_resource(&data, value, len);
	
	pthread_mutex_lock(&m);
	
	if(--reader == 0){
		pthread_cond_signal(&cond_w);
	}
	pthread_mutex_unlock(&m);

}

void rw_write(char *value, int len) {
	
	pthread_mutex_lock(&m);
	
	writer++;
	while(!((reader == 0) && (active_writer == 0))){
		pthread_cond_wait(&cond_w, &m);
	}
	
	active_writer++;
	
	pthread_mutex_unlock(&m);
	
	// write
	write_resource(&data, value, len);
	
	writer--;
	active_writer--;
	if(writer){
		pthread_cond_signal(&cond_w);
	}
	else{
		pthread_cond_broadcast(&cond_r);
	}
	
	pthread_mutex_unlock(&m);
}