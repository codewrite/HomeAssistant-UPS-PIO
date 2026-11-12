#ifndef Power_Monitor_h
#define Power_Monitor_h
#include <Arduino.h>

class PowerMonitor
{
  public:
    PowerMonitor();
    void Init();
    void Reset();
    void Read();

    // All voltages in Volts and currents in mA
    float supplyVoltageAvg;
    float supplyVoltageMax;
    float supplyVoltageMin;
    float supplyCurrentAvg;
    float supplyCurrentMax;
    float supplyCurrentMin;
    float batteryVoltageAvg;
    float batteryVoltageMax;
    float batteryVoltageMin;
    float batteryCurrentAvg;
    float batteryCurrentMax;
    float batteryCurrentMin;
    float rpiVoltageAvg;
    float rpiVoltageMax;
    float rpiVoltageMin;
    float battery_mAh_measured;
    float battery_mAh_calculated;
    byte chargingRate = 0;
    bool autoControlDisable = false;
//  private:
    long AccCount = 0;
};
#endif
