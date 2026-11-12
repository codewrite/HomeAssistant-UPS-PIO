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
void SetSupplyPower(bool enable);
void SetBatteryPower(String args);
void SetBatteryPower(bool enable);
void SetBatteryCharging(String args);
void SetBatteryCharging(byte val);
void SetBattery_mAh(String args);
void DisableAutoControl(String args);

#endif
