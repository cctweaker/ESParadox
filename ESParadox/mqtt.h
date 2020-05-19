///////////////////////////////////////////////////////////////////////
#define CHECK_CA_ROOT
#define SECRET
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// topic to publish ESP status (1 = ON, 0 = OFF)
const char MQTT_WILL_TOPIC[] = LOC "/" HOSTNAME "/status";
// topic to listen for commands
const char MQTT_COMMAND_TOPIC[] = LOC "/" HOSTNAME "/cmnd";
///////////////////////////////////////////////////////////////////////
// const char MQTT_PUB_TOPIC[] = LOC "/" HOSTNAME "/out";
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// just for debugging, will be eliminated
///////////////////////////////////////////////////////////////////////
const char MQTT_HB_TOPIC[] = LOC "/" HOSTNAME "/hb";
#define heartbeat_period 10 * 1000 // 10 sec
unsigned long last_heartbeat = 0;
const char MQTT_DEBUG_TOPIC[] = LOC "/" HOSTNAME "/debug";
const char MQTT_RAW_TOPIC[] = LOC "/" HOSTNAME "/raw";
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
const char MQTT_PANEL_TOPIC[] = LOC "/" HOSTNAME "/panel";
const char MQTT_PANEL_HW_INFO_TOPIC[] = LOC "/" HOSTNAME "/panel/info";
const char MQTT_PANEL_TROUBLE_TOPIC[] = LOC "/" HOSTNAME "/panel/trouble";
const char MQTT_PANEL_OPEN_TOPIC[] = LOC "/" HOSTNAME "/panel/open";
const char MQTT_PANEL_TAMPER_TOPIC[] = LOC "/" HOSTNAME "/panel/tamper";
const char MQTT_PANEL_FIRE_TOPIC[] = LOC "/" HOSTNAME "/panel/fire";
const char MQTT_PANEL_LEVELS_TOPIC[] = LOC "/" HOSTNAME "/panel/levels";
const char MQTT_PANEL_PARTITION_TOPIC[] = LOC "/" HOSTNAME "/panel/partition/";
const char MQTT_PANEL_RF_TROUBLE_TOPIC[] = LOC "/" HOSTNAME "/panel/rf_trouble";
const char MQTT_PANEL_RF_LOW_BATTERY_TOPIC[] = LOC "/" HOSTNAME "/panel/rf_low_battery";
const char MQTT_PANEL_AC_LOST_TOPIC[] = LOC "/" HOSTNAME "/panel/ac_lost";
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// ZONE TOPIC and open/close event message
///////////////////////////////////////////////////////////////////////
// zone number will be appeded
const char MQTT_ZONE_TOPIC[] = LOC "/" HOSTNAME "/zone/";
// subtopics: status, battery, rf level are autogenerated
///////////////////////////////////////////////////////////////////////
// define the message you want to receive in your zones topic
const char ZONE_OPEN[] = "1";
const char ZONE_OK[] = "0";
// const char ZONE_OPEN[] = "open";
// const char ZONE_OK[] = "ok";
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// like zones, PGM, Bus, repeaters and keyboards have their own topics
///////////////////////////////////////////////////////////////////////
const char MQTT_PGM_TOPIC[] = LOC "/" HOSTNAME "/pgm/";
const char MQTT_BUS_TOPIC[] = LOC "/" HOSTNAME "/module/";
const char MQTT_WREP_TOPIC[] = LOC "/" HOSTNAME "/repeater/";
const char MQTT_WKEY_TOPIC[] = LOC "/" HOSTNAME "/keypad/";
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// EVENT TOPIC where all events are reported instantly
// they are also reported on their own dedicated topics
///////////////////////////////////////////////////////////////////////
const char MQTT_EVENT_TOPIC[] = LOC "/" HOSTNAME "/event";
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
const char MQTT_PARTITION_TOPIC[] = LOC "/" HOSTNAME "/partition/";
const char MQTT_BELL_TOPIC[] = LOC "/" HOSTNAME "/bell";
const char MQTT_ALARM_TOPIC[] = LOC "/" HOSTNAME "/alarm";
const char MQTT_ARM_TOPIC[] = LOC "/" HOSTNAME "/arm";
const char MQTT_TROUBLE_TOPIC[] = LOC "/" HOSTNAME "/trouble";
///////////////////////////////////////////////////////////////////////

String topic;
String mesaj;

///////////////////////////////////////////////////////////////////////
#ifdef CHECK_CA_ROOT
// following cert is for Let's Encrypt CA
// it will authenticate any valid Let's Encrypt issued cert
static const char digicert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/
MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT
DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow
SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT
GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC
AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF
q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8
SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0
Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA
a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj
/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T
AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG
CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv
bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k
c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw
VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC
ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz
MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu
Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF
AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo
uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/
wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu
X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG
PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6
KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==
-----END CERTIFICATE-----
)EOF";
#endif
