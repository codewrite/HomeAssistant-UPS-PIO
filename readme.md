# Home Assistant Uninterruptible Power Supply (HA UPS)

## Classes and Modules

### Command

Commands can be sent via the USB serial port.

### Task

Tasks run periodically. They have a period and can have a duration.

### PowerMonitor

Provides methods and attributes associated with voltages and currents. This includes the supply (15V), battery (12V) and Raspberry Pi (5V).

### Utils

General purpose methods.

### Sensor-List

Methods to report and control the sensor values to Home Assistant.

### Control-List

Methods that can be invoked as commands (via USB serial) from Home Assistant. Currently also contains methods to control charging and power up when HA is powered down. This could be split into a separate file if it becomes too big to be kept here.

### HomeAssistant-UPS

Main entry point.
