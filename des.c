#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int job_counter = -1;
int CPU_status = 0;
int DISK1_status = 0;
int DISK2_status = 0;
int network_status = 0;
int jobs_done = 0;

int main() {
    // Read File
    FILE *log = fopen("logs.txt","w");
    
    int time = 0;
    Event *cur_job = (Event*)malloc(sizeof(Event));
    Config values;
    // Read in the values from the input file
    values = read_config(values);
    
    int CPU_len = 0;
    int DISK1_len = 0;
    int DISK2_len = 0;
    int pq_len = 0;
    int network_len = 0;
    int count = 0;
    int MAXpq = 0;
    int MAXCPU = 0;
    int MAXDISK1 = 0;
    int MAXDISK2 = 0;
    int MAXnetwork = 0;
    //SEED the random generator
    srand(values.SEED);
    //prints config vals to log.txt
    print_config(log, values);
    
    EventQueue pq = *pq_create(10);
    Queue CPU = *q_create();
    Queue DISK1 = *q_create();
    Queue DISK2 = *q_create();
    Queue network = *q_create();

    Event *j1 = e_create(values.INIT_TIME, 1, 1);
    pq_push(&pq, *j1);
    Event *FIN = e_create(values.FIN_time , 11, 0);
    pq_push(&pq, *FIN);
    
    while(!pq_isEmpty(&pq) && (time < values.FIN_time)){
        count ++;
        //pulls job out of pq
        *cur_job = pq_pop(&pq);
        //sets global time to time of event
        time = cur_job->time;
        
        pq_len += pq.size;
        CPU_len += CPU.size;
        DISK1_len += DISK1.size;
        DISK2_len += DISK2.size;
        network_len += network.size;
        
        if(pq.size > MAXpq)
            MAXpq = pq.size;
        if(MAXCPU < CPU.size)
            MAXCPU = CPU.size;
        if(MAXDISK1 < DISK1.size)
            MAXDISK1 = DISK1.size;
        if(MAXDISK2 < DISK2.size)
            MAXDISK2 = DISK2.size;
        if(MAXnetwork < network.size)
            MAXnetwork = network.size;
        
        
        switch (cur_job->type){
            //began the simulation
            case 1: 
                process_CPU(cur_job, &pq, &CPU, &DISK1, &DISK2, values, log,&network);
                break;
            //DISK1 arrival
            case 4: 
                process(cur_job, &pq, &DISK1, &DISK2, &network, log);
                break;
            //DISK2 arrival
            case 7: 
                process(cur_job, &pq, &DISK1, &DISK2, &network,log);
                break;
            //network arrival
            case 10:
                process(cur_job, &pq,  &DISK1, &DISK2, &network, log);
                break;
            //CPU finish
            case 3: 
                process_CPU(cur_job, &pq, &CPU, &DISK1, &DISK2, values, log,&network);
                break;
            //DISK1 finish
            case 6: 
                process(cur_job, &pq, &DISK1, &DISK2, &network, log);
                break;
            //DISK2 finish
            case 9: 
                process(cur_job, &pq, &DISK1, &DISK2, &network, log);
                break;
            //network finish
            case 12: 
                process(cur_job, &pq,  &DISK1, &DISK2, &network, log);
                break;
            //end of simulation
            case 14:break;
        }
        
        //proceeding CPU
        if((!(is_empty(CPU))) && (CPU_status == 0)){
            Event *process;
            //get the task from FIFO (lost after this)
            process = q_pop(&CPU);
            process->type = 2;
            char buffer[30];
            fprintf( log,"\nTime %d Job %d%s" , process->time, process->jobNumber , type_to_string(process, buffer));
            Event *job_fin = e_create(time + random_gen(values.CPU_MIN,values.CPU_MAX) , 3, process->jobNumber);
            //corresponding finish event
            pq_push(&pq, *job_fin);
            CPU_status = 1;
        }
        
        //proceeding Disk 1
        if((!(is_empty(DISK1))) && (DISK1_status == 0)){
            Event *process;
            process = q_pop(&DISK1);

            process->type = 5;
            char buffer[30];
            fprintf(log, "\nTime %d Job %d%s" , process->time, process->jobNumber , type_to_string(process, buffer));
            
            Event *job_fin = e_create(time + random_gen(values.DISK1_MIN,values.DISK1_MAX) , 13, process->jobNumber);
            //corresponding finish event
            pq_push(&pq, *job_fin);
            DISK1_status = 1;
        }
        
        //proceeding Disk 2
        if((!(is_empty(DISK2))) && (DISK2_status == 0)){
            Event *process;
            //get the task from FIFO (does nothing with this)
            process = q_pop(&DISK2);
            process->type = 8;
            char buffer[30];
            fprintf(log, "\nTime %d Job %d%s" , process->time, process->jobNumber , type_to_string(process, buffer));
            
            Event *job_fin = e_create(time + random_gen(values.DISK2_MIN,values.DISK2_MAX) , 13, process->jobNumber);//set up finish time for the job
            pq_push(&pq, *job_fin);
            //corresponding finish event
            DISK2_status = 1;
        }
        //proceeding network
        if((!(is_empty(network))) && (network_status == 0)){
            Event *process;
            process = q_pop(&network);
            
            process->type = 11;
            char buffer[30];
            fprintf( log,"\nTime %d Job %d%s ", process->time, process->jobNumber , type_to_string(process, buffer));
            
            Event *job_fin = e_create(time +random_gen(values.network_MIN, values.network_MAX), 13, process->jobNumber);
            pq_push(&pq, *job_fin);
            network_status = 1;
        }
        
    }
    
    CPU_len /= count;
    DISK1_len /= count;
    DISK2_len /= count;
    network_len /= count;
    pq_len /= count;
    
    Event finalJob = pq_pop(&pq);
    printf("\n---Results---");
    printf("\nPQ average length: %d" , pq_len);
    printf("\nCPU average length: %d" , CPU_len);
    printf("\nDISK1 average length: %d" , DISK1_len);
    printf("\nDISK2 average length: %d" , DISK2_len);
    printf("\nnetwork average length: %d" , network_len);
    printf("\nMAX pq: %d" , MAXpq);
    printf("\nMAX CPU: %d" , MAXCPU);
    printf("\nMAX DISK1: %d" , MAXDISK1);
    printf("\nMAX DISK2: %d" , MAXDISK2);
    printf("\nMAX network: %d" , MAXnetwork);
    printf("\nServiced %d jobs in %d units\n~%.4f jobs per unit time" 
            , finalJob.jobNumber, time, ((float)finalJob.jobNumber)/time);
    printf("\n\n");

    return 0;
}

