#include "Process.h"
#include "Radar.h"

Process::Process(string n, bool iSE, Base_Process* bp) {
  name = n;
  isSingleEvent = iSE;
  baseProcess = bp;
}

Process::Process(string n, bool iSE, Base_Process* bp, int priorityIn) {
  name = n;
  isSingleEvent = iSE;
  baseProcess = bp;
  priority = priorityIn; 
}
