#include <Arduino.h>
#include <limits.h>
#include <Wire.h>
#include "Adafruit_INA3221.h"
#include "PowerMonitor.h"

Adafruit_INA3221 ina3221;
const float MA_PER_V_OFFSET = 0.15;       // e.g. 15V will read 2.25mA @ zero current
const float CURRENT_ADJUSTMENT = 0.99;    // multiply INA current reading to get more accurate value

void Ina3221Read(int channel, float& v, float& ma)
{
  // Current reading from INA3221 is about 1% too high. Voltage is within 0.5%
  // Current readings are affected by resistance of wiring.
  // MOSFET ON resistance is about 45m ohms
  v = ina3221.getBusVoltage(channel);
  ma = (ina3221.getCurrentAmps(channel) * 1000.0 - v * MA_PER_V_OFFSET) * CURRENT_ADJUSTMENT;
  // Serial.print(channel);
  // Serial.print(" ");
  // Serial.print(v);
  // Serial.print(" ");
  // Serial.print(ma);
  // Serial.println();
}

PowerMonitor::PowerMonitor()
{
}

void PowerMonitor::Init()
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

void PowerMonitor::Read()
{
  AccCount++;

  float rpiVoltage = analogRead(A1) * (150.0 + 47.0) / 150.0 * 5.0 / 1024.0;

  if (AccCount == 1)
  {
    rpiVoltageAvg = rpiVoltageMax = rpiVoltageMin = rpiVoltage;
  }
  else
  {
    rpiVoltageAvg = (rpiVoltageAvg * (AccCount - 1) + rpiVoltage) / AccCount;
    rpiVoltageMax = max(rpiVoltageMax, rpiVoltage);
    rpiVoltageMin = min(rpiVoltageMin, rpiVoltage);
  }

  float v, ma;

  Ina3221Read(0,v,ma);

  if (AccCount == 1)
  {
    supplyVoltageAvg = supplyVoltageMax = supplyVoltageMin = v;
    supplyCurrentAvg = supplyCurrentMax = supplyCurrentMin = ma;
  }
  else
  {
    supplyVoltageAvg = (v * (AccCount - 1) + v) / AccCount;
    supplyVoltageMax = max(supplyVoltageMax, v);
    supplyVoltageMin = min(supplyVoltageMin, v);
    supplyCurrentAvg = (ma * (AccCount - 1) + ma) / AccCount;
    supplyCurrentMax = max(supplyCurrentMax, ma);
    supplyCurrentMin = min(supplyCurrentMin, ma);
  }

  Ina3221Read(1,v,ma);

  if (AccCount == 1)
  {
    batteryVoltageAvg = batteryVoltageMax = batteryVoltageMin = v;
    supplyCurrentAvg = batteryCurrentMax = batteryCurrentMin = ma;
  }
  else
  {
    batteryVoltageAvg = (v * (AccCount - 1) + v) / AccCount;
    batteryVoltageMax = max(batteryVoltageMax, v);
    batteryVoltageMin = min(batteryVoltageMin, v);
    batteryCurrentAvg = (ma * (AccCount - 1) + ma) / AccCount;
    batteryCurrentMax = max(batteryCurrentMax, ma);
    batteryCurrentMin = min(batteryCurrentMin, ma);
  }


}

void PowerMonitor::Reset()
{
    AccCount = 0;
}
