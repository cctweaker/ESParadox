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