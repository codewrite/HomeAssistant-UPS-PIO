#include <Arduino.h>
#include <ArduinoJson.h>
#include "Utils.h"
#include "PowerMonitor.h"
#include "Control-List.h"
#include "Sensor-List.h"

extern long chargingRate;

PowerMonitor pwrMon;

long sensorReadInterval = 1000;
long sensorAvgCount = 30;
long SensorReportInterval = sensorAvgCount * sensorReadInterval;

double battery_mAh = 0;

Task ReadSensorTask(sensorReadInterval, ReadSensors);
Task ReportSensorTask(SensorReportInterval, ReportSensors);

void Sensor_Init()
{
  pwrMon.Init();
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

  jsonDoc["rpi"]["v"] = pwrMon.rpiVoltageAvg;
  jsonDoc["vcc"]["v"] = pwrMon.supplyVoltageAvg;
  jsonDoc["vcc"]["a"] = pwrMon.supplyCurrentAvg;
  jsonDoc["bat"]["v"] = pwrMon.batteryVoltageAvg;
  jsonDoc["bat"]["a"] = pwrMon.batteryCurrentAvg;

  pwrMon.Reset();

  jsonDoc["bat"]["mah"] = battery_mAh / 1000.0;
  jsonDoc["Sensors"]["Interval"] = sensorReadInterval;
  jsonDoc["Sensors"]["AvgSamples"] = sensorAvgCount;
  jsonDoc["Control"]["Power"] = digitalRead(SupplyPower);
  jsonDoc["Control"]["Battery"] = digitalRead(BatteryPower);
  jsonDoc["Control"]["Charging"] = chargingRate;

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
