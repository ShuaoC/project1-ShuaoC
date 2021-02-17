#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int statusCPU = 0, statusDisk1 = 0, statusDisk2 = 0, statusNetwork = 0;

int main()
{
    int time = 0;

    FILE *fp = fopen("output.txt", "w");
    Event *cur_job = (Event *)malloc(sizeof(Event));
    Config values;
    values = inputFile(values);

    srand(values.SEED);
    output(fp, values);

    EventQueue pq = *createEventQueue(10);
    Queue CPU = *createQueue();
    Queue DISK1 = *createQueue();
    Queue DISK2 = *createQueue();
    Queue network = *createQueue();

    Event *j1 = createEvent(values.INIT_TIME, 1, 1);
    priPush(&pq, *j1);
    Event *FIN = createEvent(values.FIN_TIME, 11, 0);
    priPush(&pq, *FIN);

    while (!isEmptyPriorityQueue(&pq) && (time < values.FIN_TIME))
    {
        *cur_job = popPriorityQueue(&pq);
        time = cur_job->time;

        switch (cur_job->type)
        {
        case 1:
            proCPU(cur_job, &pq, &CPU, &DISK1, &DISK2, values, fp, &network);
            break;
        case 4:
            process(cur_job, &pq, &DISK1, &DISK2, &network, fp);
            break;
        case 7:
            process(cur_job, &pq, &DISK1, &DISK2, &network, fp);
            break;
        case 10:
            process(cur_job, &pq, &DISK1, &DISK2, &network, fp);
            break;
        case 3:
            proCPU(cur_job, &pq, &CPU, &DISK1, &DISK2, values, fp, &network);
            break;
        case 6:
            process(cur_job, &pq, &DISK1, &DISK2, &network, fp);
            break;
        case 9:
            process(cur_job, &pq, &DISK1, &DISK2, &network, fp);
            break;
        case 12:
            process(cur_job, &pq, &DISK1, &DISK2, &network, fp);
            break;
        case 14:
            break;
        }

        if ((!(isEmpty(CPU))) && (statusCPU == 0))
        {
            Event *process;
            process = pop(&CPU);
            process->type = 2;
            char buffer[30];
            fprintf(fp, "\nAt time %d, job %d%s", process->time, process->jobNumber, toString(process, buffer));
            Event *job_fin = createEvent(time + random_gen(values.CPU_MIN, values.CPU_MAX), 3, process->jobNumber);
            priPush(&pq, *job_fin);
            statusCPU = 1;
        }

        if ((!(isEmpty(DISK1))) && (statusDisk1 == 0))
        {
            Event *process;
            process = pop(&DISK1);

            process->type = 5;
            char buffer[30];
            fprintf(fp, "\nAt time %d, job %d%s", process->time, process->jobNumber, toString(process, buffer));

            Event *job_fin = createEvent(time + random_gen(values.DISK1_MIN, values.DISK1_MAX), 13, process->jobNumber);
            priPush(&pq, *job_fin);
            statusDisk1 = 1;
        }

        if ((!(isEmpty(DISK2))) && (statusDisk2 == 0))
        {
            Event *process;
            process = pop(&DISK2);
            process->type = 8;
            char buffer[30];
            fprintf(fp, "\nAt time %d, job %d%s", process->time, process->jobNumber, toString(process, buffer));

            Event *job_fin = createEvent(time + random_gen(values.DISK2_MIN, values.DISK2_MAX), 13, process->jobNumber);
            priPush(&pq, *job_fin);
            statusDisk2 = 1;
        }
        if ((!(isEmpty(network))) && (statusNetwork == 0))
        {
            Event *process;
            process = pop(&network);

            process->type = 11;
            char buffer[30];
            fprintf(fp, "\nAt time %d, job %d%s ", process->time, process->jobNumber, toString(process, buffer));

            Event *job_fin = createEvent(time + random_gen(values.NETWORK_MIN, values.NETWORK_MAX), 13, process->jobNumber);
            priPush(&pq, *job_fin);
            statusNetwork = 1;
        }
    }

    printf("Program Finished\n");

    return 0;
}

int getValue(char *buffer)
{
    int i;
    int num;
    int value = 0;

    for (i = 3; i < strlen(buffer); i++)
    {
        if (buffer[i] >= '0' && buffer[i] <= '9')
        {
            num = buffer[i] - '0';
            value = value * 10 + num;
        }
    }
    return value;
}

Queue *createQueue()
{
    Queue *q1;
    q1 = (Queue *)malloc(sizeof(Queue));

    q1->ptr = (Event *)malloc(maxQsize * sizeof(Event));
    q1->last = 0;
    q1->next = 0;
    q1->size = 0;

    return q1;
}

void push(Queue *q1, Event *e1)
{
    q1->ptr[(q1->last) % maxQsize] = *e1;
    q1->last++;
    q1->size++;
}

Event *pop(Queue *q1)
{
    Event *e = &(q1->ptr[q1->next]);
    q1->next++;
    q1->size--;
    return e;
}