int get_val(char *buffer) {
    int i;
    int num;
    int value = 0;

    for (i = 3; i < strlen(buffer); i++) {
        if (buffer[i] >= '0' && buffer[i] <= '9') {
            num = buffer[i] - '0';
            value = value * 10 + num;
        }
    }
    return value;
}


Queue *q_create() {
    Queue *q1;
    q1 = (Queue*)malloc(sizeof(Queue));
    
    q1->ptr = (Event*) malloc(MAX_QUEUE_SIZE * sizeof(Event));
    q1->last = 0;
    q1->next = 0;
    q1->size = 0;
    
    return q1;
}


void q_push(Queue *q1, Event *e1){
    if(q1->size == MAX_QUEUE_SIZE){
        printf("Queue is full, cannot push element onto queue. ");
        exit(2);
    }
    else{
        q1->ptr[(q1->last) % MAX_QUEUE_SIZE] = *e1;
        q1->last++;
        q1->size++;
    }
}


Event *q_pop(Queue *q1){
    if(q1->size == 0){
        printf("Queue is empty, nothing to pull from queue. ");
        exit(3);
    }
    else{
        Event *e = &(q1->ptr[q1->next]);
        q1->next++;
        q1->size--;
        return e;
    }
    
}


Event* e_create(int time, int type, int job_no){
     //pointer to new event being created
    Event *e;
    e = (Event *) malloc(sizeof(Event));
    
    e->jobNumber = job_no;
    e->time = time;
    e->type = type;
    
    return e;
}

