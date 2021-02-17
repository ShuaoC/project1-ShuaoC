#ifndef HEADER_H_
#define HEADER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxChars 30
#define maxQsize 700

extern int statusCPU;
extern int statusDisk1;
extern int statusDisk2;
extern int statusNetwork;

typedef struct {

    int SEED;    
    int INIT_TIME;
    int FIN_TIME;
    int ARRIVE_MIN;
    int ARRIVE_MAX;
    int QUIT_PROB;
    int NETWORK_PROB;
    int CPU_MIN;
    int CPU_MAX;
    int DISK1_MIN;
    int DISK1_MAX;
    int DISK2_MIN;
    int DISK2_MAX;
    int NETWORK_MIN;
    int NETWORK_MAX;
    
} Config;

typedef struct Event{
    int time; 
    int jobNumber; 
    int type; 
} Event;

typedef struct node{
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

int getValue(char *buffer);
Config inputFile(Config);
Queue *createQueue();
int isEmpty(Queue q1);
void push(Queue *q1, Event *e1);
Event *pop(Queue *q1);
Event* createEvent(int time, int type, int job_no);
EventQueue *createEventQueue(int capacity);
void priPush(EventQueue *pq, Event e);
void rePush(EventQueue *pq, int index);
void rePop(EventQueue *pq, int index);
Event popPriorityQueue(EventQueue *pq);
int isEmptyPriorityQueue(EventQueue *pq);
char *toString(Event *e, char *buffer);
void output(FILE *ptr, Config values);
int random_gen(int MAX, int MIN);
int randProb(int percent);
void proCPU(Event *job, EventQueue *pq, Queue *CPU, Queue *DISK1, Queue *DISK2, Config values, FILE *ptr,Queue *network);
void process(Event *job, EventQueue *pq, Queue *DISK1, Queue *DISK2, Queue *network, FILE *ptr);

#endif 