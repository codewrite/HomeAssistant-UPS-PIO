#include <Arduino.h>
#include "Command.h"
#include "Utils.h"
#include "PowerMonitor.h"
#include "Control-List.h"
#include "Sensor-List.h"

long loopCount = 0;   //Test code

PowerMonitor pwrMon;

Task* tasks[]
{
  &ReadSensorTask,
  &ReportSensorTask,
  &BatteryControlTask
};

Command commands[]
{
  Command("ss", UpdateSensorDutyCycle),
  Command("sp", SetSupplyPower),
  Command("sb", SetBatteryPower),
  Command("sc", SetBatteryCharging),
  Command("smah", SetBattery_mAh),
  Command("sdnow", ShutdownPi)
};

const int NUM_COMMANDS = sizeof(commands)/sizeof(commands[0]);

void setup()
{
  Serial.begin(115200);
  pwrMon.Init();
  Sensor_Init();
  Control_Init();
  ReportSensors();
}

void loop()
{
  for (Task* t : tasks) t->Run();
  Command::ReadCommand(commands, NUM_COMMANDS);

  // loopCount++;                        //Test code begin
  // if (loopCount % 100000 == 0)
  // {
  //   Serial.print("Loop count = ");
  //   Serial.print(loopCount / 100000);
  //   Serial.print(" hundred thousand");
  //   Serial.println();                 //Test code end
  // }
}
