#include <Arduino.h>
#include <ArduinoJson.h>
#include "Utils.h"
#include "Sensor-List.h"

extern int LedBlinkInterval;
extern int LedBlinkDuration;

int sensorReadInterval = 1000;
int sensorAvgCount = 30;
int SensorReportInterval = sensorAvgCount * sensorReadInterval;
long lightLevelAccumulator = 0;
int lightLevelAccCount = 0;

Task ReadSensorTask(sensorReadInterval, ReadSensors);
Task ReportSensorTask(SensorReportInterval, ReportSensors);

//***********************
// Task Methods
//***********************

void ReadSensors()
{
  lightLevelAccumulator += analogRead(A0);
  lightLevelAccCount++;
}

void ReportSensors()
{
  JsonDocument jsonDoc;

  if (lightLevelAccCount==0) ReadSensors();

  jsonDoc["lightlevel"] = lightLevelAccumulator / lightLevelAccCount;
  lightLevelAccumulator = 0;
  lightLevelAccCount = 0;
  jsonDoc["Sensor"]["Interval"] = sensorReadInterval;
  jsonDoc["Sensor"]["AvgSamples"] = sensorAvgCount;
  jsonDoc["LedBlink"]["Interval"] = LedBlinkInterval;
  jsonDoc["LedBlink"]["Duration"] = LedBlinkDuration;

  serializeJson(jsonDoc, Serial);
  Serial.println();
}

//***********************
// Command Methods
//***********************

void UpdateSensorDutyCycle(String args)
{
  int n1, n2;
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
