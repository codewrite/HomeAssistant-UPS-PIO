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
  JsonDocument rpiDoc, vccDoc, batDoc, controlDoc;

  if (pwrMon.AccCount==0) ReadSensors();

  JsonObject rpi = rpiDoc["rpi"].to<JsonObject>();
  rpi["va"] = pwrMon.rpiVoltageAvg;
  rpi["vh"] = pwrMon.rpiVoltageMax;
  rpi["vl"] = pwrMon.rpiVoltageMin;

  JsonObject vcc = vccDoc["vcc"].to<JsonObject>();
  vcc["va"] = pwrMon.supplyVoltageAvg;
  vcc["vh"] = pwrMon.supplyVoltageMax;
  vcc["vl"] = pwrMon.supplyVoltageMin;
  vcc["ma"] = pwrMon.supplyCurrentAvg;
  vcc["mh"] = pwrMon.supplyCurrentMax;
  vcc["ml"] = pwrMon.supplyCurrentMin;

  JsonObject bat = batDoc["bat"].to<JsonObject>();
  bat["va"] = pwrMon.batteryVoltageAvg;
  bat["vh"] = pwrMon.batteryVoltageMax;
  bat["vl"] = pwrMon.batteryVoltageMin;
  bat["ma"] = pwrMon.batteryCurrentAvg;
  bat["mh"] = pwrMon.batteryCurrentMax;
  bat["ml"] = pwrMon.batteryCurrentMin;
  bat["mah_m"] = pwrMon.battery_mAh_measured;
  bat["mah_c"] = pwrMon.battery_mAh_calculated;

  pwrMon.Reset();

  JsonObject sensors = controlDoc["Sensors"].to<JsonObject>();
  sensors["Interval"] = sensorReadInterval;
  sensors["AvgSamples"] = sensorAvgCount;
  JsonObject control = controlDoc["Control"].to<JsonObject>();
  control["Power"] = digitalRead(SupplyPowerPin);
  control["Battery"] = digitalRead(BatteryPowerPin);
  control["Charging"] = pwrMon.chargingRate;
  control["AutoControlDisable"] = pwrMon.autoControlDisable ? 1 : 0;

  serializeJson(rpiDoc, Serial);
  Serial.println();
  serializeJson(vccDoc, Serial);
  Serial.println();
  serializeJson(batDoc, Serial);
  Serial.println();
  serializeJson(controlDoc, Serial);
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
