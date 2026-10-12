#include <Arduino.h>
#include "Task.h"

Task::Task(int interval, taskFunctionPtr taskFn, int duration, durationFunctionPtr durationFn)
{
  _interval = interval;
  _duration = duration;
  _taskFn = taskFn;
  _durationFn = durationFn;
  _prevMillis = millis();
  _durationTime = -1;
}

void Task::Run()
{
  long currentMillis = millis();
  if (currentMillis - _prevMillis >= _interval)
  {
    _taskFn();
    _prevMillis = currentMillis;
    if (_durationFn != NULL)
    {
      _durationTime = currentMillis + _duration;
    }
  }
  else if (_durationTime >= 0 && currentMillis >= _durationTime)
  {
    _durationFn();
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
