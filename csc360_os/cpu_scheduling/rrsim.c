/* 
 * Name: Hannah Raju
 * ID: V00797276
 * Date: 17 July 2018
 * Details: CSC 360 Assignment #3
 */

/* BONUS WORK
 * - added ability for tasks to be added out of order
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

#define MAX_BUFFER_LEN 80

taskval_t *event_list = NULL;

double wait_time = 0.0;
double turnaround = 0.0;

void print_task(taskval_t *t, void *arg) {
    printf("task %03d: %5d %3.2f %3.2f\n",
        t->id,
        t->arrival_time,
        t->cpu_request,
        t->cpu_used
    );  
}


void increment_count(taskval_t *t, void *arg) {
    int *ip;
    ip = (int *)arg;
    (*ip)++;
}

/* My code here */
void run_simulation(int qlen, int dlen) {
	

	// define a linked list for ready queue
    taskval_t *ready_q = NULL;
	
	// define a pointer for event_list;
	taskval_t *ptr;
	
	
	// determine length of event list
	int num_tasks =0;
	if(event_list != NULL){
		num_tasks = 1;
	}
	
	for(ptr=event_list; ptr->next != NULL; ptr=ptr->next){
		num_tasks++;
	}
	
	
	//define counter for compelted tasks
	int tasks_complete = 0;
		
	//	exit function if no tasks specified
	if(event_list == NULL){
		fprintf(stderr, "No tasks specified. Specify at least one task to run simulation.\n");
		exit(1);
	}
	 
	// initialize tick counter 
	int t;
	t = 0;
	
	ptr = event_list;

	
	// run idle while ready queue is empty
	while(ready_q == NULL){
		
		
		// once i equal to 1st task arrival time, add to ready queue
		if(t == ptr->arrival_time){
			
			event_list = remove_front(event_list);
			ready_q = add_end(ready_q, ptr);
			
			break;
		}

		printf("[%05d] IDLE\n", t);
		
		t++;
		
	}
	
	
	// declare variable for dispatch count
	int dcount = 0;
	
	// declare variable for quantum count
	int qcount = 0;
	
	// pointer node for task currently running
	taskval_t *curr; 	

	// main loop for simulation
	while(tasks_complete < num_tasks){
		
		
		//point ptr to head of event_list, once event_list is NULL, no longer have to find new tasks
		if(event_list != NULL){
			
			ptr = event_list;

			// iterate thru event_list, see any tasks arrive at current time, add them to ready q
			while(ptr->arrival_time == t){
							
				event_list = remove_front(event_list);
				ready_q = add_end(ready_q, ptr);
				
				if(event_list != NULL){
					ptr = event_list;
				}
				else{
					break;
				}
			}
		}
		
		// if ready q is null, idle
		if(ready_q == NULL){
			printf("[%05d] IDLE\n", t);
		}
		

		// dispatch if flag on & increment counter
		else if(dcount < dlen){
		
			printf("[%05d] DISPATCHING\n", t);
			dcount++;
		}
		
		// run until quantum used up
		else if (qcount < qlen){
			
			
			// run process at beginning of ready q
			curr = ready_q;
			
			// if current task is done, inc complete count and remove from ready q
			if(curr->cpu_used > curr->cpu_request){
				
				printf("[%05d] id=%05d EXIT w=%.2f ta=%.2f\n",t,curr->id, t-curr->arrival_time-curr->cpu_request, (double)t-curr->arrival_time); 
				
				//add results to data
				wait_time += t-curr->arrival_time-curr->cpu_request;
				turnaround += t-curr->arrival_time;
				
				tasks_complete++;
				ready_q = remove_front(ready_q);
				qcount = 0;
				dcount = 0;
				
				// if there are still tasks to be completed, dispatch
				if (tasks_complete < num_tasks){
					printf("[%05d] DISPATCHING\n", t);
					dcount++;
				}
				
				// no more tasks, compute results
				// results printed to results.txt: 
				// quantum, dispatch, avg_wait_time, avg_turnaround
				else if(tasks_complete == num_tasks){
					FILE *f = fopen("a3results.txt", "a");
					if (f == NULL){
						fprintf(stderr, "Error: could not open file for writing.\n");
						exit(1);
					}
					fprintf(f, "%d,%d,%.2f,%.2f\n", qlen, dlen,  wait_time/num_tasks, turnaround/num_tasks);
			
					fclose(f);
				}
			
			}
			
			// else if task isn't done yet and quantum not used up, print the update
			else{
				printf("[%05d] id=%05d req=%.2f used=%.2f\n",t,curr->id, curr->cpu_request, curr->cpu_used);
				qcount++;
				curr->cpu_used++;
				
				// if quantum used up but task not complete, move it to back of ready q
				if(qcount == qlen){			
			
					ready_q = remove_front(ready_q);
					ready_q = add_end(ready_q, curr);
					
					qcount = 0;
					dcount = 0;
				}
			}
		
			
		}
		
		
		else{
			
			fprintf(stderr, "Error: shouldn't be here");
			
		}
		
		
		t++;
		
	}
	
	

}


int main(int argc, char *argv[]) {
    char   input_line[MAX_BUFFER_LEN];
    int    i;
    int    task_num;
    int    task_arrival;
    float  task_cpu;
    int    quantum_length = -1;
    int    dispatch_length = -1;

    taskval_t *temp_task;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--quantum") == 0 && i+1 < argc) {
            quantum_length = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "--dispatch") == 0 && i+1 < argc) {
            dispatch_length = atoi(argv[i+1]);
        }
    }

    if (quantum_length == -1 || dispatch_length == -1) {
        fprintf(stderr, 
            "usage: %s --quantum <num> --dispatch <num>\n",
            argv[0]);
        exit(1);
    }


    while(fgets(input_line, MAX_BUFFER_LEN, stdin)) {
        sscanf(input_line, "%d %d %f", &task_num, &task_arrival,
            &task_cpu);
        temp_task = new_task();
        temp_task->id = task_num;
        temp_task->arrival_time = task_arrival;
        temp_task->cpu_request = task_cpu;
        temp_task->cpu_used = 0.0;
        event_list = add_end(event_list, temp_task);
    }

#ifdef DEBUG
    int num_events;
    apply(event_list, increment_count, &num_events);
    printf("DEBUG: # of events read into list -- %d\n", num_events);
    printf("DEBUG: value of quantum length -- %d\n", quantum_length);
    printf("DEBUG: value of dispatch length -- %d\n", dispatch_length);
#endif

    run_simulation(quantum_length, dispatch_length);

    return (0);
}
