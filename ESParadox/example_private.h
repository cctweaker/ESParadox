// example file for private data, rename to private.h before building the project

#define LOC "location"          // location of your ESParadox device
#define HOSTNAME "esparadox"    // hostname for esp-07 device and mqtt topic name of your ESParadox device

// the mqtt topic name using above data will be
// "location/esparadox/"


const char MQTT_HOST[] = "mqtt.server.local";       // mqtt server name
const int MQTT_PORT = 8883;                         // secure port
const char MQTT_USER[] = "mqtt_user";               // mqtt username
const char MQTT_PASS[] = "mqtt_password";           // mqtt password