Event *createEvent(int time, int type, int job_no)
{
    Event *e;
    e = (Event *)malloc(sizeof(Event));

    e->jobNumber = job_no;
    e->time = time;
    e->type = type;

    return e;
}

int isEmpty(Queue q1)
{
    return (q1.size == 0);
}

Config inputFile(Config c1)
{
    char buffer[20];
    FILE *fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Cannot find file\n");
        exit(1);
    }
    
    fgets(buffer, maxChars, fp);
    c1.SEED = getValue(buffer);
    fgets(buffer, maxChars, fp);
    c1.INIT_TIME = getValue(buffer);
    fgets(buffer, maxChars, fp);
    c1.FIN_TIME = getValue(buffer);
    fgets(buffer, maxChars, fp);
    c1.ARRIVE_MIN = getValue(buffer);
    fgets(buffer, maxChars, fp);
    c1.ARRIVE_MAX = getValue(buffer);
    fgets(buffer, maxChars, fp);
    c1.QUIT_PROB = getValue(buffer);
    fgets(buffer, maxChars, fp);
    c1.NETWORK_PROB = getValue(buffer);
    fgets(buffer, maxChars, fp);
    c1.CPU_MIN = getValue(buffer);
    fgets(buffer, maxChars, fp);
    c1.CPU_MAX = getValue(buffer);
    fgets(buffer, maxChars, fp);
    c1.DISK1_MIN = getValue(buffer);
    fgets(buffer, maxChars, fp);
    c1.DISK1_MAX = getValue(buffer);
    fgets(buffer, maxChars, fp);
    c1.DISK2_MIN = getValue(buffer);
    fgets(buffer, maxChars, fp);
    c1.DISK2_MAX = getValue(buffer);
    fgets(buffer, maxChars, fp);
    c1.NETWORK_MIN = getValue(buffer);
    fgets(buffer, maxChars, fp);
    c1.NETWORK_MAX = getValue(buffer);

    fclose(fp);
    return c1;
}

EventQueue *createEventQueue(int capacity)
{
    EventQueue *pq = (EventQueue *)malloc(sizeof(EventQueue));

    pq->capacity = capacity;
    pq->size = 0;
    pq->eventListPtr = (Event *)malloc(sizeof(Event));

    return pq;
}

void priPush(EventQueue *pq, Event e)
{
    pq->eventListPtr[pq->size] = e;
    rePush(pq, pq->size);
    (pq->size)++;
}

void rePush(EventQueue *pq, int index)
{
    Event temp;
    int parent_node = (index - 1) / 2;

    if (pq->eventListPtr[parent_node].time > pq->eventListPtr[index].time)
    {
        temp = pq->eventListPtr[parent_node];
        pq->eventListPtr[parent_node] = pq->eventListPtr[index];
        pq->eventListPtr[index] = temp;
        rePush(pq, parent_node);
    }
}

void rePop(EventQueue *pq, int parent)
{
    int left = parent * 2 + 1;
    int right = parent * 2 + 2;
    int MIN;

    Event temp;
    if (left >= pq->size || left < 0)
        left = -1;
    if (right >= pq->size || right < 0)
        right = -1;

    if (left != -1 && pq->eventListPtr[left].time < pq->eventListPtr[parent].time)
        MIN = left;
    else
        MIN = parent;

    if (right != -1 && pq->eventListPtr[right].time < pq->eventListPtr[MIN].time)
        MIN = right;

    if (MIN != parent)
    {
        temp = pq->eventListPtr[MIN];
        pq->eventListPtr[MIN] = pq->eventListPtr[parent];
        pq->eventListPtr[parent] = temp;

        rePop(pq, MIN);
    }
}

Event popPriorityQueue(EventQueue *pq)
{
    Event e;
    e = pq->eventListPtr[0];
    pq->eventListPtr[0] = pq->eventListPtr[pq->size - 1];
    pq->size--;

    rePop(pq, 0);

    return e;
}

int isEmptyPriorityQueue(EventQueue *pq)
{
    return (pq->size == 0);
}

char *toString(Event *e, char *buffer)
{

    memset(buffer, 0, sizeof(*buffer));
    switch (e->type)
    {
    case 0:
        strcpy(buffer, " arrived the system");
        break;
    case 1:
        strcpy(buffer, " arrived CPU queue. \0");
        break;
    case 2:
        strcpy(buffer, " began processing at the CPU. \0");
        break;
    case 4:
        strcpy(buffer, " arrived at Disk 1. \0");
        break;
    case 5:
        strcpy(buffer, " began processing at Disk 1. \0");
        break;
    case 7:
        strcpy(buffer, " arrived at Disk 2. \0");
        break;
    case 8:
        strcpy(buffer, " began processing at Disk 2. \0");
        break;
    case 10:
        strcpy(buffer, "arrived at Network. \0");
        break;
    case 11:
        strcpy(buffer, " began processing at Network. \0");
        break;
    case 3:
        strcpy(buffer, " finished processing at the CPU. \0");
        break;
    case 6:
        strcpy(buffer, " finished processing at Disk 1. \0");
        break;
    case 9:
        strcpy(buffer, " finished processing at Disk 2. \0");
        break;
    case 12:
        strcpy(buffer, "finished processing at Network. \0");
        break;
    case 13:
        strcpy(buffer, " exited the system. \0");
        break;
    case 14:
        strcpy(buffer, " end the simulation. \0");
        break;
    }
    return buffer;
}

