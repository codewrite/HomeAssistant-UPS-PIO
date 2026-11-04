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
// 3. reapply power when safe to do so (if the battery appears to have enough charge).
void BatteryControl()
{
  // Battery Voltage: 12.9 100%, 12.8 90%, 12.65 80%, 12.5 70%, 12.4 60%, 12.2 50%, 12.1 40%, 12.0 30%, 11.8 20%
  // Battery Voltage charging: 13.65 100%

  // Scenario 1: Shutdown Requested
  if (pwrMon.shutdownNow && pwrMon.supplyVoltageAvg < 10.0 && pwrMon.rpiVoltageAvg < 2.0)
  {
    SetSupplyPower(false);
    SetBatteryPower(false);
  }

  // Scenario 2: Charge the battery after shutdown
  if (pwrMon.shutdownNow && pwrMon.supplyVoltageAvg > 10.0 && pwrMon.batteryVoltageAvg < 12.6)
  {
    SetBatteryCharging(255);
  }

  // Scenario 3: Battery charged enough to startup
  if (pwrMon.shutdownNow && pwrMon.supplyVoltageAvg > 10.0 && pwrMon.batteryVoltageAvg > 13.0)
  {
    SetBatteryPower(true);
    SetSupplyPower(true);
    pwrMon.shutdownNow = false;
  }
}

//***********************
// Command Methods
//***********************

void SetSupplyPower(String args)
{
  byte n1;
  GetNumber(args, n1);
  SetSupplyPower(n1>0);
  ReportSensors();
}

void SetSupplyPower(bool enable)
{
  digitalWrite(SupplyPowerPin, enable ? HIGH : LOW);
}

void SetBatteryPower(String args)
{
  byte n1;
  GetNumber(args, n1);
  SetBatteryPower(n1>0);
  ReportSensors();
}

void SetBatteryPower(bool enable)
{
  digitalWrite(BatteryPowerPin, enable ? HIGH : LOW);
}

void SetBatteryCharging(String args)
{
  byte n1;
  GetNumber(args, n1);
  SetBatteryCharging(n1);
  ReportSensors();
}

void SetBatteryCharging(byte val)
{
  pwrMon.chargingRate = val;
  analogWrite(BatteryChargingPin, val);
}

void SetBattery_mAh(String args)
{
  long n1;
  GetNumber(args, n1);
  pwrMon.battery_mAh = n1;
  ReportSensors();
}

void ShutdownPi(String args)
{
  byte n1;
  GetNumber(args, n1);
  pwrMon.shutdownNow = n1 > 0;
  ReportSensors();
}
