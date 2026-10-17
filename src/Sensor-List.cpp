#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "Adafruit_INA3221.h"
#include "Utils.h"
#include "Control-List.h"
#include "Sensor-List.h"

Adafruit_INA3221 ina3221;

int sensorReadInterval = 1000;
int sensorAvgCount = 30;
int SensorReportInterval = sensorAvgCount * sensorReadInterval;
long piVoltageAccumulator = 0;
long supplyVoltageAccumulator = 0;
long supplyCurrentAccumulator = 0;
long batteryVoltageAccumulator = 0;
long batteryCurrentAccumulator = 0;
int AccCount = 0;

Task ReadSensorTask(sensorReadInterval, ReadSensors);
Task ReportSensorTask(SensorReportInterval, ReportSensors);

void Sensor_Init()
{
  if (!ina3221.begin(0x40, &Wire))       // can use other I2C addresses or buses
  {
    Serial.println("Failed to find INA3221 chip");
    for (uint8_t i = 0; i < 3; i++)
      ina3221.setShuntResistance(i, 0.1);
  }
  else
    Serial.println("INA3221 Found!");
}

//***********************
// Task Methods
//***********************

void ReadSensors()
{
  piVoltageAccumulator += analogRead(A1) * (150.0 + 47.0) / 150.0 * 4.7 / 1024.0 * 1000.0;       // 1 = 1mV
  AccCount++;
  supplyVoltageAccumulator += ina3221.getBusVoltage(0) * 1000.0;     // 1 = 1mV
  supplyCurrentAccumulator += ina3221.getCurrentAmps(0) * 1000.0;    // 1 = 1mA
  batteryVoltageAccumulator += ina3221.getBusVoltage(1) * 1000.0;    // 1 = 1mV
  batteryCurrentAccumulator += ina3221.getCurrentAmps(1) * 1000.0;   // 1 = 1mA
}

void ReportSensors()
{
  JsonDocument jsonDoc;

  if (AccCount==0) ReadSensors();

  jsonDoc["rpi"]["v"] = piVoltageAccumulator / AccCount;
  jsonDoc["vcc"]["v"] = supplyVoltageAccumulator / AccCount;
  jsonDoc["vcc"]["a"] = supplyCurrentAccumulator / AccCount;
  jsonDoc["bat"]["v"] = batteryVoltageAccumulator / AccCount;
  jsonDoc["bat"]["a"] = batteryCurrentAccumulator / AccCount;
  piVoltageAccumulator = 0;
  supplyVoltageAccumulator = 0;
  supplyCurrentAccumulator = 0;
  batteryVoltageAccumulator = 0;
  batteryCurrentAccumulator = 0;
  AccCount = 0;
  jsonDoc["Sensors"]["Interval"] = sensorReadInterval;
  jsonDoc["Sensors"]["AvgSamples"] = sensorAvgCount;
  jsonDoc["Control"]["Power"] = digitalRead(SupplyPower);
  jsonDoc["Control"]["Battery"] = digitalRead(BatteryPower);
  jsonDoc["Control"]["Charging"] = digitalRead(BatteryCharging);

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