int is_empty(Queue q1) {
    return (q1.size == 0);
}

Config read_config(Config c1) {
    //buffer for each input string
    char buffer[20]; 
    FILE *fp = fopen("input.txt", "r"); 

    if (fp == NULL) {
        printf("File cannot be opened\n");
        exit(1);
    }

    //grab a line from the input file, and extract the integer
    fgets(buffer, MAX_CHARS, fp);
    c1.SEED = get_val(buffer);
    fgets(buffer, MAX_CHARS, fp);
    c1.INIT_TIME = get_val(buffer);
    fgets(buffer, MAX_CHARS, fp);
    c1.FIN_time = get_val(buffer);
    fgets(buffer, MAX_CHARS, fp);
    c1.arrive_MIN = get_val(buffer);
    fgets(buffer, MAX_CHARS, fp);
    c1.arrive_MAX = get_val(buffer);
    fgets(buffer, MAX_CHARS, fp);
    c1.quit_prob = get_val(buffer);
    fgets(buffer, MAX_CHARS, fp);
    c1.network_prob = get_val(buffer);
    fgets(buffer, MAX_CHARS, fp);
    c1.CPU_MIN = get_val(buffer);
    fgets(buffer, MAX_CHARS, fp);
    c1.CPU_MAX = get_val(buffer);
    fgets(buffer, MAX_CHARS, fp);
    c1.DISK1_MIN = get_val(buffer);
    fgets(buffer, MAX_CHARS, fp);
    c1.DISK1_MAX = get_val(buffer);
    fgets(buffer, MAX_CHARS, fp);
    c1.DISK2_MIN = get_val(buffer);
    fgets(buffer, MAX_CHARS, fp);
    c1.DISK2_MAX = get_val(buffer);
    fgets(buffer, MAX_CHARS, fp);
    c1.network_MIN = get_val(buffer);
    fgets(buffer, MAX_CHARS, fp);
    c1.network_MAX =get_val(buffer);

    fclose(fp);
    return c1;
}



// Constructor for a priority queue
EventQueue *pq_create(int capacity){
    EventQueue *pq = (EventQueue *)malloc(sizeof(EventQueue));
    
    if(pq == NULL){
        printf("Memory Error!");
        exit(4);
    }
    
    pq->capacity = capacity;
    pq->size = 0;
    pq->eventListPtr = (Event *)malloc(sizeof(Event));
    
    if(pq->eventListPtr == NULL){
        printf("Memory Error!");
        exit(4);
    }
    
    return pq;
}

// Adds an element in order to priority queue
void pq_push(EventQueue *pq, Event e){
    if(pq_isFull(pq)){
        printf("\n***PRIORITY QUEUE IS FULL***\ncreate a bigger pq capacity\n");
        exit(4);
    }
    pq->eventListPtr[pq->size] = e;
    reheap_push(pq, pq->size);
    (pq->size)++;
}


void reheap_push(EventQueue *pq, int index){
    Event temp;   //temp storage
    int parent_node = (index-1)/2; //index of parent

    if(pq->eventListPtr[parent_node].time > pq->eventListPtr[index].time){
        //switch the elements and recursively call
        temp = pq->eventListPtr[parent_node];
        pq->eventListPtr[parent_node] = pq->eventListPtr[index];
        pq->eventListPtr[index] = temp;
        reheap_push(pq,parent_node);
    }
}


void reheap_pop(EventQueue *pq, int parent){
    //location of left node
    int left = parent*2+1;
    int right = parent*2+2;
    //the MIN of the two children
    int MIN;
    
    Event temp;

    //make sure that left and right are in bounds
    if(left >= pq->size || left <0)
        left = -1;
    if(right >= pq->size || right <0)
        right = -1;

    
    if(left != -1 && pq->eventListPtr[left].time < pq->eventListPtr[parent].time)
        MIN = left;
    else
        MIN = parent;
    
    if(right != -1 && pq->eventListPtr[right].time < pq->eventListPtr[MIN].time)
        MIN = right;

    //if MIN is a child 
    if(MIN != parent){
        temp = pq->eventListPtr[MIN];
        pq->eventListPtr[MIN] = pq->eventListPtr[parent];
        pq->eventListPtr[parent] = temp;

        // recursive  call
        reheap_pop(pq, MIN);
    }
}


