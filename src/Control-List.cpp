#include <Arduino.h>
#include "Utils.h"
#include "Sensor-List.h"
#include "Control-List.h"

int SupplyPower = 3;
int BatteryPower = 2;
int BatteryCharging = 5;
long ControlOneSecondTickInterval = 1000;
long chargingRate = 0;

Task ControlOneSecondTickTask(ControlOneSecondTickInterval, ControlOneSecondTick);

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

void ControlOneSecondTick()
{

}

//***********************
// Command Methods
//***********************

void SetSupplyPower(String args)
{
  long n1;
  GetNumber(args, n1);
  digitalWrite(SupplyPower, n1>0 ? HIGH : LOW);
  ReportSensors();
}

void SetBatteryPower(String args)
{
  long n1;
  GetNumber(args, n1);
  digitalWrite(BatteryPower, n1>0 ? HIGH : LOW);
  ReportSensors();
}

void SetBatteryCharging(String args)
{
  //long n1;
  GetNumber(args, chargingRate);
  //digitalWrite(BatteryCharging, n1>0 ? HIGH : LOW);
  analogWrite(BatteryCharging, chargingRate);
  ReportSensors();
}
