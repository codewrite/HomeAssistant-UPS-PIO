#include <Arduino.h>
#include "Utils.h"

void GetNumber(String s, long& n1)
{
  n1 = atol(s.c_str());
}

void Get2Numbers(String s, long& n1, long& n2)
{
  n1 = atol(s.c_str());
  n2 = atol(s.c_str()+s.indexOf(",")+1);
}
