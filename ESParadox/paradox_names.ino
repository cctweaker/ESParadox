//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void prepare_event_json()
{
    got_event = false;

    StaticJsonDocument<1024> doc;

    uint8_t i = 0;
    uint8_t event = paradox_rx[7];
    uint8_t subgroup = paradox_rx[8];
    bool is_zone = false;
    bool is_partition = false;
    bool is_bell = false;
    bool is_module = false;
    bool is_arm = false;
    bool is_alarm = false;
    bool is_trouble = false;

    mesaj = "";

    get_rx_date_time();
    doc["dt"] = mesaj;
    doc["eg#"] = paradox_rx[7];
    doc["sg#"] = paradox_rx[8];
    doc["prt"] = paradox_rx[9];
    if (paradox_rx[10] || paradox_rx[11] || paradox_rx[12] || paradox_rx[13])
    {
        get_rx_serial_number();
        doc["sn#"] = mesaj;
    }
    get_rx_label();
    doc["label"] = mesaj;

    switch (event)
    {
    case 0:
        mesaj = "Zone OK";
        is_zone = true;
        break;
        ////////////////////////////////////////////

    case 1:
        mesaj = "Zone open";
        is_zone = true;
        break;
        ////////////////////////////////////////////

    case 2:
        is_partition = true;

        switch (subgroup)
        {
        case 2:
            mesaj = "Silent alarm";
            break;
            ///////////
        case 3:
            mesaj = "Buzzer alarm";
            break;
            ///////////
        case 4:
            mesaj = "Steady alarm";
            break;
            ///////////
        case 5:
            mesaj = "Pulsed alarm";
            break;
            ///////////
        case 6:
            mesaj = "Strobe";
            break;
            ///////////
        case 7:
            mesaj = "Alarm stopped";
            break;
            ///////////
        case 8:
            mesaj = "Squawk ON";
            break;
            ///////////
        case 9:
            mesaj = "Squawk OFF";
            break;
            ///////////
        case 10:
            mesaj = "Ground start";
            break;
            ///////////
        case 11:
            mesaj = "Disarm partition";
            break;
            ///////////
        case 12:
            mesaj = "Arm partition";
            break;
            ///////////
        case 13:
            mesaj = "Entry delay started";
            break;
            ///////////
        case 14:
            mesaj = "Exit delay started";
            break;
            ///////////
        case 15:
            mesaj = "Pre-alarm delay";
            break;
            ///////////
        case 16:
            mesaj = "Report confirmation";
            break;
            ///////////
        case 99:
            mesaj = "Any partition status event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 3:
        is_bell = true;

        switch (subgroup)
        {
        case 0:
            mesaj = "Bell OFF";
            break;
            ///////////
        case 1:
            mesaj = "Bell ON";
            break;
            ///////////
        case 2:
            mesaj = "Bell squawk arm";
            break;
            ///////////
        case 3:
            mesaj = "Bell squawk disarm";
            break;
            ///////////
        case 99:
            mesaj = "Any bell status event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 6:

        switch (subgroup)
        {
        case 0:
            mesaj = "Telephone line trouble";
            break;
            ///////////
        case 1:
            mesaj = "ENTER + CLEAR + POWER pressed";
            break;
            ///////////
        case 2:
            mesaj = "N/A";
            break;
            ///////////
        case 3:
            mesaj = "Arm in stay mode";
            break;
            ///////////
        case 4:
            mesaj = "Arm in sleep mode";
            break;
            ///////////
        case 5:
            mesaj = "Arm in force mode";
            break;
            ///////////
        case 6:
            mesaj = "Full arm when armed in stay mode";
            break;
            ///////////
        case 7:
            mesaj = "PC fail to communicate";
            break;
            ///////////
        case 8:
            mesaj = "Utility Key 1 pressed (1+2)";
            break;
            ///////////
        case 9:
            mesaj = "Utility Key 2 pressed (4+5)";
            break;
            ///////////
        case 10:
            mesaj = "Utility Key 3 pressed (7+8)";
            break;
            ///////////
        case 11:
            mesaj = "Utility Key 4 pressed (2+3)";
            break;
            ///////////
        case 12:
            mesaj = "Utility Key 5 pressed (5+6)";
            break;
            ///////////
        case 13:
            mesaj = "Utility Key 6 pressed (8+9)";
            break;
            ///////////
        case 14:
            mesaj = "Tamper generated alarm";
            break;
            ///////////
        case 15:
            mesaj = "Supervision loss generated alarm";
            break;
            ///////////
        case 20:
            mesaj = "Full arm when armed in sleep mode";
            break;
            ///////////
        case 21:
            mesaj = "Firmware upgrade";
            break;
            ///////////
        case 23:
            mesaj = "StayD mode activated";
            break;
            ///////////
        case 24:
            mesaj = "StayD mode deactivated";
            break;
            ///////////
        case 25:
            mesaj = "IP Registration status change";
            break;
            ///////////
        case 26:
            mesaj = "GPRS Registration status change";
            break;
            ///////////
        case 99:
            mesaj = "Any non-reportable event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 8:
        mesaj = "Remote " + String(subgroup) + ", pressed button B";
        break;
        ////////////////////////////////////////////

    case 9:
        mesaj = "Remote " + String(subgroup) + ", pressed button C";
        break;
        ////////////////////////////////////////////

    case 10:
        mesaj = "Remote " + String(subgroup) + ", pressed button D";
        break;
        ////////////////////////////////////////////

    case 11:
        mesaj = "Remote " + String(subgroup) + ", pressed button E";
        break;
        ////////////////////////////////////////////

    case 12:
        is_zone = true;
        mesaj = "Cold start wireless zone";
        break;
        ////////////////////////////////////////////

    case 13:
        is_module = true;
        mesaj = "Cold start wireless module " + String(subgroup);
        break;
        ////////////////////////////////////////////

    case 14:
        mesaj = "Bypass programming user " + String(subgroup);
        break;
        ////////////////////////////////////////////

    case 15:
        mesaj = "User " + String(subgroup) + " code activated output";
        break;
        ////////////////////////////////////////////

    case 16:
        is_zone = true;
        mesaj = "Wireless smoke maintenance signal";
        break;
        ////////////////////////////////////////////

    case 17:
        is_zone = true;
        mesaj = "Delay zone alarm transmission";
        break;
        ////////////////////////////////////////////

    case 18:
        is_zone = true;
        mesaj = "Zone signal strength weak 1";
        break;
        ////////////////////////////////////////////

    case 19:
        is_zone = true;
        mesaj = "Zone signal strength weak 2";
        break;
        ////////////////////////////////////////////

    case 20:
        is_zone = true;
        mesaj = "Zone signal strength weak 3";
        break;
        ////////////////////////////////////////////

    case 21:
        is_zone = true;
        mesaj = "Zone signal strength weak 4";
        break;
        ////////////////////////////////////////////

    case 22:
        mesaj = "Remote " + String(subgroup) + ", button option 5";
        break;
        ////////////////////////////////////////////

    case 23:
        mesaj = "Remote " + String(subgroup) + ", button option 6";
        break;
        ////////////////////////////////////////////

    case 24:
        is_zone = true;
        mesaj = "Fire Delay started";
        break;
        ////////////////////////////////////////////

    case 26:
        switch (subgroup)
        {
            ///////////
        case 1:
            mesaj = "Software Access: WinLoad/Babyware direct";
            break;
            ///////////
        case 2:
            mesaj = "Software Access: WinLoad/Babyware IP module";
            break;
            ///////////
        case 3:
            mesaj = "Software Access: WinLoad/Babyware GSM module";
            break;
            ///////////
        case 4:
            mesaj = "Software Access: WinLoad/Babyware modem";
            break;
            ///////////
        case 9:
            mesaj = "Software Access: IP100 direct";
            break;
            ///////////
        case 10:
            mesaj = "Software Access: VDMP3 direct";
            break;
            ///////////
        case 11:
            mesaj = "Software Access: Voice through GSM module";
            break;
            ///////////
        case 12:
            mesaj = "Software Access: Remote access";
            break;
            ///////////
        case 13:
            mesaj = "Software Access: SMS through GSM module";
            break;
            ///////////
        case 99:
            mesaj = "Software Access: Any software access";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 27:
        switch (subgroup)
        {
            ///////////
        case 0:
            mesaj = "A bus module was added";
            break;
            ///////////
        case 1:
            mesaj = "A bus module was removed";
            break;
            ///////////
        case 2:
            mesaj = "2-way RF Module Communication Failure";
            break;
            ///////////
        case 3:
            mesaj = "2-way RF Module Communication Restored";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 28:
        is_zone = true;
        mesaj = "StayD pass acknowledged";
        break;
        ////////////////////////////////////////////

    case 29:
        is_arm = true;
        mesaj = "Arming with user " + String(subgroup);
        break;
        ////////////////////////////////////////////

    case 30:
        is_arm = true;

        switch (subgroup)
        {
            ///////////
        case 0:
            mesaj = "Auto-arming (on time/no movement)";
            break;
            ///////////
        case 1:
            mesaj = "Late to close";
            break;
            ///////////
        case 2:
            mesaj = "No movement arming";
            break;
            ///////////
        case 3:
            mesaj = "Partial arming";
            break;
        ///////////
        case 4:
            mesaj = "Quick arming";
            break;
        ///////////
        case 5:
            mesaj = "Arming through WinLoad / BabyWare";
            break;
        ///////////
        case 6:
            mesaj = "Arming with keyswitch";
            break;
            ///////////
        case 99:
            mesaj = "Any special arming";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 31:
        is_arm = true;
        mesaj = "Disarming with user " + String(subgroup);
        break;
        ////////////////////////////////////////////

    case 32:
        is_arm = true;
        mesaj = "Disarming after alarm with user " + String(subgroup);
        break;
        ////////////////////////////////////////////

    case 33:
        is_arm = true;
        mesaj = "Alarm cancelled with user " + String(subgroup);
        break;
        ////////////////////////////////////////////

    case 34:
        is_arm = true;

        switch (subgroup)
        {
            ///////////
        case 0:
            mesaj = "Auto-arm cancelled (on time/no movement)";
            break;
            ///////////
        case 1:
            mesaj = "Disarming through WinLoad / BabyWare";
            break;
            ///////////
        case 2:
            mesaj = "Disarming through WinLoad / BabyWare after alarm";
            break;
            ///////////
        case 3:
            mesaj = "Alarm cancelled through WinLoad / BabyWare";
            break;
        ///////////
        case 4:
            mesaj = "Paramedical alarm cancelled";
            break;
        ///////////
        case 5:
            mesaj = "Disarm with keyswitch";
            break;
        ///////////
        case 6:
            mesaj = "Disarm with keyswitch after an alarm";
            break;
        ///////////
        case 7:
            mesaj = "Alarm cancelled with keyswitch";
            break;
            ///////////
        case 99:
            mesaj = "Any special disarming";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 35:
        is_zone = true;
        mesaj = "Zone bypassed";
        break;
        ////////////////////////////////////////////

    case 36:
        is_zone = true;
        mesaj = "Zone in alarm";
        break;
        ////////////////////////////////////////////

    case 37:
        is_zone = true;
        mesaj = "Fire alarm";
        break;
        ////////////////////////////////////////////

    case 38:
        is_zone = true;
        mesaj = "Zone alarm restore";
        break;
        ////////////////////////////////////////////

    case 39:
        is_zone = true;
        mesaj = "Fire alarm restore";
        break;
        ////////////////////////////////////////////

    case 40:
        is_alarm = true;

        switch (subgroup)
        {
            ///////////
        case 0:
            mesaj = "Panic non-medical emergency";
            break;
            ///////////
        case 1:
            mesaj = "Panic medical";
            break;
            ///////////
        case 2:
            mesaj = "Panic fire";
            break;
            ///////////
        case 3:
            mesaj = "Recent closing";
            break;
        ///////////
        case 4:
            mesaj = "Global shutdown";
            break;
        ///////////
        case 5:
            mesaj = "Duress alarm";
            break;
        ///////////
        case 6:
            mesaj = "Keyboard lockout";
            break;
            ///////////
        case 99:
            mesaj = "Any special alarm event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 41:
        is_zone = true;
        mesaj = "Zone shutdown";
        break;
        ////////////////////////////////////////////

    case 42:
        is_zone = true;
        mesaj = "Zone tampered";
        break;
        ////////////////////////////////////////////

    case 43:
        is_zone = true;
        mesaj = "Zone tamper restore";
        break;
        ////////////////////////////////////////////

    case 44:
        is_trouble = true;

        switch (subgroup)
        {
        case 1:
            mesaj = "AC failure";
            break;
            ///////////
        case 2:
            mesaj = "Battery failure";
            break;
            ///////////
        case 3:
            mesaj = "Auxiliary current overload";
            break;
            ///////////
        case 4:
            mesaj = "Bell current overload";
            break;
        ///////////
        case 5:
            mesaj = "Bell disconnected";
            break;
        ///////////
        case 6:
            mesaj = "Clock loss";
            break;
        ///////////
        case 7:
            mesaj = "Fire loop trouble";
            break;
        ///////////
        case 8:
            mesaj = "Fail to communicate to monitoring station telephone #1";
            break;
        ///////////
        case 9:
            mesaj = "Fail to communicate to monitoring station telephone #2";
            break;
        ///////////
        case 11:
            mesaj = "Fail to communicate to voice report";
            break;
        ///////////
        case 12:
            mesaj = "RF jamming";
            break;
        ///////////
        case 13:
            mesaj = "GSM RF jamming";
            break;
        ///////////
        case 14:
            mesaj = "GSM no service";
            break;
        ///////////
        case 15:
            mesaj = "GSM supervision lost";
            break;
        ///////////
        case 16:
            mesaj = "Fail To Communicate IP Receiver 1 (GPRS)";
            break;
        ///////////
        case 17:
            mesaj = "Fail To Communicate IP Receiver 2 (GPRS)";
            break;
        ///////////
        case 18:
            mesaj = "IP Module No Service";
            break;
        ///////////
        case 19:
            mesaj = "IP Module Supervision Loss";
            break;
        ///////////
        case 20:
            mesaj = "Fail To Communicate IP Receiver 1 (IP)";
            break;
        ///////////
        case 21:
            mesaj = "Fail To Communicate IP Receiver 2 (IP)";
            break;
            ///////////
        case 99:
            mesaj = "Any new trouble event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 45:
        is_trouble = true;

        switch (subgroup)
        {
        case 0:
            mesaj = "Telephone line restored";
            break;
            ///////////
        case 1:
            mesaj = "AC failure restore";
            break;
            ///////////
        case 2:
            mesaj = "Battery failure restore";
            break;
            ///////////
        case 3:
            mesaj = "Auxiliary current overload restore";
            break;
            ///////////
        case 4:
            mesaj = "Bell current overload restore";
            break;
        ///////////
        case 5:
            mesaj = "Bell disconnected restore";
            break;
        ///////////
        case 6:
            mesaj = "Clock loss restore";
            break;
        ///////////
        case 7:
            mesaj = "Fire loop trouble restore";
            break;
        ///////////
        case 8:
            mesaj = "Fail to communicate to monitoring station telephone #1 restore";
            break;
        ///////////
        case 9:
            mesaj = "Fail to communicate to monitoring station telephone #2 restore";
            break;
        ///////////
        case 11:
            mesaj = "Fail to communicate to voice report restore";
            break;
        ///////////
        case 12:
            mesaj = "RF jamming restore";
            break;
        ///////////
        case 13:
            mesaj = "GSM RF jamming restore";
            break;
        ///////////
        case 14:
            mesaj = "GSM no service restore";
            break;
        ///////////
        case 15:
            mesaj = "GSM supervision lost restore";
            break;
        ///////////
        case 16:
            mesaj = "Fail To Communicate restore IP Receiver 1 (GPRS)";
            break;
        ///////////
        case 17:
            mesaj = "Fail To Communicate restore IP Receiver 2 (GPRS)";
            break;
        ///////////
        case 18:
            mesaj = "IP Module No Service restore";
            break;
        ///////////
        case 19:
            mesaj = "IP Module Supervision Loss restore";
            break;
        ///////////
        case 20:
            mesaj = "Fail To Communicate restore IP Receiver 1 (IP)";
            break;
        ///////////
        case 21:
            mesaj = "Fail To Communicate restore IP Receiver 2 (IP)";
            break;
            ///////////
        case 99:
            mesaj = "Any new trouble restored event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 46:
        is_trouble = true;

        switch (subgroup)
        {
            ///////////
        case 0:
            mesaj = "Bus module communication fault";
            break;
            ///////////
        case 1:
            mesaj = "Bus module tamper trouble";
            break;
            ///////////
        case 2:
            mesaj = "Bus module power fail";
            break;
            ///////////
        case 3:
            mesaj = "Bus module battery failure";
            break;
            ///////////
        case 99:
            mesaj = "Any bus module trouble event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 47:
        is_trouble = true;

        switch (subgroup)
        {
            ///////////
        case 0:
            mesaj = "Bus module communication fault restore";
            break;
            ///////////
        case 1:
            mesaj = "Bus module tamper trouble restore";
            break;
            ///////////
        case 2:
            mesaj = "Bus module power fail restore";
            break;
            ///////////
        case 3:
            mesaj = "Bus module battery failure restore";
            break;
            ///////////
        case 99:
            mesaj = "Any bus module trouble restored event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 48:
        switch (subgroup)
        {
            ///////////
        case 0:
            mesaj = "System power up";
            break;
            ///////////
        case 1:
            mesaj = "Reporting test";
            break;
            ///////////
        case 2:
            mesaj = "Software log on";
            break;
            ///////////
        case 3:
            mesaj = "Software log off";
            break;
            ///////////
        case 4:
            mesaj = "Installer in programming mode";
            break;
            ///////////
        case 5:
            mesaj = "Installer exited programming mode";
            break;
            ///////////
        case 6:
            mesaj = "Maintenance in programming mode";
            break;
            ///////////
        case 7:
            mesaj = "Maintenance exited programming mode";
            break;
            ///////////
        case 8:
            mesaj = "Closing delinquency delay elapsed";
            break;
            ///////////
        case 99:
            mesaj = "Any special event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 49:
        is_zone = true;
        mesaj = "Low battery on zone";
        break;
        ////////////////////////////////////////////

    case 50:
        is_zone = true;
        mesaj = "Low battery on zone restore";
        break;
        ////////////////////////////////////////////

    case 51:
        is_zone = true;
        mesaj = "Zone supervision trouble";
        break;
        ////////////////////////////////////////////

    case 52:
        is_zone = true;
        mesaj = "Zone supervision restore";
        break;
        ////////////////////////////////////////////

    case 53:
        is_module = true;
        mesaj = "Wireless module " + String(subgroup) + " supervision trouble";
        break;
        ////////////////////////////////////////////

    case 54:
        is_module = true;
        mesaj = "Wireless module " + String(subgroup) + " supervision restore";
        break;
        ////////////////////////////////////////////

    case 55:
        is_module = true;
        mesaj = "Wireless module " + String(subgroup) + " tamper trouble";
        break;
        ////////////////////////////////////////////

    case 56:
        is_module = true;
        mesaj = "Wireless module " + String(subgroup) + " tamper restore";
        break;
        ////////////////////////////////////////////

    case 57:
        is_alarm = true;
        mesaj = "Non-medical alarm (paramedic) user " + String(subgroup);
        break;
        ////////////////////////////////////////////

    case 58:
        is_zone = true;
        mesaj = "Zone forced";
        break;
        ////////////////////////////////////////////

    case 59:
        is_zone = true;
        mesaj = "Zone included";
        break;
        ////////////////////////////////////////////

    case 64:
        mesaj = "System Status " + String(subgroup);
        break;

    default:
        mesaj = "Undocumented";
    }

    doc["event"] = mesaj;

    if (is_zone)
    {
        client.publish(MQTT_ZONE_TOPIC + String(subgroup), mesaj, true, 0);
        client.publish(MQTT_ZONE_TOPIC + String(subgroup) + "/label", doc["label"], true, 0);
        client.publish(MQTT_ZONE_TOPIC + String(subgroup) + "/sn", doc["sn#"], true, 0);
    }

    if (is_partition)
    {
        client.publish(MQTT_PARTITION_TOPIC + String(paradox_rx[9]), mesaj, true, 0);
    }

    if (is_bell)
    {
        client.publish(MQTT_BELL_TOPIC, mesaj, true, 0);
    }

    if (is_module)
    {
        client.publish(MQTT_BUS_TOPIC + String(subgroup), mesaj, true, 0);
    }

    if (is_arm)
    {
        client.publish(MQTT_ARM_TOPIC, mesaj, true, 0);
    }

    if (is_alarm)
    {
        client.publish(MQTT_ALARM_TOPIC, mesaj, true, 0);
    }

    if (is_trouble)
    {
        client.publish(MQTT_TROUBLE_TOPIC, mesaj, true, 0);
    }

    got_mqtt_data = true;
    mesaj = "";
    serializeJson(doc, mesaj);
    doc.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void get_rx_date_time()
{
    mesaj = String(paradox_rx[1]);
    mesaj += String(paradox_rx[2]);
    mesaj += "-";
    if (paradox_rx[3] < 10)
        mesaj += "0";
    mesaj += String(paradox_rx[3]);
    mesaj += "-";
    if (paradox_rx[4] < 10)
        mesaj += "0";
    mesaj += String(paradox_rx[4]);
    mesaj += " ";
    if (paradox_rx[5] < 10)
        mesaj += "0";
    mesaj += String(paradox_rx[5]);
    mesaj += ":";
    if (paradox_rx[6] < 10)
        mesaj += "0";
    mesaj += String(paradox_rx[6]);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void get_rx_serial_number()
{
    mesaj = "";

    for (uint8_t i = 10; i <= 13; i++)
    {
        if (paradox_rx[i] < 0x10)
            mesaj += "0";
        mesaj += String(paradox_rx[i], HEX);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void get_rx_label()
{
    mesaj = "";

    for (uint8_t i = 15; i <= 30; i++)
    {
        if (paradox_rx[i] > 0)
        {
            mesaj += char(paradox_rx[i]);
        }
        else
        {
            mesaj += " ";
        }
    }

    mesaj.trim();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void panel_1_partition_status_names(uint8_t byte, uint8_t bit)
{
    switch (byte)
    {
    case 0:
        switch (bit)
        {
        case 7:
            mesaj = "Pulse Alarm (fire alarm).";
            break;
        case 6:
            mesaj = "Audible alarm.";
            break;
        case 5:
            mesaj = "Silent alarm.";
            break;
        case 4:
            mesaj = "Is / was in Alarm";
            break;
        case 2:
            mesaj = "Stay Arm";
            break;
        case 1:
            mesaj = "Sleep Arm";
            break;
        case 0:
            mesaj = "Arm (Stay/Sleep/Full)";
            break;
        }
        break;
    case 1:
        switch (bit)
        {
        case 7:
            mesaj = "Bell Activated";
            break;
        case 6:
            mesaj = "Auto-Arming Engaged";
            break;
        case 5:
            mesaj = "Recent Closing Delay";
            break;
        case 4:
            mesaj = "Intellizone Delay";
            break;
        case 3:
            mesaj = "At least one zone is bypassed";
            break;
        case 2:
            mesaj = "Alarm(s) is / are in the memory";
            break;
        case 1:
            mesaj = "Entry Delay";
            break;
        case 0:
            mesaj = "Exit Delay";
            break;
        }
        break;
    case 2:
        switch (bit)
        {
        case 7:
            mesaj = "Paramedic Alarm";
            break;
        case 6:
            mesaj = "Reserved";
            break;
        case 5:
            mesaj = "Arm with a Remote";
            break;
        case 4:
            mesaj = "Transmission Delay Finished";
            break;
        case 3:
            mesaj = "Bell Delay Finished";
            break;
        case 2:
            mesaj = "Entry Delay Finished";
            break;
        case 1:
            mesaj = "Exit Delay Finished";
            break;
        case 0:
            mesaj = "Intellizone Delay Finished";
            break;
        }
        break;
    case 3:
        switch (bit)
        {
        case 6:
            mesaj = "Wait windoe (window mode).";
            break;
        case 4:
            mesaj = "In remote delay.";
            break;
        case 2:
            mesaj = "StayD mode active.";
            break;
        case 1:
            mesaj = "Force Arm";
            break;
        case 0:
            mesaj = "Ready Status";
            break;
        }
        break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
