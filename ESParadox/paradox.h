#define paradox_message_length 37
#define paradox_checksum_length 36

boolean got_paradox_data = false;
boolean got_data = false;
boolean panel_connected = false;

uint8_t paradox_rx[paradox_message_length];
uint8_t paradox_tx[paradox_message_length];

#define PCpassword1 0xA0
#define PCpassword2 0xA0

#define Mpassword1 0x19
#define Mpassword2 0x69


const boolean DEBUG = false;

#define PDHP 30 * 1000      // panel data heartbeat period
unsigned long pdlh = 0;     // panel data last hearbeat


// PANEL COMMAND
uint8_t command = 0;
uint8_t subcommand = 0;
uint8_t set_time = 0;


// PANEL STATUS
boolean timer_loss;
boolean power_trouble;
boolean ac_failure;
boolean low_battery;
boolean telephone_trouble;

uint8_t ac_voltage;
uint8_t battery_voltage;
uint8_t dc_voltage;

boolean fire;
boolean audible;
boolean silent;
boolean alarm;
boolean stay;
boolean sleep;
boolean arm;
