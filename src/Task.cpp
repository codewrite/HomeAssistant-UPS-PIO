#include <Arduino.h>
#include <limits.h>
#include "Task.h"

Task::Task(int interval, taskFunctionPtr taskFn, int duration, durationFunctionPtr durationFn)
{
  _interval = interval;
  _duration = duration;
  _taskFn = taskFn;
  _durationFn = durationFn;
  _prevMillis = millis();
  _taskStarted = false;
}

void Task::Run()
{
  unsigned long currentMillis = millis();
  unsigned long delta = currentMillis >= _prevMillis ?
                        currentMillis - _prevMillis :
                        ULONG_MAX - _prevMillis + currentMillis + 1;
  if (!_taskStarted)
  {
    if (delta >= _interval)
    {
      _taskFn();
      _prevMillis = currentMillis;
      _taskStarted = (_durationFn != NULL);
    }
  }
  else //if (_durationFn != NULL)
  {
    if (delta >= _duration)
    {
      _durationFn();
      _prevMillis = currentMillis;
      _taskStarted = false;
    }
  }
}

void Task::SetInterval(long interval)
{
  _interval = interval;
}

void Task::SetDuration(long duration)
{
  _duration = duration;
}
