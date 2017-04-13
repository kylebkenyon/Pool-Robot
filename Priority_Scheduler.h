#ifndef PRIORITY_SCHEDULER_H
#define PRIORITY_SCHEDULER_H

#include <queue>
#include "Process.h"
#include "Radar.h"
#include "GlobalCounter.h"
#include "FCFS_Scheduler.h"

#define PRIORITY_LEVELS 2

using namespace std;

class Priority_Scheduler {
    private:
      FCFS_Scheduler priority_array[PRIORITY_LEVELS];
    public:
      Priority_Scheduler();
      // Radar radar; //decide if it will be used
      bool addProcess(Process process);
      void next();
};

#endif
