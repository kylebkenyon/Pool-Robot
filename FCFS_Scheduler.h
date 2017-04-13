#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include <queue>
#include "Process.h"
#include "Radar.h"

using namespace std;

class FCFS_Scheduler {
    private:
      queue<Process> processes;
      const static int QUEUE_LIMIT = 10;
    public:
      FCFS_Scheduler();
      // Radar radar; //decide if it will be used
      bool addProcess(Process process);
      void next();
};

#endif
