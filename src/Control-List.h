#ifndef Control_List_h
#define Control_List_h
#include <Arduino.h>
#include "Task.h"

extern Task BlinkLedTask;

//***********************
// Task Methods
//***********************

void SetLED();
void ClearLED();

//***********************
// Command Methods
//***********************

void UpdateLedDutyCycle(String args);

#endif
