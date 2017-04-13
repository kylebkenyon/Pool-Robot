#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "Base_Process.h"

using namespace std;

class Process {
    // private:
    public:
      Process(string n, bool iSE, Base_Process* bp);
      Process(string n, bool iSE, Base_Process* bp, int priorityIn);
      string name;
      int priority;
      Base_Process * baseProcess;
      bool isSingleEvent;
};

#endif
