//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void prepare_event_json()
{
    got_event = false;
    got_mqtt_data = true;

    StaticJsonDocument<1024> doc;

    uint8_t i = 0;
    uint8_t event = paradox_rx[7];
    uint8_t subgroup = paradox_rx[8];
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
        doc["event"] = "Zone OK";
        // send closed status
        client.publish(MQTT_ZONE_TOPIC + String(subgroup), "ok", true, 0);
        break;
        ////////////////////////////////////////////

    case 1:
        doc["event"] = "Zone open";
        // send open status
        client.publish(MQTT_ZONE_TOPIC + String(subgroup), "open", true, 0);
        // save zone name
        client.publish(MQTT_ZONE_TOPIC + String(subgroup) + "/label", mesaj, true, 0);
        break;
        ////////////////////////////////////////////

    case 2:
        topic = MQTT_EVENT_TOPIC;

        switch (subgroup)
        {
        case 2:
            doc["event"] = "Silent alarm";
            break;
            ///////////
        case 3:
            doc["event"] = "Buzzer alarm";
            break;
            ///////////
        case 4:
            doc["event"] = "Steady alarm";
            break;
            ///////////
        case 5:
            doc["event"] = "Pulsed alarm";
            break;
            ///////////
        case 6:
            doc["event"] = "Strobe";
            break;
            ///////////
        case 7:
            doc["event"] = "Alarm stopped";
            break;
            ///////////
        case 8:
            doc["event"] = "Squawk ON";
            break;
            ///////////
        case 9:
            doc["event"] = "Squawk OFF";
            break;
            ///////////
        case 10:
            doc["event"] = "Ground start";
            break;
            ///////////
        case 11:
            doc["event"] = "Disarm partition";
            break;
            ///////////
        case 12:
            doc["event"] = "Arm partition";
            break;
            ///////////
        case 13:
            doc["event"] = "Entry delay started";
            break;
            ///////////
        case 14:
            doc["event"] = "Exit delay started";
            break;
            ///////////
        case 15:
            doc["event"] = "Pre-alarm delay";
            break;
            ///////////
        case 16:
            doc["event"] = "Report confirmation";
            break;
            ///////////
        case 99:
            doc["event"] = "Any partition status event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 3:
        topic = MQTT_EVENT_TOPIC;

        switch (subgroup)
        {
        case 0:
            doc["event"] = "Bell OFF";
            break;
            ///////////
        case 1:
            doc["event"] = "Bell ON";
            break;
            ///////////
        case 2:
            doc["event"] = "Bell squawk arm";
            break;
            ///////////
        case 3:
            doc["event"] = "Bell squawk disarm";
            break;
            ///////////
        case 99:
            doc["event"] = "Any bell status event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 6:
        topic = MQTT_EVENT_TOPIC;

        switch (subgroup)
        {
        case 0:
            doc["event"] = "Telephone line trouble";
            break;
            ///////////
        case 1:
            doc["event"] = "ENTER + CLEAR + POWER pressed";
            break;
            ///////////
        case 2:
            doc["event"] = "N/A";
            break;
            ///////////
        case 3:
            doc["event"] = "Arm in stay mode";
            break;
            ///////////
        case 4:
            doc["event"] = "Arm in sleep mode";
            break;
            ///////////
        case 5:
            doc["event"] = "Arm in force mode";
            break;
            ///////////
        case 6:
            doc["event"] = "Full arm when armed in stay mode";
            break;
            ///////////
        case 7:
            doc["event"] = "PC fail to communicate";
            break;
            ///////////
        case 8:
            doc["event"] = "Utility Key 1 pressed (1+2)";
            break;
            ///////////
        case 9:
            doc["event"] = "Utility Key 2 pressed (4+5)";
            break;
            ///////////
        case 10:
            doc["event"] = "Utility Key 3 pressed (7+8)";
            break;
            ///////////
        case 11:
            doc["event"] = "Utility Key 4 pressed (2+3)";
            break;
            ///////////
        case 12:
            doc["event"] = "Utility Key 5 pressed (5+6)";
            break;
            ///////////
        case 13:
            doc["event"] = "Utility Key 6 pressed (8+9)";
            break;
            ///////////
        case 14:
            doc["event"] = "Tamper generated alarm";
            break;
            ///////////
        case 15:
            doc["event"] = "Supervision loss generated alarm";
            break;
            ///////////
        case 20:
            doc["event"] = "Full arm when armed in sleep mode";
            break;
            ///////////
        case 21:
            doc["event"] = "Firmware upgrade";
            break;
            ///////////
        case 23:
            doc["event"] = "StayD mode activated";
            break;
            ///////////
        case 24:
            doc["event"] = "StayD mode deactivated";
            break;
            ///////////
        case 25:
            doc["event"] = "IP Registration status change";
            break;
            ///////////
        case 26:
            doc["event"] = "GPRS Registration status change";
            break;
            ///////////
        case 99:
            doc["event"] = "Any non-reportable event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 8:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Remote " + String(subgroup) + "button B";
        break;
        ////////////////////////////////////////////

    case 9:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Remote " + String(subgroup) + "button C";
        break;
        ////////////////////////////////////////////

    case 10:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Remote " + String(subgroup) + "button D";
        break;
        ////////////////////////////////////////////

    case 11:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Remote " + String(subgroup) + "button E";
        break;
        ////////////////////////////////////////////

    case 12:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Cold start wireless zone";
        break;
        ////////////////////////////////////////////

    case 13:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Cold start wireless module " + String(subgroup);
        break;
        ////////////////////////////////////////////

    case 14:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Bypass programming user " + String(subgroup);
        break;
        ////////////////////////////////////////////

    case 15:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "User " + String(subgroup) + " code activated output";
        break;
        ////////////////////////////////////////////

    case 16:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Wireless smoke maintenance signal";
        break;
        ////////////////////////////////////////////

    case 17:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Delay zone alarm transmission";
        break;
        ////////////////////////////////////////////

    case 18:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Zone sognal strength weak 1";
        break;
        ////////////////////////////////////////////

    case 19:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Zone sognal strength weak 2";
        break;
        ////////////////////////////////////////////

    case 20:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Zone sognal strength weak 3";
        break;
        ////////////////////////////////////////////

    case 21:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Zone sognal strength weak 4";
        break;
        ////////////////////////////////////////////

    case 22:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Remote " + String(subgroup) + "button option 5";
        break;
        ////////////////////////////////////////////

    case 23:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Remote " + String(subgroup) + "button option 6";
        break;
        ////////////////////////////////////////////

    case 24:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Fire Delay started";
        break;
        ////////////////////////////////////////////

    case 26:
        topic = MQTT_EVENT_TOPIC;

        switch (subgroup)
        {
            ///////////
        case 1:
            doc["event"] = "Software Access: WinLoad/Babyware direct";
            break;
            ///////////
        case 2:
            doc["event"] = "Software Access: WinLoad/Babyware IP module";
            break;
            ///////////
        case 3:
            doc["event"] = "Software Access: WinLoad/Babyware GSM module";
            break;
            ///////////
        case 4:
            doc["event"] = "Software Access: WinLoad/Babyware modem";
            break;
            ///////////
        case 9:
            doc["event"] = "Software Access: IP100 direct";
            break;
            ///////////
        case 10:
            doc["event"] = "Software Access: VDMP3 direct";
            break;
            ///////////
        case 11:
            doc["event"] = "Software Access: Voice through GSM module";
            break;
            ///////////
        case 12:
            doc["event"] = "Software Access: Remote access";
            break;
            ///////////
        case 13:
            doc["event"] = "Software Access: SMS through GSM module";
            break;
            ///////////
        case 99:
            doc["event"] = "Software Access: Any software access";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 27:
        topic = MQTT_EVENT_TOPIC;

        switch (subgroup)
        {
            ///////////
        case 0:
            doc["event"] = "A bus module was added";
            break;
            ///////////
        case 1:
            doc["event"] = "A bus module was removed";
            break;
            ///////////
        case 2:
            doc["event"] = "2-way RF Module Communication Failure";
            break;
            ///////////
        case 3:
            doc["event"] = "2-way RF Module Communication Restored";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 28:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "StayD pass acknowledged";
        break;
        ////////////////////////////////////////////

    case 29:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Arming with user " + String(subgroup);
        break;
        ////////////////////////////////////////////

    case 30:
        topic = MQTT_EVENT_TOPIC;

        switch (subgroup)
        {
            ///////////
        case 0:
            doc["event"] = "Auto-arming (on time/no movement)";
            break;
            ///////////
        case 1:
            doc["event"] = "Late to close";
            break;
            ///////////
        case 2:
            doc["event"] = "No movement arming";
            break;
            ///////////
        case 3:
            doc["event"] = "Partial arming";
            break;
        ///////////
        case 4:
            doc["event"] = "Quick arming";
            break;
        ///////////
        case 5:
            doc["event"] = "Arming through WinLoad / BabyWare";
            break;
        ///////////
        case 6:
            doc["event"] = "Arming with keyswitch";
            break;
            ///////////
        case 99:
            doc["event"] = "Any special arming";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 31:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Disarming with user " + String(subgroup);
        break;
        ////////////////////////////////////////////

    case 32:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Disarming after alarm with user " + String(subgroup);
        break;
        ////////////////////////////////////////////

    case 33:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Alarm cancelled with user " + String(subgroup);
        break;
        ////////////////////////////////////////////

    case 34:
        topic = MQTT_EVENT_TOPIC;

        switch (subgroup)
        {
            ///////////
        case 0:
            doc["event"] = "Auto-arm cancelled (on time/no movement)";
            break;
            ///////////
        case 1:
            doc["event"] = "Disarming through WinLoad / BabyWare";
            break;
            ///////////
        case 2:
            doc["event"] = "Disarming through WinLoad / BabyWare after alarm";
            break;
            ///////////
        case 3:
            doc["event"] = "Alarm cancelled through WinLoad / BabyWare";
            break;
        ///////////
        case 4:
            doc["event"] = "Paramedical alarm cancelled";
            break;
        ///////////
        case 5:
            doc["event"] = "Disarm with keyswitch";
            break;
        ///////////
        case 6:
            doc["event"] = "Disarm with keyswitch after an alarm";
            break;
        ///////////
        case 7:
            doc["event"] = "Alarm cancelled with keyswitch";
            break;
            ///////////
        case 99:
            doc["event"] = "Any special disarming";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 35:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Zone bypassed";
        break;
        ////////////////////////////////////////////

    case 36:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Zone in alarm";
        break;
        ////////////////////////////////////////////

    case 37:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Fire alarm";
        break;
        ////////////////////////////////////////////

    case 38:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Zone alarm restore";
        break;
        ////////////////////////////////////////////

    case 39:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Fire alarm restore";
        break;
        ////////////////////////////////////////////

    case 40:
        topic = MQTT_EVENT_TOPIC;

        switch (subgroup)
        {
            ///////////
        case 0:
            doc["event"] = "Panic non-medical emergency";
            break;
            ///////////
        case 1:
            doc["event"] = "Panic medical";
            break;
            ///////////
        case 2:
            doc["event"] = "Panic fire";
            break;
            ///////////
        case 3:
            doc["event"] = "Recent closing";
            break;
        ///////////
        case 4:
            doc["event"] = "Global shutdown";
            break;
        ///////////
        case 5:
            doc["event"] = "Duress alarm";
            break;
        ///////////
        case 6:
            doc["event"] = "Keyboard lockout";
            break;
            ///////////
        case 99:
            doc["event"] = "Any special alarm event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 41:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Zone shutdown";
        break;
        ////////////////////////////////////////////

    case 42:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Zone tampered";
        break;
        ////////////////////////////////////////////

    case 43:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Zone tamper restore";
        break;
        ////////////////////////////////////////////

    case 44:
        topic = MQTT_TROUBLE_TOPIC;

        switch (subgroup)
        {
        case 1:
            doc["event"] = "AC failure";
            break;
            ///////////
        case 2:
            doc["event"] = "Battery failure";
            break;
            ///////////
        case 3:
            doc["event"] = "Auxiliary current overload";
            break;
            ///////////
        case 4:
            doc["event"] = "Bell current overload";
            break;
        ///////////
        case 5:
            doc["event"] = "Bell disconnected";
            break;
        ///////////
        case 6:
            doc["event"] = "Clock loss";
            break;
        ///////////
        case 7:
            doc["event"] = "Fire loop trouble";
            break;
        ///////////
        case 8:
            doc["event"] = "Fail to communicate to monitoring station telephone #1";
            break;
        ///////////
        case 9:
            doc["event"] = "Fail to communicate to monitoring station telephone #2";
            break;
        ///////////
        case 11:
            doc["event"] = "Fail to communicate to voice report";
            break;
        ///////////
        case 12:
            doc["event"] = "RF jamming";
            break;
        ///////////
        case 13:
            doc["event"] = "GSM RF jamming";
            break;
        ///////////
        case 14:
            doc["event"] = "GSM no service";
            break;
        ///////////
        case 15:
            doc["event"] = "GSM supervision lost";
            break;
        ///////////
        case 16:
            doc["event"] = "Fail To Communicate IP Receiver 1 (GPRS)";
            break;
        ///////////
        case 17:
            doc["event"] = "Fail To Communicate IP Receiver 2 (GPRS)";
            break;
        ///////////
        case 18:
            doc["event"] = "IP Module No Service";
            break;
        ///////////
        case 19:
            doc["event"] = "IP Module Supervision Loss";
            break;
        ///////////
        case 20:
            doc["event"] = "Fail To Communicate IP Receiver 1 (IP)";
            break;
        ///////////
        case 21:
            doc["event"] = "Fail To Communicate IP Receiver 2 (IP)";
            break;
            ///////////
        case 99:
            doc["event"] = "Any new trouble event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 45:
        topic = MQTT_TROUBLE_TOPIC;

        switch (subgroup)
        {
        case 0:
            doc["event"] = "Telephone line restored";
            break;
            ///////////
        case 1:
            doc["event"] = "AC failure restore";
            break;
            ///////////
        case 2:
            doc["event"] = "Battery failure restore";
            break;
            ///////////
        case 3:
            doc["event"] = "Auxiliary current overload restore";
            break;
            ///////////
        case 4:
            doc["event"] = "Bell current overload restore";
            break;
        ///////////
        case 5:
            doc["event"] = "Bell disconnected restore";
            break;
        ///////////
        case 6:
            doc["event"] = "Clock loss restore";
            break;
        ///////////
        case 7:
            doc["event"] = "Fire loop trouble restore";
            break;
        ///////////
        case 8:
            doc["event"] = "Fail to communicate to monitoring station telephone #1 restore";
            break;
        ///////////
        case 9:
            doc["event"] = "Fail to communicate to monitoring station telephone #2 restore";
            break;
        ///////////
        case 11:
            doc["event"] = "Fail to communicate to voice report restore";
            break;
        ///////////
        case 12:
            doc["event"] = "RF jamming restore";
            break;
        ///////////
        case 13:
            doc["event"] = "GSM RF jamming restore";
            break;
        ///////////
        case 14:
            doc["event"] = "GSM no service restore";
            break;
        ///////////
        case 15:
            doc["event"] = "GSM supervision lost restore";
            break;
        ///////////
        case 16:
            doc["event"] = "Fail To Communicate restore IP Receiver 1 (GPRS)";
            break;
        ///////////
        case 17:
            doc["event"] = "Fail To Communicate restore IP Receiver 2 (GPRS)";
            break;
        ///////////
        case 18:
            doc["event"] = "IP Module No Service restore";
            break;
        ///////////
        case 19:
            doc["event"] = "IP Module Supervision Loss restore";
            break;
        ///////////
        case 20:
            doc["event"] = "Fail To Communicate restore IP Receiver 1 (IP)";
            break;
        ///////////
        case 21:
            doc["event"] = "Fail To Communicate restore IP Receiver 2 (IP)";
            break;
            ///////////
        case 99:
            doc["event"] = "Any new trouble restored event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 46:
        topic = MQTT_EVENT_TOPIC;

        switch (subgroup)
        {
            ///////////
        case 0:
            doc["event"] = "Bus module communication fault";
            break;
            ///////////
        case 1:
            doc["event"] = "Bus module tamper trouble";
            break;
            ///////////
        case 2:
            doc["event"] = "Bus module power fail";
            break;
            ///////////
        case 3:
            doc["event"] = "Bus module battery failure";
            break;
            ///////////
        case 99:
            doc["event"] = "Any bus module trouble event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 47:
        topic = MQTT_EVENT_TOPIC;

        switch (subgroup)
        {
            ///////////
        case 0:
            doc["event"] = "Bus module communication fault restore";
            break;
            ///////////
        case 1:
            doc["event"] = "Bus module tamper trouble restore";
            break;
            ///////////
        case 2:
            doc["event"] = "Bus module power fail restore";
            break;
            ///////////
        case 3:
            doc["event"] = "Bus module battery failure restore";
            break;
            ///////////
        case 99:
            doc["event"] = "Any bus module trouble restored event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 48:
        topic = MQTT_EVENT_TOPIC;

        switch (subgroup)
        {
            ///////////
        case 0:
            doc["event"] = "System power up";
            break;
            ///////////
        case 1:
            doc["event"] = "Reporting test";
            break;
            ///////////
        case 2:
            doc["event"] = "Software log on";
            break;
            ///////////
        case 3:
            doc["event"] = "Software log off";
            break;
            ///////////
        case 4:
            doc["event"] = "Installer in programming mode";
            break;
            ///////////
        case 5:
            doc["event"] = "Installer exited programming mode";
            break;
            ///////////
        case 6:
            doc["event"] = "Maintenance in programming mode";
            break;
            ///////////
        case 7:
            doc["event"] = "Maintenance exited programming mode";
            break;
            ///////////
        case 8:
            doc["event"] = "Closing delinquency delay elapsed";
            break;
            ///////////
        case 99:
            doc["event"] = "Any special event";
            break;
        }
        break;
        ////////////////////////////////////////////

    case 49:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Low battery on zone";
        break;
        ////////////////////////////////////////////

    case 50:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Low battery on zone restore";
        break;
        ////////////////////////////////////////////

    case 51:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Zone supervision trouble";
        break;
        ////////////////////////////////////////////

    case 52:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Zone supervision restore";
        break;
        ////////////////////////////////////////////

    case 53:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Wireless module " + String(subgroup) + " supervision trouble";
        break;
        ////////////////////////////////////////////

    case 54:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Wireless module " + String(subgroup) + " supervision restore";
        break;
        ////////////////////////////////////////////

    case 55:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Wireless module " + String(subgroup) + " tamper trouble";
        break;
        ////////////////////////////////////////////

    case 56:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Wireless module " + String(subgroup) + " tamper restore";
        break;
        ////////////////////////////////////////////

    case 57:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Non-medical alarm (paramedic) user " + String(subgroup);
        break;
        ////////////////////////////////////////////

    case 58:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Zone forced";
        break;
        ////////////////////////////////////////////

    case 59:
        topic = MQTT_ZONE_TOPIC;
        topic += String(subgroup);
        doc["event"] = "Zone included";
        break;
        ////////////////////////////////////////////

    case 64:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "System Status " + String(subgroup);
        break;

    default:
        topic = MQTT_EVENT_TOPIC;
        doc["event"] = "Undocumented";
    }

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

void panel_2_zone_status_names_double(uint8_t bit5, uint8_t bit4)
{
    if (bit5 && bit4)
        mesaj = "In Fire Delay";
    if (!bit5 && bit4)
        mesaj = "In Entry Delay";
    if (bit5 && !bit4)
        mesaj = "In Intellizone Delay";
    if (!bit5 && !bit4)
        mesaj = "In No Delay";
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
