#include <Arduino.h>
#include "Command.h"
#include "Utils.h"
#include "Control-List.h"
#include "Sensor-List.h"

long loopCount = 0;   //Test code

Task* tasks[]
{
  &ReadSensorTask,
  &ReportSensorTask,
  &BlinkLedTask
};


Command commands[]
{
  Command("sl", UpdateLedDutyCycle),
  Command("ss", UpdateSensorDutyCycle)
};

const int NUM_COMMANDS = sizeof(commands)/sizeof(commands[0]);

void setup()
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  Sensor_Init();
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
