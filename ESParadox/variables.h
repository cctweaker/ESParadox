/////////////////////////////////////////////////
// configuration files
/////////////////////////////////////////////////
const char wifi_cfg_file[] = "wifi.json";
const char dev_cfg_file[] = "device.json";
const char mqtt_cfg_file[] = "mqtt.json";
const char paradox_cfg_file[] = "paradox.json";
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
// flags
/////////////////////////////////////////////////
bool cfg_wifi = false;    // wifi config file read
bool cfg_dev = false;     // device config file read
bool cfg_mqtt = false;    // mqtt config file read
bool cfg_paradox = false; // paradox config file read
/////////////////////////////////////////////////
bool start_mqtt = false;     // start MQTT client
bool start_webserver = true; // start webserver
bool start_paradox = false;  // start paradox monitoring
/////////////////////////////////////////////////
bool use_wifi = false;      // wifi started
bool use_softap = false;    // SoftAP for configuration started
bool use_dns = false;       // DNS server started
bool use_mqtt = false;      // mqtt started
bool use_webserver = false; // webserver started
bool use_paradox = false;   //paradox connected
/////////////////////////////////////////////////
bool do_ota_update = false; // ota update requested
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
// ########     ###    ########     ###    ########   #######  ##     ##
// ##     ##   ## ##   ##     ##   ## ##   ##     ## ##     ##  ##   ##
// ##     ##  ##   ##  ##     ##  ##   ##  ##     ## ##     ##   ## ##
// ########  ##     ## ########  ##     ## ##     ## ##     ##    ###
// ##        ######### ##   ##   ######### ##     ## ##     ##   ## ##
// ##        ##     ## ##    ##  ##     ## ##     ## ##     ##  ##   ##
// ##        ##     ## ##     ## ##     ## ########   #######  ##     ##
/////////////////////////////////////////////////
#define PDX_MSG_LENGTH 37
#define PDX_CHK_LENGTH 36
uint8_t pdx_rx_buffer[PDX_MSG_LENGTH];
uint8_t pdx_tx_buffer[PDX_MSG_LENGTH];
/////////////////////////////////////////////////
#define SourceID 0x05             // NEWARE DIRECT
uint8_t pdx_panel_connection = 0; // panel connection stage
bool pdx_got_data = false;        // have data in RX buffer
bool pdx_got_event = false;       // have event in RX buffer
/////////////////////////////////////////////////
bool partition_rights_access_1 = false;
bool partition_rights_access_2 = false;
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
// PANEL STATUS
/////////////////////////////////////////////////
bool pdx_got_panel_data = false;          // have data to show
bool pdx_panel_refresh = false;            // enable panel data refresh
uint8_t pdx_panel_number_request = 0;     // panel to request
uint8_t pdx_panel_refresh_time = 5;       // time for refresh 5 sec
unsigned long pdx_panel_last_refresh = 0; // panel data last hearbeat
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// USER VARIABLES
/////////////////////////////////////////////////
uint16_t UserID = 0;
uint32_t UserPASS = 0;
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
// GENERAL ALARM STATUS
/////////////////////////////////////////////////
bool SIA = false; // System in alarm, bit 2 of command byte
bool WLC = false; // Winload line connection, bit 1 of command byte
bool WDC = false; // Winload direct connection, bit 0 of command byte
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
// PANEL DATA
/////////////////////////////////////////////////
bool pdx_panel_data_login = false; // send once per login and retain
uint8_t PPI = 0;                   // Panel Product ID
uint8_t PFV = 0;                   // Panel Firmware Version
uint8_t PFR = 0;                   // Panel Firmware Revision
uint8_t PFB = 0;                   // Panel Firmware Build
uint8_t PPID1 = 0;                 // Programmed Panel ID Digit 1&2 [910]
uint8_t PPID2 = 0;                 // Programmed Panel ID Digit 3&4 [910]
uint8_t TFB = 0;                   // Transceiver Firmware Build
uint8_t TF = 0;                    // Transceiver Family
uint8_t TFV = 0;                   // Transceiver Firmware Version
uint8_t TFR = 0;                   // Transceiver Firmware Revision
uint8_t TNFL = 0;                  // Transceiver Noise Floor Level
uint8_t THR = 0;                   // Transceiver Hardware Revision
bool NFLTH = false;                // Noise Floor Level too high
bool CCD = false;                  // Constant Carrier Detected
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
// ##      ## #### ######## ####
// ##  ##  ##  ##  ##        ##
// ##  ##  ##  ##  ##        ##
// ##  ##  ##  ##  ######    ##
// ##  ##  ##  ##  ##        ##
// ##  ##  ##  ##  ##        ##
//  ###  ###  #### ##       ####
/////////////////////////////////////////////////
char SSIDa[32] = "";
char PASSa[65] = "";

