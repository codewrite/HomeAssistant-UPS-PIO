#include <Arduino.h>
#include "Utils.h"
#include "Sensor-List.h"
#include "Control-List.h"

int LedBlinkInterval = 3000;
int LedBlinkDuration = 10;
bool PowerOn = true;
bool Charging = false;

bool VccOn = true;
bool BatteryOutOn = true;
bool BatteryInOn = false;

Task BlinkLedTask(LedBlinkInterval, SetLED, LedBlinkDuration, ClearLED);

//***********************
// Task Methods
//***********************

void SetLED(bool on)
{
    digitalWrite(2, on ? HIGH : LOW);
}

void SetLED() { SetLED(true); }
void ClearLED() { SetLED(false); }

//***********************
// Command Methods
//***********************

void UpdateLedDutyCycle(String args)
{
  int n1, n2;
  Get2Numbers(args, n1, n2);
  if (n1 > 0 && n2 > 0 && n1 > n2)
  {
    LedBlinkInterval = n1;
    LedBlinkDuration = n2;
    BlinkLedTask.SetInterval(n1);
    BlinkLedTask.SetDuration(n2);
    ReportSensors();
  }
}
