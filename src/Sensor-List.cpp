#include <Arduino.h>
#include <ArduinoJson.h>
#include "Utils.h"
#include "PowerMonitor.h"
#include "Control-List.h"
#include "Sensor-List.h"

extern PowerMonitor pwrMon;

long sensorReadInterval = 1000;
long sensorAvgCount = 30;
long SensorReportInterval = sensorAvgCount * sensorReadInterval;

Task ReadSensorTask(sensorReadInterval, ReadSensors);
Task ReportSensorTask(SensorReportInterval, ReportSensors);

void Sensor_Init()
{
}

//***********************
// Task Methods
//***********************

void ReadSensors()
{
  pwrMon.Read();
}

void ReportSensors()
{
/*
  long batVolt = batteryVoltageAccumulator / AccCount;
  if (batVolt < 10500L)
  {
    digitalWrite(SupplyPower, HIGH);
    chargingRate = 255;
    analogWrite(BatteryCharging, chargingRate);
  }
  if (batVolt > 14000L)
  {
    digitalWrite(SupplyPower, LOW);
    chargingRate = 0;
    analogWrite(BatteryCharging, chargingRate);
  }
*/
  JsonDocument jsonDoc;

  if (pwrMon.AccCount==0) ReadSensors();

  jsonDoc["rpi"]["va"] = pwrMon.rpiVoltageAvg;
  jsonDoc["rpi"]["vh"] = pwrMon.rpiVoltageMax;
  jsonDoc["rpi"]["vl"] = pwrMon.rpiVoltageMin;
  jsonDoc["vcc"]["va"] = pwrMon.supplyVoltageAvg;
  jsonDoc["vcc"]["vh"] = pwrMon.supplyVoltageMax;
  jsonDoc["vcc"]["vl"] = pwrMon.supplyVoltageMin;
  jsonDoc["vcc"]["ma"] = pwrMon.supplyCurrentAvg;
  jsonDoc["vcc"]["mh"] = pwrMon.supplyCurrentMax;
  jsonDoc["vcc"]["ml"] = pwrMon.supplyCurrentMin;
  jsonDoc["bat"]["va"] = pwrMon.batteryVoltageAvg;
  jsonDoc["bat"]["vh"] = pwrMon.batteryVoltageMax;
  jsonDoc["bat"]["vl"] = pwrMon.batteryVoltageMin;
  jsonDoc["bat"]["ma"] = pwrMon.batteryCurrentAvg;
  jsonDoc["bat"]["mh"] = pwrMon.batteryCurrentMax;
  jsonDoc["bat"]["ml"] = pwrMon.batteryCurrentMin;

  pwrMon.Reset();

  jsonDoc["bat"]["mah"] = pwrMon.battery_mAh;
  jsonDoc["Sensors"]["Interval"] = sensorReadInterval;
  jsonDoc["Sensors"]["AvgSamples"] = sensorAvgCount;
  jsonDoc["Control"]["Power"] = digitalRead(SupplyPowerPin);
  jsonDoc["Control"]["Battery"] = digitalRead(BatteryPowerPin);
  jsonDoc["Control"]["Charging"] = pwrMon.chargingRate;
  jsonDoc["Control"]["ShutdownNow"] = pwrMon.shutdownNow ? 1 : 0;

  serializeJson(jsonDoc, Serial);
  Serial.println();
}

//***********************
// Command Methods
//***********************

void UpdateSensorDutyCycle(String args)
{
  long n1, n2;
  Get2Numbers(args, n1, n2);
  if (n1 > 0 && n2 > 0)
  {
    sensorReadInterval = n1;
    sensorAvgCount = n2;
    SensorReportInterval = n2 * n1;
    ReadSensorTask.SetInterval(n1);
    ReportSensorTask.SetInterval(SensorReportInterval);
    ReportSensors();
  }
}
