# Multi-Level-Feedback-Queue
A Scheduler that learns from the past to predict the future using Round Robin, Priority Boosting and Time allocation

This scheduler follows 5 rules: -
  1) Suppose A and B are 2 jobs. If Priority(A) > Priority(B), A runs (B doesn’t).
  2) If Priority(A) = Priority(B), A & B run in Round Robin.
  3) When a job enters the system, it is placed at the highest priority.
  4) Once a job uses up its time allotment at a given level (regardless of how many times it has given up the CPU), its priority is              reduced(i.e., it moves down on queue).
  5) After some time period S, move all the jobs in the system to the topmost queue.

This program is written for linux. Queues and Linkedlist have been used to form the Multi Level Queue. 

Different test cases are also provided to check the program. 
