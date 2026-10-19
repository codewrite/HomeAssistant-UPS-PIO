#ifndef Task_h
#define Task_h
#include <Arduino.h>

typedef void(*taskFunctionPtr)();
typedef void(*durationFunctionPtr)();

class Task
{
  public:
    Task(long interval, taskFunctionPtr taskFn, long duration = 0, durationFunctionPtr durationFn = NULL);
    void Run();
    void SetInterval(long interval);
    void SetDuration(long duration);
  private:
    long _interval;
    long _duration;
    taskFunctionPtr _taskFn;
    taskFunctionPtr _durationFn;
    unsigned long _prevMillis;
    bool _taskStarted;
};
#endif
