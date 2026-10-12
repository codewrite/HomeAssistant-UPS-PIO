#include <Arduino.h>
#include "Command.h"

String _commandAccumulator;

Command::Command(char* commandName, commandFunctionPtr commandFn)
{
  _commandName = commandName;
  _commandFn = commandFn;
}

void Command::ReadCommand(Command commands[], int cmdCount)
{
  while (Serial.available() > 0)
  {
    _commandAccumulator += Serial.readString();
  }
  int ix = _commandAccumulator.indexOf(';');
  if (ix >= 0)
  {
    String cmd = _commandAccumulator.substring(0, ix);
    cmd.replace("\r", "");
    cmd.replace("\n", "");
    int cmdLen = cmd.length();
    bool cmdFound = false;
    if (cmdLen > 0)
    {
      for (int i=0; i<cmdCount; i++)
      {
        Command c = commands[i];
        if (cmd.indexOf(c._commandName) == 0)
        {
          cmd.remove(0, c._commandName.length());
          c._commandFn(cmd);
          cmdFound = true;
          break;
        }
      }
      if (!cmdFound)
      {
        Serial.print("Unrecognised command: ");
        Serial.println(cmd);
      }
    }
    _commandAccumulator.remove(0, ix+1);
  }
}
