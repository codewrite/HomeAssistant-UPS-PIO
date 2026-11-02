#ifndef Control_List_h
#define Control_List_h
#include <Arduino.h>
#include "Task.h"

// Pin definitions
#define SupplyPowerPin 3
#define BatteryPowerPin 2
#define BatteryChargingPin 5

extern Task BatteryControlTask;

void Control_Init();

//***********************
// Task Methods
//***********************

void BatteryControl();

//***********************
// Command Methods
//***********************

void SetSupplyPower(String args);
void SetBatteryPower(String args);
void SetBatteryCharging(String args);
void SetBattery_mAh(String args);
void ShutdownPi(String args);

#endif
