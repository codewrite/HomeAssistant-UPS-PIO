#ifndef Task_h
#define Task_h
#include <Arduino.h>

typedef void(*taskFunctionPtr)();
typedef void(*durationFunctionPtr)();

class Task
{
  public:
    Task(unsigned long interval, taskFunctionPtr taskFn, unsigned long duration = 0, durationFunctionPtr durationFn = NULL);
    void Run();
    void SetInterval(long interval);
    void SetDuration(long duration);
  private:
    unsigned long _interval;
    unsigned long _duration;
    taskFunctionPtr _taskFn;
    taskFunctionPtr _durationFn;
    unsigned long _prevMillis;
    bool _taskStarted;
};
#endif
