#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "Adafruit_INA3221.h"
#include "Utils.h"
#include "Control-List.h"
#include "Sensor-List.h"

extern int chargingRate;

Adafruit_INA3221 ina3221;
const float MA_PER_V_OFFSET = 0.15;       // e.g. 15V will read 2.25mA @ zero current

int sensorReadInterval = 1000;
int sensorAvgCount = 30;
int SensorReportInterval = sensorAvgCount * sensorReadInterval;
long piVoltageAccumulator = 0;
long supplyVoltageAccumulator = 0;
long supplyCurrentAccumulator = 0;
long batteryVoltageAccumulator = 0;
long ch2CurrentAccumulator = 0;
long ch2VoltageAccumulator = 0;
long batteryCurrentAccumulator = 0;
int AccCount = 0;

Task ReadSensorTask(sensorReadInterval, ReadSensors);
Task ReportSensorTask(SensorReportInterval, ReportSensors);

void Sensor_Init()
{
  if (!ina3221.begin(0x40, &Wire))       // can use other I2C addresses or buses
  {
    Serial.println("Failed to find INA3221 chip");
  }
  else
  {
    Serial.println("INA3221 Found!");
    for (uint8_t i = 0; i < 3; i++)
    {
      ina3221.setShuntResistance(i, 0.1);
    }
  }
}

void Ina3221Read(int channel, float& mv, float& ma)
{
  float v = ina3221.getBusVoltage(channel);
  ma = ina3221.getCurrentAmps(channel) * 1000.0 - v * MA_PER_V_OFFSET;
  mv = v * 1000.0;
  // Serial.print(channel);
  // Serial.print(" ");
  // Serial.print(mv);
  // Serial.print(" ");
  // Serial.print(ma);
  // Serial.println();
}

//***********************
// Task Methods
//***********************

void ReadSensors()
{
  piVoltageAccumulator += analogRead(A1) * (150.0 + 47.0) / 150.0 * 4.7 / 1024.0 * 1000.0;       // 1 = 1mV
  AccCount++;
  float ma, mv;
  Ina3221Read(0,mv,ma);
  supplyVoltageAccumulator += mv;        // 1 = 1mV
  supplyCurrentAccumulator += ma * 10;   // 1 = 100uA
  Ina3221Read(1,mv,ma);
  batteryVoltageAccumulator += mv;       // 1 = 1mV
  batteryCurrentAccumulator += ma * 10;  // 1 = 100uA
  Ina3221Read(2,mv,ma);
  ch2VoltageAccumulator += mv;           // 1 = 1mV
  ch2CurrentAccumulator += ma * 10;      // 1 = 100uA
  //Serial.println(ina3221.getShuntVoltage(0),6);
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
  jsonDoc["ch2"]["v"] = ch2VoltageAccumulator / AccCount;
  jsonDoc["ch2"]["a"] = ch2CurrentAccumulator / AccCount;
  piVoltageAccumulator = 0;
  supplyVoltageAccumulator = 0;
  supplyCurrentAccumulator = 0;
  batteryVoltageAccumulator = 0;
  batteryCurrentAccumulator = 0;
  ch2VoltageAccumulator = 0;
  ch2CurrentAccumulator = 0;
  AccCount = 0;
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
