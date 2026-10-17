#include <Arduino.h>
#include "Utils.h"

void GetNumber(String s, int& n1)
{
  n1 = atoi(s.c_str());
}

void Get2Numbers(String s, int& n1, int& n2)
{
  n1 = atoi(s.c_str());
  n2 = atoi(s.c_str()+s.indexOf(",")+1);
}
