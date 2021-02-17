#ifndef HEADER_H_
#define HEADER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 20
#define MAX_QUEUE_SIZE 500

extern int job_counter;
extern int CPU_status;
extern int DISK1_status;
extern int DISK2_status;
extern int network_status;
extern int jobs_done;

typedef struct {

    int SEED;    
    int INIT_TIME;
    int FIN_time;
    int arrive_MIN;
    int arrive_MAX;
    int quit_prob;
    int network_prob;
    int CPU_MIN;
    int CPU_MAX;
    int DISK1_MIN;
    int DISK1_MAX;
    int DISK2_MIN;
    int DISK2_MAX;
    int network_MIN;
    int network_MAX;
    
} Config;

typedef struct Event{
    int time; 
    int jobNumber; 
    int type; 
} Event;

//queue of EVENTS
typedef struct node{
    //the pointer to the queue
    Event *ptr; 

    int next;  
    int last;   
    int size;   
} Queue;

typedef struct EventQueue{
    int capacity;           
    int size;               
    Event *eventListPtr;    
} EventQueue;

int get_val(char *buffer);
Config read_config(Config);
Queue *q_create();
int is_empty(Queue q1);
void q_push(Queue *q1, Event *e1);
Event *q_pop(Queue *q1);
Event* e_create(int time, int type, int job_no);
EventQueue *pq_create(int capacity);
void pq_push(EventQueue *pq, Event e);
void reheap_push(EventQueue *pq, int index);
void reheap_pop(EventQueue *pq, int index);
Event pq_pop(EventQueue *pq);
void pq_free(EventQueue *pq);
int pq_isEmpty(EventQueue *pq);
int pq_isFull(EventQueue *pq);
void pq_print(EventQueue *pq);
char *type_to_string(Event *e, char *buffer);
void print_config(FILE *ptr, Config values);
int random_gen(int MAX, int MIN);
int prob_select(int percent);
void process_CPU(Event *job, EventQueue *pq, Queue *CPU, Queue *DISK1, Queue *DISK2, Config values, FILE *ptr,Queue *network);
void process(Event *job, EventQueue *pq, Queue *DISK1, Queue *DISK2, Queue *network, FILE *ptr);

#endif 