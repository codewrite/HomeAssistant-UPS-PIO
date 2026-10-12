#ifndef Command_h
#define Command_h
#include <Arduino.h>

typedef void(*commandFunctionPtr)(String args);

class Command
{
  public:
    Command(const char* commandName, commandFunctionPtr commandFn);
    static void ReadCommand(Command commands[], int cmdCount);
  private:
    String _commandName;
    commandFunctionPtr _commandFn;
};
#endif
