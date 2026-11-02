#include <Arduino.h>
#include "Utils.h"

void GetNumber(String s, long& n1)
{
  n1 = s.toInt();
}

void GetNumber(String s, byte& n1)
{
  n1 = (byte)s.toInt();
}

void Get2Numbers(String s, long& n1, long& n2)
{
  n1 = s.toInt();
  n2 = s.substring(s.indexOf(',')+1).toInt();
}