Event pq_pop(EventQueue *pq){
    Event e;
    if(pq_isEmpty(pq)){
        printf("\nQueue is Empty\n");
        exit(5);
    }
    // replace first node by last and delete last
    e = pq->eventListPtr[0];
    pq->eventListPtr[0] = pq->eventListPtr[pq->size-1];
    pq->size--;
    
    reheap_pop(pq, 0);
    
    return e;
}


void pq_free(EventQueue *pq){
    free(pq);
}


int pq_isEmpty(EventQueue *pq){
    return(pq->size == 0);
}


int pq_isFull(EventQueue *pq){
    return (pq->size == pq->capacity);
}


void pq_print(EventQueue *pq){
    int i;
    printf("\n_______Event Queue_______\n");
    for(i=0;i< pq->size;i++){
        printf("%-5d| %-4s%d|%-20d\n" , pq->eventListPtr[i].time, "Job" , pq->eventListPtr[i].jobNumber, pq->eventListPtr[i].type);
    }                                                                                               
}


char* type_to_string(Event *e, char *buffer){

    //clear the buffer
    memset(buffer, 0, sizeof(*buffer));
    switch(e->type){
        case 0:
            strcpy(buffer, " entered the system");
            break;
        case 1:
            strcpy(buffer, " entered CPU queue. \0");
            break;
        case 2:
            strcpy(buffer, " began using the CPU. \0");
            break;
        case 4:
            strcpy(buffer, " arrived at Disk 1. \0");
            break;
        case 5:
            strcpy(buffer, " began using Disk 1. \0");
            break;
        case 7:
            strcpy(buffer, " arrived at Disk 2. \0");
            break;
        case 8:
            strcpy(buffer, " began using Disk 2. \0");
            break;
        case 10:
            strcpy(buffer, "arrived at Network. \0");
            break;
        case 11:
            strcpy(buffer, " began using Network. \0");
            break;
        case 3:
            strcpy(buffer, " finished using the CPU. \0");
            break;
        case 6:
            strcpy(buffer, " finished using Disk 1. \0");
            break;
        case 9:
            strcpy(buffer, " finished using Disk 2. \0");
            break;
        case 12:
            strcpy(buffer, "finished using Network. \0");
            break;
        case 13:
            strcpy(buffer, " exited the system. \0");
            break;
        case 14:
            strcpy(buffer, " end the simulation. \0");
            break;
        default:
            printf("\nEvent type has mistakes. \n");
            exit(7);
            break;
    }
    return buffer;
}

void print_config(FILE *ptr, Config vals){
    fprintf(ptr, "SEED %d\n", vals.SEED);
    fprintf( ptr,"INIT_TIME %d\n", vals.INIT_TIME);
    fprintf( ptr,"FIN_TIME %d\n", vals.FIN_time);
    fprintf(ptr, "ARRIVE_MIN %d\n", vals.arrive_MIN);
    fprintf( ptr,"ARRIVE_MAX %d\n", vals.arrive_MAX);
    fprintf(ptr, "QUIT_PROB %d\n", vals.quit_prob);
    fprintf( ptr,"NETWORK_PROB %d\n", vals.network_prob);
    fprintf( ptr,"CPU_MIN %d\n", vals.CPU_MIN);
    fprintf(ptr, "CPU_MAX %d\n", vals.CPU_MAX);
    fprintf(ptr, "DISK1_MIN %d\n" , vals.DISK1_MIN);
    fprintf( ptr,"DISK1_MAX %d\n" , vals.DISK1_MAX);
    fprintf( ptr,"DISK2_MIN %d\n" , vals.DISK2_MIN);
    fprintf( ptr,"DISK1_MAX %d\n" , vals.DISK1_MAX);
    fprintf( ptr,"Network_MIN %d\n" , vals.network_MIN);
    fprintf( ptr,"Network_MAX %d\n" , vals.network_MAX);
}

