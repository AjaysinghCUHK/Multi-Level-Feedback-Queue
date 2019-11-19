/*
Name: - Ajay Sing Ramsingh Raghuwanshi
SID: - 1155083332
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab5_queue.h"

void outprint(int time_x, int time_y, int pid, int arrival_time, int remaining_time);

void scheduler(Process* proc, LinkedQueue** ProcessQueue, int proc_num, int queue_num, int period){
    printf("Process number: %d\n", proc_num);
    for (int i = 0;i < proc_num; i++)
        printf("%d %d %d\n", proc[i].process_id, proc[i].arrival_time, proc[i].execution_time);

    printf("\nQueue number: %d\n", queue_num);
    printf("Period: %d\n", period);
    for (int i = 0;i < queue_num; i++){
        printf("%d %d %d\n", i, ProcessQueue[i]->time_slice, ProcessQueue[i]->allotment_time);
    }

    int current_time = 0;
    int done = 0; 
    int chaser = 0;
    int procs = 0;
    int boostcount = 1;
    int procs_in_sys = 0;
    int procs_waiting = proc_num;

    if(proc_num != 0)
    {
        if(queue_num != 0)
        {
            current_time = proc[0].arrival_time; //setting current time to first process arrival time
            while(done == 0)
            {

                for (int i = procs; i < proc_num; i++) //add new process to HP Queue rule 3
                {   
                    if(current_time >= proc[i].arrival_time)
                    {   proc[i].service_time = 0;
                        EnQueue(ProcessQueue[queue_num - 1], proc[i]);
                        //printf("enqueuing process %d\n", FrontQueue(ProcessQueue[queue_num - 1]).process_id);
                        procs++;
                        procs_in_sys++;
                        procs_waiting--;
                    }
                }
                if(current_time >= period) //boost
                {   boostcount++;
                    //printf("boosting\n");
                    chaser = chaser + period; 
                    current_time = current_time % period;
                    for (int i = queue_num - 2;i > -1; i--) //run through each queue
                    {
                        int len =  Length(ProcessQueue[i]);
                        Process* test2 = (Process*)malloc(1*sizeof(Process));
                        for(int j = 0; j < len; j++)
                            {
                                 test2[0] = DeQueue(ProcessQueue[i]);
                                 test2[0].service_time = 0;
                                 EnQueue(ProcessQueue[queue_num - 1],  test2[0]);
                            }
                    }
                }
                for (int i = queue_num - 1;i > -1; i--) //run through each queue
                {
                    if(IsEmptyQueue(ProcessQueue[i])) {continue; } //rule 1
                    else
                    {
                        //printf("queue num %d, queue timeslice %d\n",i,ProcessQueue[i]->time_slice);
                        int len = Length(ProcessQueue[i]);
                        Process* procbeta = (Process*)malloc(proc_num * sizeof(Process));
                        Process* sortproc = (Process*)malloc(proc_num * sizeof(Process));
                        Process* test = (Process*)malloc(len*sizeof(Process));
                        for(int x = 0; x < len; x++)
                        {
                            sortproc[x] = DeQueue(ProcessQueue[i]);
                        }

                         for (int u = 0; u < len; u++) 
                        {
 
                            for (int v = u + 1; v < len; v++)
                            {
                                if(sortproc[u].process_id > sortproc[v].process_id)
                                {

                                    test[0] = sortproc[u];
                                    sortproc[u] = sortproc[v];
                                    sortproc[v]= test[0];

                                }
                            }
                        }

                        for(int x = 0; x < len; x++)
                        {  
                            EnQueue(ProcessQueue[i], sortproc[x]);
                        }

                        for(int j = 0; j < len; j++)
                        {
                                int initial_time = current_time;
                                procbeta[j].process_id = FrontQueue(ProcessQueue[i]).process_id;
                                procbeta[j].execution_time = FrontQueue(ProcessQueue[i]).execution_time;
                                procbeta[j].service_time = FrontQueue(ProcessQueue[i]).service_time;
                                procbeta[j].arrival_time = FrontQueue(ProcessQueue[i]).arrival_time;

                                int adder; 

                                if(procbeta[j].execution_time < (ProcessQueue[i]->time_slice)) adder = procbeta[j].execution_time;
                                else adder = ProcessQueue[i]->time_slice;

                                if((current_time+chaser+adder)>(period*boostcount))
                                {   
                                    int diff = adder-((current_time+chaser+adder) - (period*boostcount));
                                    current_time += diff;
                                    procbeta[j].service_time = 0;
                                    procbeta[j].execution_time -= diff;
                                }
                                else if(procbeta[j].execution_time < (ProcessQueue[i]->time_slice)) 
                                {
                                    current_time += procbeta[j].execution_time;
                                    procbeta[j].service_time +=  procbeta[j].execution_time;
                                    procbeta[j].execution_time = 0;
                                }
                                else
                                {
                                    current_time += (ProcessQueue[i]->time_slice);
                                    procbeta[j].service_time += (ProcessQueue[i]->time_slice);
                                    procbeta[j].execution_time -= (ProcessQueue[i]->time_slice);
                                }

                                if(procbeta[j].execution_time == 0)
                                {   DeQueue(ProcessQueue[i]);
                                    procs_in_sys--;
                                }
                                else if(((procbeta[j].service_time) >= (ProcessQueue[i]->allotment_time))&&(i != 0))
                                {
                                            procbeta[j].service_time = 0;
                                            EnQueue(ProcessQueue[i-1],  procbeta[j]);
                                            DeQueue(ProcessQueue[i]);
                                }
                                else if(procbeta[j].execution_time > 0)
                                {
                                    EnQueue(ProcessQueue[i], procbeta[j]);
                                    DeQueue(ProcessQueue[i]);
                                }

                                outprint(initial_time + chaser, current_time + chaser,(procbeta[j].process_id),(procbeta[j].arrival_time),(procbeta[j].execution_time));
                                int flag = 0;
                                for (int t = procs; t < proc_num; t++) //add new process to HP Queue rule 3
                                {
                                    if(current_time >= proc[t].arrival_time)
                                    {
                                        flag = 1;
                                    }
                                }
                                if(flag == 1)
                                {
                                    break;
                                }
                    }
                    break;
                    }
                }//queue loop
                if((procs_in_sys+procs_waiting)==0)
                {
                done = 1;
                }
            }//while loop
        }
    }
}

