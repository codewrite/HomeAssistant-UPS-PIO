#ifndef Control_List_h
#define Control_List_h
#include <Arduino.h>
#include "Task.h"

extern int SupplyPower;
extern int BatteryPower;
extern int BatteryCharging;

void Control_Init();

//***********************
// Task Methods
//***********************

//***********************
// Command Methods
//***********************

void SetSupplyPower(String args);
void SetBatteryPower(String args);
void SetBatteryCharging(String args);

#endif