void output(FILE *ptr, Config vals)
{
    fprintf(ptr, "SEED %d\n", vals.SEED);
    fprintf(ptr, "INIT_TIME %d\n", vals.INIT_TIME);
    fprintf(ptr, "FIN_TIME %d\n", vals.FIN_TIME);
    fprintf(ptr, "ARRIVE_MIN %d\n", vals.ARRIVE_MIN);
    fprintf(ptr, "ARRIVE_MAX %d\n", vals.ARRIVE_MAX);
    fprintf(ptr, "QUIT_PROB %d\n", vals.QUIT_PROB);
    fprintf(ptr, "NETWORK_PROB %d\n", vals.NETWORK_PROB);
    fprintf(ptr, "CPU_MIN %d\n", vals.CPU_MIN);
    fprintf(ptr, "CPU_MAX %d\n", vals.CPU_MAX);
    fprintf(ptr, "DISK1_MIN %d\n", vals.DISK1_MIN);
    fprintf(ptr, "DISK1_MAX %d\n", vals.DISK1_MAX);
    fprintf(ptr, "DISK2_MIN %d\n", vals.DISK2_MIN);
    fprintf(ptr, "DISK1_MAX %d\n", vals.DISK1_MAX);
    fprintf(ptr, "NETWORK_MIN %d\n", vals.NETWORK_MIN);
    fprintf(ptr, "NETWORK_MAX %d\n", vals.NETWORK_MAX);
}

int random_gen(int MAX, int MIN)
{
    return (rand() % (MAX - MIN + 1)) + MIN;
}

int randProb(int percent)
{
    if ((percent < 0) || percent > 100)
    {
        exit(1);
    }
    else
    {
        int temp = random_gen(0, 100);
        if (temp <= percent)
            return 1;
        else
            return 0;
    }
}

void proCPU(Event *job, EventQueue *pq, Queue *CPU,
                 Queue *DISK1, Queue *DISK2, Config values, FILE *ptr, Queue *network)
{

    if (job->type == 1)
    {
        char buffer[30];
        fprintf(ptr, "\nAt time %d, job %d%s", job->time, job->jobNumber, toString(job, buffer));

        Event *next_job = createEvent(job->time + random_gen(values.ARRIVE_MIN, values.ARRIVE_MAX), 1, job->jobNumber + 1);
        priPush(pq, *next_job);

        push(CPU, job);
    }

    else
    {

        statusCPU = 0;
        char buffer[5];
        fprintf(ptr, "\nAt time %d, job %d%s", job->time, job->jobNumber, toString(job, buffer));

        if (!randProb(values.QUIT_PROB))
        {
            if (DISK1->size <= DISK2->size)
            {
                Event DISK1 = *createEvent(job->time, 4, job->jobNumber);
                priPush(pq, DISK1);
            }
            else
            {
                Event DISK2 = *createEvent(job->time, 7, job->jobNumber);
                priPush(pq, DISK2);
                if (!randProb(values.NETWORK_PROB))
                {
                    Event network = *createEvent(job->time, 10, job->jobNumber);
                    priPush(pq, network);
                }
            }
        }
        else
        {
            job->type = 3;
            char buffer[30];
            fprintf(ptr, "\nAt time %d, job %d%s", job->time, job->jobNumber, toString(job, buffer));
        }
    }
}

void process(Event *job, EventQueue *pq, Queue *DISK1, Queue *DISK2, Queue *network, FILE *ptr)
{

    if (job->type == 4)
    {

        push(DISK1, job);
        char buffer[30];
        fprintf(ptr, "\nAt time %d, job %d%s", job->time, job->jobNumber, toString(job, buffer));
    }
    else if (job->type == 7)
    {
        push(DISK2, job);
        char buffer[30];
        fprintf(ptr, "\nAt time %d, job %d%s", job->time, job->jobNumber, toString(job, buffer));
    }
    else if (job->type == 10)
    {
        push(network, job);
        char buffer[30];
        fprintf(ptr, "\nAt time %d, job %d%s", job->time, job->jobNumber, toString(job, buffer));
    }
    else if (job->type == 6)
    {
        char buffer[30];
        statusDisk1 = 0;

        job->type = 6;
        priPush(pq, *job);
        fprintf(ptr, "\nAt time %d, job %d%s", job->time, job->jobNumber, toString(job, buffer));
    }
    else if (job->type == 9)
    {

        char buffer[30];

        statusDisk2 = 0;
        job->type = 9;
        priPush(pq, *job);
        fprintf(ptr, "\nAt time %d, job %d%s", job->time, job->jobNumber, toString(job, buffer));
    }
    else if (job->type == 12)
    {
        char buffer[30];
        statusNetwork = 0;
        job->type = 12;
        priPush(pq, *job);
        fprintf(ptr, "\nAt time %d, job %d%s", job->time, job->jobNumber, toString(job, buffer));
    }
}
