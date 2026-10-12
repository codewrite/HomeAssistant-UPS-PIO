#ifndef Sensor_List_h
#define Sensor_List_h
#include <Arduino.h>
#include "Task.h"

extern Task ReadSensorTask;
extern Task ReportSensorTask;

void Sensor_Init();

//***********************
// Task Methods
//***********************

void ReadSensors();
void ReportSensors();

//***********************
// Command Methods
//***********************

void UpdateSensorDutyCycle(String args);

#endif
