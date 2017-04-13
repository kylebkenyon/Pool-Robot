#include "Priority_Scheduler.h"

Priority_Scheduler::Priority_Scheduler() {
  for (int i = 0; i < PRIORITY_LEVELS; i++) {
    priority_array[i] = FCFS_Scheduler();
  }
}

bool Priority_Scheduler::addProcess(Process process) {
  return priority_array[process.priority -1].addProcess(process);
}

void Priority_Scheduler::next() {
  priority_array[0].next();
  if(global_counter%5 == 0)
    priority_array[1].next();
}
