#include <stdio.h>
#include <stdlib.h>
#include "header.h"

struct event
{
    int time;
    int job_number;
    int event_type;
    int timearrival:
};

struct cpu
{
    int device_state;
};

struct disk1
{
    int device_state;
};

struct disk2
{
    int device_state;
};

struct network
{
    int device_state;
};

int main()
{
    int curr_time = init_time;
    while (curr_time != fin_time)
    {

        curr_time++;
    }

    return 0;
}