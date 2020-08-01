# ESParadox
Network interface for Paradox alarm systems. Provides a bidirectional communication between a Paradox alarm system and MQTT.
Reports all events as they occur. If logged in then it provides extra information about the system and can arm/disarm partitions through MQTT.

# Supported mainboards
- SP5500
- SP6000
- SP7000
- MG5000
- MG5050

# Usage
Flash latest release using a 3.3V serial adapter, connect to the created hot-spot (ESParadox_espid) and set it up using the web interface.
Easy to mount inside the Paradox metal box (4 holes for mounting and 1 for the wifi antenna), simple 4 wire connection to the paradox main board.

# Commands
home/alarm/ESParadox/...
- cmnd/arm - full arm
- cmnd/disarm - disarm
- cmnd/stay_arm - stay arm
- cmnd/sleep_arm - sleep arm
- cmnd/stay_arm_d - stay arm with Stay-D
- cmnd/sleep_arm_d - sleep arm with Stay-D
- cmnd/disarm_both - disarm both partitions, disable Stay-D
- cmnd/bypass - send as value zone number to bypass (0-31)
- cmnd/pgm_on_ow - PGM ON in overwrite mode
- cmnd/pgm_off_ow - PGM OFF in overwrite mode
- cmnd/pgm_on - PGM ON
- cmnd/pgm_off - PGM OFF
- cmnd/ - send json

JSON commands:
{"cmd":number, "scmd":number} - send command/subcommand, read manual
{"time":"set", "year":2020, "month":7, "day":31, "hour":15, "minute":33} - set date/time
{"update":any} - perform update
{"reset":any} - perform ESP restart
{"disconnect":any} - log out from Paradox (does a log in immediately)

# Quick tip
subscribe to home/alarm/ESParadox/arm for arm (1) / disarm (0) status
publish to home/alarm/ESParadox/cmnd/AD value (1) for arm / (0) for disarm

# Paradox readouts
home/alarm/ESParadox/....
- panel/info - contains information about the mainboard
- panel/values - AC voltage, DC voltage, Battery voltage, RF Noise floor (for RF capable mainboards)
- event - contains the last reported event, date/time, event group, event subgroup, partition, sensor serial number, sensor label, event description
- trouble - current trouble indicators
- trouble/event - last trouble event decription

- zone/open - array of open zones
- zone/tamper - array of tampered zones
- zone/fire - array of zones in fire alarm
- zone/RF trouble - array of zones in RF trouble
- zone/low battery - array of zones with low battery
- zone/X - last event per zone number (X = 1 to 32)
- zone/X/status - status per zone number
- zone/X/signal - signal strength per zone number
- zone/X/exit delay - of zone in exit delay (0/1)
- zone/X/label - label for zone
- zone/X/sn - serial number of device

- pgm/tamper - array of tampered pgms
- pgm/RF trouble - array of pgms in RF trouble
- pgm/X/signal - signal strength per pgm number (X = 1 to 16)

- module/tamper - array of tampered modules
- module/supervision - array of modules that lost supervision (in trouble)

- repeater/supervision - array of repeaters that lost supervision
- repeater/AC lost - array of repeaters that lost AC power
- repeater/battery failure - array of repeaters that have a bad battery
- repeater/X/signal - signal strength per repeater number (X = 1 to 2)

- keypad/supervision - array of keypads that lost supervision
- keypad/AC lost - array of keypads that lost AC power
- keypad/battery failure - array of keypads that have a bad battery
- keypad/X/signal - signal strength per keypad number (X = 1 to 8)

- partition/X - last event per partition number (X = 1 or 2)
- partition/X/status - current partition status

- arm - shows current arm/disarm status (1 = arm, 0 = disarm)

- bell - last bell event description


# Build
You can build the project yourself using the Arduino IDE. Needed libraries are linked below. Or even better using PlatformIO, libraries are taken care of automatically.

# Libraries
- https://github.com/256dpi/arduino-mqtt
- https://github.com/bblanchon/ArduinoJson

# Hardware
Roll your own using the provided schematics or buy one as PCB, kit or fully assembled from https://3dstar.ro/proiecte/esparadox.

# PCB example
<img src="https://github.com/cctweaker/esparadox/blob/master/Hardware/ESParadox v1.3 top example.jpg?raw=true">
<img src="https://github.com/cctweaker/esparadox/blob/master/Hardware/ESParadox v1.3 bottom example.jpg?raw=true">