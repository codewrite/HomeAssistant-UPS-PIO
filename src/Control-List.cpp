#include <Arduino.h>
#include "Utils.h"
#include "PowerMonitor.h"
#include "Sensor-List.h"
#include "Control-List.h"

extern PowerMonitor pwrMon;

Task BatteryControlTask(1000, BatteryControl);

void Control_Init()
{
  pinMode(SupplyPowerPin, OUTPUT);
  pinMode(BatteryPowerPin, OUTPUT);
  pinMode(BatteryChargingPin, OUTPUT);
  digitalWrite(SupplyPowerPin, HIGH);
  digitalWrite(BatteryPowerPin, HIGH);
  digitalWrite(BatteryChargingPin, LOW);
}

//***********************
// Task Methods
//***********************

// This runs even when the supply is off.
// It's main role is to monitor the supply when it has been off and:
// 1. actually cut the power after "shutdown now" has been issued.
// 2. charge the battery (after the power has been restored).
// 2. reapply power when safe to do so (if the battery appears to have enough charge).
void BatteryControl()
{
  // Battery Voltage: 12.9 100%, 12.8 90%, 12.65 80%, 12.5 70%, 12.4 60%, 12.2 50%, 12.1 40%, 12.0 30%, 11.8 20%
  // Battery Voltage charging: 13.65 100%

}

//***********************
// Command Methods
//***********************

void SetSupplyPower(String args)
{
  byte n1;
  GetNumber(args, n1);
  digitalWrite(SupplyPowerPin, n1>0 ? HIGH : LOW);
  ReportSensors();
}

void SetBatteryPower(String args)
{
  byte n1;
  GetNumber(args, n1);
  digitalWrite(BatteryPowerPin, n1>0 ? HIGH : LOW);
  ReportSensors();
}

void SetBatteryCharging(String args)
{
  //byte n1;
  GetNumber(args, pwrMon.chargingRate);
  analogWrite(BatteryChargingPin, pwrMon.chargingRate);
  ReportSensors();
}

void SetBattery_mAh(String args)
{
  long n1;
  GetNumber(args, n1);
  pwrMon.battery_mAh = n1;
}

void ShutdownPi(String args)
{
  byte n1;
  GetNumber(args, n1);
  pwrMon.shutdownNow = n1 > 0;
}
