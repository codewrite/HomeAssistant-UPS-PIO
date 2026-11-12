#include <Arduino.h>
#include <limits.h>
#include <Wire.h>
#include "Adafruit_INA3221.h"
#include "PowerMonitor.h"

extern long sensorReadInterval;

Adafruit_INA3221 ina3221;
const float MA_PER_V_OFFSET = 0.15;       // e.g. 15V will read 2.25mA @ zero current
const float CURRENT_ADJUSTMENT = 0.99;    // multiply INA current reading to get more accurate value
const float BATTERY_LEAKAGE_MA = 0.4;     // Self discharge of battery (in mA)
const float MILLIS_PER_SEC = 1000.0;
const float SEC_PER_HOUR = 3600.0;
const float AMPS_TO_MILLIAMPS = 1000.0;
const float RPI_VDIV_TOP = 47.0;          // Top of voltage divider (in k ohms)
const float RPI_VDIV_BOTTOM = 150.0;      // Bottom of voltage divider (in k ohms)
const float NANO_VCC = 5.0;
const float ADC_MAX = 1024.0;
const float BATTERY_MAH_MAX = 4000.0;
const float MAH_INTERCEPT = 11.3;
const float MAH_SLOPE = 0.64;             // percentage: 0-1 = 0-100%

void Ina3221Read(int channel, float& v, float& ma)
{
  // Current reading from INA3221 is about 1% too high. Voltage is within 0.5%
  // Current readings are affected by resistance of wiring.
  // MOSFET ON resistance is about 45m ohms
  v = ina3221.getBusVoltage(channel);
  ma = (ina3221.getCurrentAmps(channel) * AMPS_TO_MILLIAMPS - v * MA_PER_V_OFFSET) * CURRENT_ADJUSTMENT;
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
    //Serial.println("Failed to find INA3221 chip");
  }
  else
  {
    //Serial.println("INA3221 Found!");
    for (byte i = 0; i < 3; i++)
    {
      ina3221.setShuntResistance(i, 0.1);
    }
  }
}

void PowerMonitor::Read()
{
  AccCount++;

  float rpiVoltage = analogRead(A1) * (RPI_VDIV_BOTTOM + RPI_VDIV_TOP) / RPI_VDIV_BOTTOM * NANO_VCC / ADC_MAX;

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
    batteryCurrentAvg = batteryCurrentMax = batteryCurrentMin = ma;
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

  battery_mAh_measured -= (batteryCurrentAvg + BATTERY_LEAKAGE_MA) * (sensorReadInterval / MILLIS_PER_SEC) / SEC_PER_HOUR;
  battery_mAh_calculated = (batteryVoltageAvg - MAH_INTERCEPT) * MAH_SLOPE * BATTERY_MAH_MAX;
}

void PowerMonitor::Reset()
{
    AccCount = 0;
}