char SSIDb[32] = "";
char PASSb[65] = "";
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
//  ######   #######  ##    ## ######## ####  ######         ###    ########
// ##    ## ##     ## ###   ## ##        ##  ##    ##       ## ##   ##     ##
// ##       ##     ## ####  ## ##        ##  ##            ##   ##  ##     ##
// ##       ##     ## ## ## ## ######    ##  ##   ####    ##     ## ########
// ##       ##     ## ##  #### ##        ##  ##    ##     ######### ##
// ##    ## ##     ## ##   ### ##        ##  ##    ##     ##     ## ##
//  ######   #######  ##    ## ##       ####  ######      ##     ## ##
/////////////////////////////////////////////////
const char STA_PASS[] = "abcd1234";
IPAddress STA_IP(192, 168, 10, 1);
IPAddress STA_MASK(255, 255, 255, 0);
const byte DNS_PORT = 53;
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
// ########  ######## ##     ## ####  ######  ########
// ##     ## ##       ##     ##  ##  ##    ## ##
// ##     ## ##       ##     ##  ##  ##       ##
// ##     ## ######   ##     ##  ##  ##       ######
// ##     ## ##        ##   ##   ##  ##       ##
// ##     ## ##         ## ##    ##  ##    ## ##
// ########  ########    ###    ####  ######  ########
/////////////////////////////////////////////////
const char FW_NAME[] = "ESParadox";

char LOC[16] = "home";
char TIP[16] = "alarm";
char NAME[16] = "ESParadox";

char update_url[128] = "esp.3dstar.ro/";

bool heartbeat = true;
uint8_t heartbeat_minutes = 10;
unsigned long last_heartbeat = 0;
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
// ##     ##  #######  ######## ########
// ###   ### ##     ##    ##       ##
// #### #### ##     ##    ##       ##
// ## ### ## ##     ##    ##       ##
// ##     ## ##  ## ##    ##       ##
// ##     ## ##    ##     ##       ##
// ##     ##  ##### ##    ##       ##
/////////////////////////////////////////////////
char MQTT_HOST[64] = "mqtt.3dstar.ro";
int MQTT_PORT = 1883;
char MQTT_USER[16] = "test";
char MQTT_PASS[32] = "12345678";

char WILL[16] = "/will";
char PUB[16] = "";
char SUB[16] = "/cmnd";
char STAT[16] = "/stat";

char mqtt_tx[256] = "";
uint8_t command = 0;
uint8_t subcommand = 0;

bool panel_set_date_time = false;
bool mqtt_SIA_sent = false; // System in alarm, bit 2 of command byte
/////////////////////////////////////////////////

//

/////////////////////////////////////////////////
//  ######   ######  ##
// ##    ## ##    ## ##
// ##       ##       ##
//  ######   ######  ##
//       ##       ## ##
// ##    ## ##    ## ##
//  ######   ######  ########
/////////////////////////////////////////////////
#ifdef USE_SSL
// following cert is for Let's Encrypt CA
// it will authenticate any valid Let's Encrypt issued cert
// for the following few years
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