#include <Arduino.h>
#include "Utils.h"
#include "Sensor-List.h"
#include "Control-List.h"

int SupplyPower = 3;
int BatteryPower = 2;
int BatteryCharging = 4;

void Control_Init()
{
  pinMode(SupplyPower, OUTPUT);
  pinMode(BatteryPower, OUTPUT);
  pinMode(BatteryCharging, OUTPUT);
  digitalWrite(SupplyPower, HIGH);
  digitalWrite(BatteryPower, HIGH);
  digitalWrite(BatteryCharging, LOW);
}

//***********************
// Task Methods
//***********************

//***********************
// Command Methods
//***********************

void SetSupplyPower(String args)
{
  int n1;
  GetNumber(args, n1);
  digitalWrite(SupplyPower, n1>0 ? HIGH : LOW);
  ReportSensors();
}

void SetBatteryPower(String args)
{
  int n1;
  GetNumber(args, n1);
  digitalWrite(BatteryPower, n1>0 ? HIGH : LOW);
  ReportSensors();
}

void SetBatteryCharging(String args)
{
  int n1;
  GetNumber(args, n1);
  digitalWrite(BatteryCharging, n1>0 ? HIGH : LOW);
  ReportSensors();
}
