Shuao Chen
Section 4
Project 1 

The Data structures I will be using for this project are the followings:
●	Struct
●	Array
●	FIFO Queue
●	Priority Queue 

The system will start and it is represented as INIT_TIME = 0. Then the system will terminate once it reaches FIN_TIME (doesn’t matter if there 
are still jobs in the queues), which I am thinking 1000 for now. If it is too short I will adjust later.
Whenever a new job is created, it will be assigned with a random Arrival_time which represents the time the job will begin being processed. 
And the job will be put in a priority queue and be sorted based on Arrival_time. And the first in the queue will be taken out and put into 
the CPU to be used. 
The INIT_TIME will be 0 when the program starts, and the time will advance using a loop.
In a priority queue, an element with high priority is served before an element with low priority. And in this case, we are assigning each 
job with an Arrival_time, so the purpose of the priority queue is to see which job gets to be processed first. For example, if jobA arrives
 at time 4 and jobB arrives at time 8, jobA will be processed first. Events will be removed from the queue once the previous events are finished.
The source of entries will be randomized. The computer will keep on feeding the priority queue with objects with different attributes until 
the program stops. We only create jobs when a new job is put into the priority queue. In other words, whenever a job enters the FIFO queue 
before the CPU it will also create a job into the priority queue. This way we can make sure that the system will never stop before reaching 
FIN_TIME because of an empty priority queue.
The actual start time for a new process entering the system is determined by (the current time + randomly assigned Arrival_time).
