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
  char buf[30];
  StaticJsonDocument<100> jsonBuffer;

  if (lightLevelAccCount==0) ReadSensors();

  jsonBuffer["lightlevel"] = lightLevelAccumulator / lightLevelAccCount;
  lightLevelAccumulator = 0;
  lightLevelAccCount = 0;
  jsonBuffer["Sensor"]["Interval"] = sensorReadInterval;
  jsonBuffer["Sensor"]["AvgSamples"] = sensorAvgCount;
  jsonBuffer["LedBlink"]["Interval"] = LedBlinkInterval;
  jsonBuffer["LedBlink"]["Duration"] = LedBlinkDuration;

  serializeJson(jsonBuffer, Serial);
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
