#include "FCFS_Scheduler.h"

FCFS_Scheduler::FCFS_Scheduler() {

}

bool FCFS_Scheduler::addProcess(Process process) {
  if (processes.size() == QUEUE_LIMIT) {
    return false;
  }

  processes.push(process);
  return true;
}

void FCFS_Scheduler::next() {
  Process current = processes.front();
  processes.pop();
  (current.baseProcess)->callback();
  if (!current.isSingleEvent) {
    addProcess(current);
  }
}
