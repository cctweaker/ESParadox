// example file for private data, rename to private.h before building the project

#define LOC "location"          // location of your ESParadox device
#define HOSTNAME "esparadox"    // hostname for esp-07 device and mqtt topic name of your ESParadox device

// the mqtt topic name using above data will be
// "location/esparadox/"


const char MQTT_HOST[] = "mqtt.server.local";       // mqtt server name
const int MQTT_PORT = 8883;                         // secure port
const char MQTT_USER[] = "mqtt_user";               // mqtt username
const char MQTT_PASS[] = "mqtt_password";           // mqtt password


// your main SSID & PASS
const char ssid[] = "SSID";
const char pass[] = "password";

// backup SSID & PASS
// set it up on your phone to allow initial setup or changes in the future
// or add your second SSID if you have it
const char ssidb[] = "AndroidAP";
const char passb[] = "abcdefgh";
// for safety, if main network fails ESParadox will try to connect to backup SSID
// if backup SSID is not present then the unit will restart

// WIFIManager setups are nicer but if your AC power goes down and the router with it
// the ESP can boot in station mode and allow hijacking