// Creates a random number between MAX and MIN
int random_gen(int MAX, int MIN){
    return (rand() % (MAX - MIN + 1)) + MIN;
}

int prob_select(int percent){
    if((percent < 0) || percent > 100){
        printf("%d is an invalid probability. exiting." , percent);
        exit(8);
    }
    else{
        int temp = random_gen(0,100);
        if(temp <= percent)
            return 1;
        else
            return 0;
    }
}

void process_CPU(Event *job, EventQueue *pq, Queue *CPU,
        Queue *DISK1, Queue *DISK2, Config values, FILE *ptr,Queue *network){

    //if job is an arrival
    if (job->type == 1){
        //arrived into system.
        char buffer[30];
        fprintf( ptr,"\nTime %d Job %d%s" , job->time, job->jobNumber , type_to_string(job, buffer));
        
        Event *next_job= e_create(job->time + random_gen(values.arrive_MIN, values.arrive_MAX), 1, job->jobNumber + 1); 
        //enqueue the new job
        pq_push(pq, *next_job);
        
        q_push(CPU , job);
    }

    //if job is done 
    else{

        CPU_status = 0;
        //job is done at CPU
        char buffer[5];
        fprintf(ptr,"\nTime %d Job %d%s" , job->time, job->jobNumber , type_to_string(job, buffer));

        if (!prob_select(values.quit_prob)){
                // DISK1 has a shorter line
                if(DISK1->size <= DISK2->size){
                    Event DISK1 = *e_create(job->time , 4, job->jobNumber);
                    // Add DISK1 arrival to pq
                    pq_push(pq, DISK1);
                }else{
                    Event DISK2 = *e_create(job->time , 7, job->jobNumber);
                    // Add DISK2 arrival to pq
                    pq_push(pq, DISK2);
                    if(!prob_select(values.network_prob)){
                    Event network = *e_create(job-> time , 10,job-> jobNumber);
                    pq_push(pq,network);
                    
                }  
            
            }
                
        }
        //job is finished
        else{
            job->type = 3;

            //write finish event to log
            char buffer[30];
            fprintf(ptr,"\nTime %d Job %d%s" , job->time, job->jobNumber , type_to_string(job, buffer));
        }
    }
}

// Processing in the DISK
void process(Event *job, EventQueue *pq, Queue *DISK1, Queue *DISK2, Queue *network, FILE *ptr){

    //arrival DISK1
    if(job->type == 4){

        q_push(DISK1, job);
        //write DISK1 arrival
        char buffer[30];
        fprintf(ptr,"\nTime %d Job %d%s" , job->time, job->jobNumber , type_to_string(job, buffer));
    }
    //arrival DISK2
    else if(job->type == 7){
        q_push(DISK2, job);
        //write DISK2 arrival
        char buffer[30];
        fprintf(ptr,"\nTime %d Job %d%s" , job->time, job->jobNumber , type_to_string(job, buffer));
    }
    else if(job->type == 10){
        q_push(network,job);
        //Write network arrival
        char buffer[30];
        fprintf(ptr,"\nTime %d Job %d%s", job->time, job->jobNumber, type_to_string(job, buffer));
    
    }
    else if(job->type == 6){
        //write CPU re-arrival
        char buffer[30];
        //disk 1 is now idle
        DISK1_status = 0;
        
        job->type = 6;
        pq_push(pq, *job);
        fprintf( ptr,"\nTime %d Job %d%s" , job->time, job->jobNumber , type_to_string(job, buffer));
    }else if(job->type == 9){

        //write CPU re-arrival
        char buffer[30];

        DISK2_status = 0;
        //push same job back into CPU with CPU arrival status
        job->type = 9;
        pq_push(pq, *job);
        fprintf(ptr,"\nTime %d Job %d%s" , job->time, job->jobNumber , type_to_string(job, buffer));
        
    }else if(job ->type == 12){
        char buffer[30];
        //network is now idle
        network_status = 0;
        job->type = 12;
        pq_push(pq, *job);
        fprintf(ptr,"\nTime %d Job %d%s" , job->time, job->jobNumber , type_to_string(job, buffer));
        
        
    }

}
