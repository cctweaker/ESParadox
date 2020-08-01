//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######                                 ###
//  #     #   ##   #    # ###### #          #  #    # ######  ####
//  #     #  #  #  ##   # #      #          #  ##   # #      #    #
//  ######  #    # # #  # #####  #          #  # #  # #####  #    #
//  #       ###### #  # # #      #          #  #  # # #      #    #
//  #       #    # #   ## #      #          #  #   ## #      #    #
//  #       #    # #    # ###### ######    ### #    # #       ####

void translate_panel_info()
{

    DynamicJsonDocument doc(256);

    switch (PPI)
    {
    case 0x15:
        doc["Model"] = F("SP5500");
        break;
    case 0x16:
        doc["Model"] = F("SP6000");
        break;
    case 0x17:
        doc["Model"] = F("SP7000");
        break;
    case 0x40:
        doc["Model"] = F("MG5000");
        break;
    case 0x41:
        doc["Model"] = F("MG5050");
        break;
    }

    doc["Firmware"] = String(PFV) + "." + String(PFB) + "." + String(PFR);

    sprintf(mqtt_tx, "%02X%02X", PPID1, PPID2);
    doc["PPID"] = mqtt_tx;

    if (PPI > 0x20)
    {
        doc["Transceiver"]["Family"] = String(TF);
        doc["Transceiver"]["HW"] = String(THR);
        doc["Transceiver"]["FW"] = String(TFV) + "." + String(TFB) + "." + String(TFR);
        doc["Transceiver"]["Noise floor lvl."] = String(TNFL);
        doc["Transceiver"]["Noise floor"] = NFLTH ? "high" : "low, ok";
        doc["Transceiver"]["Constant carrier"] = CCD ? "warning" : "absent, ok";
    }

    serializeJson(doc, mqtt_tx);
    doc.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######                                   ###
//  #     #   ##   #    # ###### #          #   #     ##### #####   ####  #    # #####  #      ######  ####
//  #     #  #  #  ##   # #      #         #     #      #   #    # #    # #    # #    # #      #      #
//  ######  #    # # #  # #####  #         #     #      #   #    # #    # #    # #####  #      #####   ####
//  #       ###### #  # # #      #         #     #      #   #####  #    # #    # #    # #      #           #
//  #       #    # #   ## #      #          #   #       #   #   #  #    # #    # #    # #      #      #    #
//  #       #    # #    # ###### ######      ###        #   #    #  ####   ####  #####  ###### ######  ####

void translate_panel_0_trouble()
{
    strcpy(mqtt_tx, "no trouble");

    DynamicJsonDocument doc(256);

    if (pdx_rx_buffer[4] || pdx_rx_buffer[5] || pdx_rx_buffer[6] || pdx_rx_buffer[7] || pdx_rx_buffer[8]) // report only if events exist
    {
        // PANEL 0 BYTE 4
        if (bitRead(pdx_rx_buffer[4], 7))
            doc.add(F("Timer loss"));
        if (bitRead(pdx_rx_buffer[4], 6))
            doc.add(F("Global Fire-Loop Trouble"));
        if (bitRead(pdx_rx_buffer[4], 5))
            doc.add(F("Global Module Tamper"));
        if (bitRead(pdx_rx_buffer[4], 4))
            doc.add(F("Global Zone Tamper"));
        if (bitRead(pdx_rx_buffer[4], 3))
            doc.add(F("Global Communication Trouble"));
        if (bitRead(pdx_rx_buffer[4], 2))
            doc.add(F("Global Bell trouble"));
        if (bitRead(pdx_rx_buffer[4], 1))
            doc.add(F("Global Power Trouble"));
        if (bitRead(pdx_rx_buffer[4], 0))
            doc.add(F("Global RF Transmitter Low Battery"));

        // PANEL 0 BYTE 5
        if (bitRead(pdx_rx_buffer[5], 7))
            doc.add(F("RF Interference"));
        if (bitRead(pdx_rx_buffer[5], 1))
            doc.add(F("Global Module Supervision"));
        if (bitRead(pdx_rx_buffer[5], 0))
            doc.add(F("Global Zone Supervision"));

        // PANEL 0 BYTE 6
        if (bitRead(pdx_rx_buffer[6], 6))
            doc.add(F("Wireless Repeater Battery Failure"));
        if (bitRead(pdx_rx_buffer[6], 5))
            doc.add(F("Wireless Repeater AC Loss"));
        if (bitRead(pdx_rx_buffer[6], 4))
            doc.add(F("Wireless Keypad Battery Failure"));
        if (bitRead(pdx_rx_buffer[6], 3))
            doc.add(F("Wireless Keypad AC Loss"));
        if (bitRead(pdx_rx_buffer[6], 2))
            doc.add(F("Auxiliary Output Overload"));
        if (bitRead(pdx_rx_buffer[6], 1))
            doc.add(F("AC Failure"));
        if (bitRead(pdx_rx_buffer[6], 0))
            doc.add(F("No/Low Battery"));

        // PANEL 0 BYTE 7
        if (bitRead(pdx_rx_buffer[7], 1))
            doc.add(F("Bell Output Overload"));
        if (bitRead(pdx_rx_buffer[7], 0))
            doc.add(F("Bell Output Disconnect"));

        // PANEL 0 BYTE 8
        if (bitRead(pdx_rx_buffer[8], 5))
            doc.add(F("Computer Fail to Communicate"));
        if (bitRead(pdx_rx_buffer[8], 4))
            doc.add(F("Voice Fail to Communicate"));
        if (bitRead(pdx_rx_buffer[8], 3))
            doc.add(F("Pager Fail to Communicate"));
        if (bitRead(pdx_rx_buffer[8], 2))
            doc.add(F("Central 2 Reporting FTC"));
        if (bitRead(pdx_rx_buffer[8], 1))
            doc.add(F("Central 1 Reporting FTC"));
        if (bitRead(pdx_rx_buffer[8], 0))
            doc.add(F("Telephone Line Trouble"));

        serializeJson(doc, mqtt_tx);
        doc.clear();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######                                   ###
//  #     #   ##   #    # ###### #          #   #     #    #   ##   #      #    # ######  ####
//  #     #  #  #  ##   # #      #         #     #    #    #  #  #  #      #    # #      #
//  ######  #    # # #  # #####  #         #     #    #    # #    # #      #    # #####   ####
//  #       ###### #  # # #      #         #     #    #    # ###### #      #    # #           #
//  #       #    # #   ## #      #          #   #      #  #  #    # #      #    # #      #    #
//  #       #    # #    # ###### ######      ###        ##   #    # ######  ####  ######  ####

void translate_panel_0_values()
{
    DynamicJsonDocument doc(256);

    doc["AC voltage"] = pdx_rx_buffer[15];
    doc["DC voltage"] = pdx_rx_buffer[16];
    doc["Battery voltage"] = pdx_rx_buffer[17];
    doc["RF Noise floor"] = pdx_rx_buffer[18];

    serializeJson(doc, mqtt_tx);
    doc.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######                                 ### ######
//  #     #   ##   #    # ###### #          #  #     #  ####
//  #     #  #  #  ##   # #      #          #  #     # #
//  ######  #    # # #  # #####  #          #  #     #  ####
//  #       ###### #  # # #      #          #  #     #      #
//  #       #    # #   ## #      #          #  #     # #    #
//  #       #    # #    # ###### ######    ### ######   ####

void translate_panel_ids(uint8_t position, uint8_t max)
{
    uint8_t i = 0;
    uint8_t b = 0;

    DynamicJsonDocument doc(256);

    for (i = 0; i < max; i++)
        for (b = 0; b < 8; b++)
            if (bitRead(pdx_rx_buffer[position + i], b))
                doc.add((i * 8 + b + 1));

    serializeJson(doc, mqtt_tx);
    doc.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######
//  #     #   ##   #####  ##### # ##### #  ####  #    #     ####  #####   ##   ##### #    #  ####
//  #     #  #  #  #    #   #   #   #   # #    # ##   #    #        #    #  #    #   #    # #
//  ######  #    # #    #   #   #   #   # #    # # #  #     ####    #   #    #   #   #    #  ####
//  #       ###### #####    #   #   #   # #    # #  # #         #   #   ######   #   #    #      #
//  #       #    # #   #    #   #   #   # #    # #   ##    #    #   #   #    #   #   #    # #    #
//  #       #    # #    #   #   #   #   #  ####  #    #     ####    #   #    #   #    ####   ####

void translate_partition_status(uint8_t position)
{
    DynamicJsonDocument doc(256);

    if (bitRead(pdx_rx_buffer[position], 7))
        doc.add(F("Pulse Alarm (fire alarm)."));
    if (bitRead(pdx_rx_buffer[position], 6))
        doc.add(F("Audible alarm."));
    if (bitRead(pdx_rx_buffer[position], 5))
        doc.add(F("Silent alarm."));
    if (bitRead(pdx_rx_buffer[position], 4))
        doc.add(F("Is / was in Alarm Indicator. (Strobe)"));
    if (bitRead(pdx_rx_buffer[position], 3))
        doc.add(F("N/U"));
    if (bitRead(pdx_rx_buffer[position], 2))
        doc.add(F("Stay Arm"));
    if (bitRead(pdx_rx_buffer[position], 1))
        doc.add(F("Sleep Arm"));
    if (bitRead(pdx_rx_buffer[position], 0))
        doc.add(F("Arm (Stay/Sleep/Full)"));

    //////////

    if (bitRead(pdx_rx_buffer[position + 1], 7))
        doc.add(F("Bell Activated"));
    if (bitRead(pdx_rx_buffer[position + 1], 6))
        doc.add(F("Auto-Arming Engaged"));
    if (bitRead(pdx_rx_buffer[position + 1], 5))
        doc.add(F("Recent Closing Delay"));
    if (bitRead(pdx_rx_buffer[position + 1], 4))
        doc.add(F("Intellizone Delay"));
    if (bitRead(pdx_rx_buffer[position + 1], 3))
        doc.add(F("At least one zone is bypassed"));
    if (bitRead(pdx_rx_buffer[position + 1], 2))
        doc.add(F("Alarm(s) is / are in the memory"));
    if (bitRead(pdx_rx_buffer[position + 1], 1))
        doc.add(F("Entry Delay"));
    if (bitRead(pdx_rx_buffer[position + 1], 0))
        doc.add(F("Exit Delay"));

    //////////

    if (bitRead(pdx_rx_buffer[position + 2], 7))
        doc.add(F("Paramedic Alarm"));
    if (bitRead(pdx_rx_buffer[position + 2], 6))
        doc.add(F("Reserved"));
    if (bitRead(pdx_rx_buffer[position + 2], 5))
        doc.add(F("Arm with a Remote"));
    if (bitRead(pdx_rx_buffer[position + 2], 4))
        doc.add(F("Transmission Delay Finished"));
    if (bitRead(pdx_rx_buffer[position + 2], 3))
        doc.add(F("Bell Delay Finished"));
    if (bitRead(pdx_rx_buffer[position + 2], 2))
        doc.add(F("Entry Delay Finished"));
    if (bitRead(pdx_rx_buffer[position + 2], 1))
        doc.add(F("Exit Delay Finished"));
    if (bitRead(pdx_rx_buffer[position + 2], 0))
        doc.add(F("Intellizone Delay Finished"));

    //////////

    if (bitRead(pdx_rx_buffer[position + 3], 7))
        doc.add(F("N/U"));
    if (bitRead(pdx_rx_buffer[position + 3], 6))
        doc.add(F("Wait window (window mode)."));
    if (bitRead(pdx_rx_buffer[position + 3], 5))
        doc.add(F("N/U"));
    if (bitRead(pdx_rx_buffer[position + 3], 4))
        doc.add(F("In remote delay."));
    if (bitRead(pdx_rx_buffer[position + 3], 3))
        doc.add(F("N/U"));
    if (bitRead(pdx_rx_buffer[position + 3], 2))
        doc.add(F("StayD mode active."));
    if (bitRead(pdx_rx_buffer[position + 3], 1))
        doc.add(F("Force Arm"));
    if (bitRead(pdx_rx_buffer[position + 3], 0))
        doc.add(F("Ready Status"));

    //////////

    serializeJson(doc, mqtt_tx);
    doc.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######
//  #     # ######   ##   #####  ######   ##   ##### ###### #####
//  #     # #       #  #  #    # #       #  #    #   #      #    #
//  ######  #####  #    # #    # #####  #    #   #   #####  #    #
//  #   #   #      ###### #####  #      ######   #   #      #####
//  #    #  #      #    # #      #      #    #   #   #      #   #
//  #     # ###### #    # #      ###### #    #   #   ###### #    #

//  #    #
//  #   #  ###### #   # #####    ##   #####
//  #  #   #       # #  #    #  #  #  #    #
//  ###    #####    #   #    # #    # #    #
//  #  #   #        #   #####  ###### #    #
//  #   #  #        #   #      #    # #    #
//  #    # ######   #   #      #    # #####

void translate_panel_repeater_keypad(uint8_t position, bool repeater)
{
    DynamicJsonDocument doc(256);

    if (repeater)
    {
        if (bitRead(pdx_rx_buffer[position], 7))
            doc.add(2);
        if (bitRead(pdx_rx_buffer[position], 6))
            doc.add(1);
    }
    else
    {
        if (bitRead(pdx_rx_buffer[position + 17], 7))
            doc.add(6);
        if (bitRead(pdx_rx_buffer[position + 17], 6))
            doc.add(5);

        if (bitRead(pdx_rx_buffer[position], 5))
            doc.add(6);
        if (bitRead(pdx_rx_buffer[position], 4))
            doc.add(5);
        if (bitRead(pdx_rx_buffer[position], 3))
            doc.add(4);
        if (bitRead(pdx_rx_buffer[position], 2))
            doc.add(3);
        if (bitRead(pdx_rx_buffer[position], 1))
            doc.add(2);
        if (bitRead(pdx_rx_buffer[position], 0))
            doc.add(1);
    }

    serializeJson(doc, mqtt_tx);
    doc.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  #######
//       #   ####  #    # ######     ####  #####   ##   ##### #    #  ####
//      #   #    # ##   # #         #        #    #  #    #   #    # #
//     #    #    # # #  # #####      ####    #   #    #   #   #    #  ####
//    #     #    # #  # # #              #   #   ######   #   #    #      #
//   #      #    # #   ## #         #    #   #   #    #   #   #    # #    #
//  #######  ####  #    # ######     ####    #   #    #   #    ####   ####

void translate_zone_status(uint8_t position)
{
    bool bit5 = false;
    bool bit4 = false;

    DynamicJsonDocument doc(256);

    if (bitRead(pdx_rx_buffer[position], 7))
        doc.add(F("Zone was in Alarm"));

    if (bitRead(pdx_rx_buffer[position], 6))
        doc.add(F("Zone is in Alarm"));

    if (bitRead(pdx_rx_buffer[position], 5))
        bit5 = true;
    if (bitRead(pdx_rx_buffer[position], 4))
        bit4 = true;

    if (bit5 && bit4)
        doc.add(F("In Fire Delay"));
    if (!bit5 && bit4)
        doc.add(F("In Entry Delay"));
    if (bit5 && !bit4)
        doc.add(F("In Intellizone Delay"));
    if (!bit5 && !bit4)
        doc.add(F("In No Delay"));

    if (bitRead(pdx_rx_buffer[position], 3))
        doc.add(F("Zone Bypassed"));
    if (bitRead(pdx_rx_buffer[position], 2))
        doc.add(F("Zone Shutdown"));
    if (bitRead(pdx_rx_buffer[position], 1))
        doc.add(F("Zone is in TX Delay"));
    if (bitRead(pdx_rx_buffer[position], 0))
        doc.add(F("Zone was Bypassed"));

    serializeJson(doc, mqtt_tx);
    doc.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  #######
//  #       #    # ###### #    # #####
//  #       #    # #      ##   #   #
//  #####   #    # #####  # #  #   #
//  #       #    # #      #  # #   #
//  #        #  #  #      #   ##   #
//  #######   ##   ###### #    #   #

void translate_event()
{
    uint8_t i = 0;

    bool is_zone = false;
    bool is_partition = false;
    bool is_bell = false;
    bool is_module = false;
    bool is_arm = false;
    bool is_alarm = false;
    bool is_trouble = false;

    char topic[128];

    DynamicJsonDocument doc(256);

    // event date/time
    sprintf(topic, "%02d%02d-%02d-%02d %02d:%02d", pdx_rx_buffer[1], pdx_rx_buffer[2], pdx_rx_buffer[3], pdx_rx_buffer[4], pdx_rx_buffer[5], pdx_rx_buffer[6]);
    doc["dt"] = topic;

    // event group
    doc["eg#"] = pdx_rx_buffer[7];

    // event subgroup
    doc["sg#"] = pdx_rx_buffer[8];

    // event partition
    doc["prt"] = pdx_rx_buffer[9];

    // device serial number
    if (pdx_rx_buffer[10] || pdx_rx_buffer[11] || pdx_rx_buffer[12] || pdx_rx_buffer[13])
    {
        sprintf(topic, "%02x%02x%02x%02x", pdx_rx_buffer[10], pdx_rx_buffer[11], pdx_rx_buffer[12], pdx_rx_buffer[13]);
        doc["sn#"] = topic;
    }

    // device label (name)
    for (i = 15; i <= 30; i++)
        topic[i - 15] = (pdx_rx_buffer[i] == 0 ? 0x20 : pdx_rx_buffer[i]);
    topic[16] = 0;
    for (i = 15; i >= 0; i--)
    {
        if (topic[i] != 0x20)
            break;
        else
            topic[i] = 0;
    }

    doc["label"] = topic;

    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //

    switch (pdx_rx_buffer[7])
    {
    case 0:
        doc["event"] = F("Zone OK");
        is_zone = true;
        break;
        ////////////////////////////////////////////

    case 1:
        doc["event"] = F("Zone open");
        is_zone = true;
        break;
        ////////////////////////////////////////////

    case 2:
        is_partition = true;

        switch (pdx_rx_buffer[8])
        {
        case 2:
            doc["event"] = F("Silent alarm");
            break;
            ///////////
        case 3:
            doc["event"] = F("Buzzer alarm");
            break;
            ///////////
        case 4:
            doc["event"] = F("Steady alarm");
            break;
            ///////////
        case 5:
            doc["event"] = F("Pulsed alarm");
            break;
            ///////////
        case 6:
            doc["event"] = F("Strobe");
            break;
            ///////////
        case 7:
            doc["event"] = F("Alarm stopped");
            break;
            ///////////
        case 8:
            doc["event"] = F("Squawk ON");
            break;
            ///////////
        case 9:
            doc["event"] = F("Squawk OFF");
            break;
            ///////////
        case 10:
            doc["event"] = F("Ground start");
            break;
            ///////////
        case 11:
            doc["event"] = F("Disarm partition");
            break;
            ///////////
        case 12:
            doc["event"] = F("Arm partition");
            break;
            ///////////
        case 13:
            doc["event"] = F("Entry delay started");
            break;
            ///////////
        case 14:
            doc["event"] = F("Exit delay started");
            sprintf(topic, "%s/%s/%s%s/arm", LOC, TIP, NAME, PUB);
            client.publish(topic, "1", true, 0);
            break;
            ///////////
        case 15:
            doc["event"] = F("Pre-alarm delay");
            break;
            ///////////
        case 16:
            doc["event"] = F("Report confirmation");
            break;
            ///////////
        case 99:
            doc["event"] = F("Any partition status event");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 3:
        is_bell = true;

        switch (pdx_rx_buffer[8])
        {
        case 0:
            doc["event"] = F("Bell OFF");
            break;
            ///////////
        case 1:
            doc["event"] = F("Bell ON");
            break;
            ///////////
        case 2:
            doc["event"] = F("Bell squawk arm");
            break;
            ///////////
        case 3:
            doc["event"] = F("Bell squawk disarm");
            break;
            ///////////
        case 99:
            doc["event"] = F("Any bell status event");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 6:

        switch (pdx_rx_buffer[8])
        {
        case 0:
            doc["event"] = F("Telephone line trouble");
            break;
            ///////////
        case 1:
            doc["event"] = F("ENTER + CLEAR + POWER pressed");
            break;
            ///////////
        case 2:
            doc["event"] = F("N/A");
            break;
            ///////////
        case 3:
            doc["event"] = F("Arm in stay mode");
            break;
            ///////////
        case 4:
            doc["event"] = F("Arm in sleep mode");
            break;
            ///////////
        case 5:
            doc["event"] = F("Arm in force mode");
            break;
            ///////////
        case 6:
            doc["event"] = F("Full arm when armed in stay mode");
            break;
            ///////////
        case 7:
            doc["event"] = F("PC fail to communicate");
            break;
            ///////////
        case 8:
            doc["event"] = F("Utility Key 1 pressed (1+2)");
            break;
            ///////////
        case 9:
            doc["event"] = F("Utility Key 2 pressed (4+5)");
            break;
            ///////////
        case 10:
            doc["event"] = F("Utility Key 3 pressed (7+8)");
            break;
            ///////////
        case 11:
            doc["event"] = F("Utility Key 4 pressed (2+3)");
            break;
            ///////////
        case 12:
            doc["event"] = F("Utility Key 5 pressed (5+6)");
            break;
            ///////////
        case 13:
            doc["event"] = F("Utility Key 6 pressed (8+9)");
            break;
            ///////////
        case 14:
            doc["event"] = F("Tamper generated alarm");
            break;
            ///////////
        case 15:
            doc["event"] = F("Supervision loss generated alarm");
            break;
            ///////////
        case 20:
            doc["event"] = F("Full arm when armed in sleep mode");
            break;
            ///////////
        case 21:
            doc["event"] = F("Firmware upgrade");
            break;
            ///////////
        case 23:
            doc["event"] = F("StayD mode activated");
            break;
            ///////////
        case 24:
            doc["event"] = F("StayD mode deactivated");
            break;
            ///////////
        case 25:
            doc["event"] = F("IP Registration status change");
            break;
            ///////////
        case 26:
            doc["event"] = F("GPRS Registration status change");
            break;
            ///////////
        case 99:
            doc["event"] = F("Any non-reportable event");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 8:
        sprintf_P(topic, PSTR("Remote %d, pressed button B"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 9:
        sprintf_P(topic, PSTR("Remote %d, pressed button C"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 10:
        sprintf_P(topic, PSTR("Remote %d, pressed button D"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 11:
        sprintf_P(topic, PSTR("Remote %d, pressed button E"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 12:
        is_zone = true;
        doc["event"] = F("Cold start wireless zone");
        break;
        ////////////////////////////////////////////

    case 13:
        is_module = true;
        sprintf_P(topic, PSTR("Cold start wireless module %d"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 14:
        sprintf_P(topic, PSTR("Bypass programming user  %d"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 15:
        sprintf_P(topic, PSTR("User %d code activated output"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 16:
        is_zone = true;
        doc["event"] = F("Wireless smoke maintenance signal");
        break;
        ////////////////////////////////////////////

    case 17:
        is_zone = true;
        doc["event"] = F("Delay zone alarm transmission");
        break;
        ////////////////////////////////////////////

    case 18:
        is_zone = true;
        doc["event"] = F("Zone signal strength weak 1");
        break;
        ////////////////////////////////////////////

    case 19:
        is_zone = true;
        doc["event"] = F("Zone signal strength weak 2");
        break;
        ////////////////////////////////////////////

    case 20:
        is_zone = true;
        doc["event"] = F("Zone signal strength weak 3");
        break;
        ////////////////////////////////////////////

    case 21:
        is_zone = true;
        doc["event"] = F("Zone signal strength weak 4");
        break;
        ////////////////////////////////////////////

    case 22:
        sprintf_P(topic, PSTR("Remote %d, button option 5"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 23:
        sprintf_P(topic, PSTR("Remote %d, button option 6"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 24:
        is_zone = true;
        doc["event"] = F("Fire Delay started");
        break;
        ////////////////////////////////////////////

    case 26:
        switch (pdx_rx_buffer[8])
        {
            ///////////
        case 1:
            doc["event"] = F("Software Access: WinLoad/Babyware direct");
            break;
            ///////////
        case 2:
            doc["event"] = F("Software Access: WinLoad/Babyware IP module");
            break;
            ///////////
        case 3:
            doc["event"] = F("Software Access: WinLoad/Babyware GSM module");
            break;
            ///////////
        case 4:
            doc["event"] = F("Software Access: WinLoad/Babyware modem");
            break;
            ///////////
        case 9:
            doc["event"] = F("Software Access: IP100 direct");
            break;
            ///////////
        case 10:
            doc["event"] = F("Software Access: VDMP3 direct");
            break;
            ///////////
        case 11:
            doc["event"] = F("Software Access: Voice through GSM module");
            break;
            ///////////
        case 12:
            doc["event"] = F("Software Access: Remote access");
            break;
            ///////////
        case 13:
            doc["event"] = F("Software Access: SMS through GSM module");
            break;
            ///////////
        case 99:
            doc["event"] = F("Software Access: Any software access");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 27:
        switch (pdx_rx_buffer[8])
        {
            ///////////
        case 0:
            doc["event"] = F("A bus module was added");
            break;
            ///////////
        case 1:
            doc["event"] = F("A bus module was removed");
            break;
            ///////////
        case 2:
            doc["event"] = F("2-way RF Module Communication Failure");
            break;
            ///////////
        case 3:
            doc["event"] = F("2-way RF Module Communication Restored");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 28:
        is_zone = true;
        doc["event"] = F("StayD pass acknowledged");
        break;
        ////////////////////////////////////////////

    case 29:
        is_arm = true;
        sprintf_P(topic, PSTR("Arming with user %d"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 30:
        is_arm = true;

        switch (pdx_rx_buffer[8])
        {
            ///////////
        case 0:
            doc["event"] = F("Auto-arming (on time/no movement)");
            break;
            ///////////
        case 1:
            doc["event"] = F("Late to close");
            break;
            ///////////
        case 2:
            doc["event"] = F("No movement arming");
            break;
            ///////////
        case 3:
            doc["event"] = F("Partial arming");
            break;
        ///////////
        case 4:
            doc["event"] = F("Quick arming");
            break;
        ///////////
        case 5:
            doc["event"] = F("Arming through WinLoad / BabyWare");
            break;
        ///////////
        case 6:
            doc["event"] = F("Arming with keyswitch");
            break;
            ///////////
        case 99:
            doc["event"] = F("Any special arming");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 31:
        is_arm = true;
        sprintf_P(topic, PSTR("Disarming with user %d"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 32:
        is_arm = true;
        sprintf_P(topic, PSTR("Disarming after alarm with user %d"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 33:
        is_arm = true;
        sprintf_P(topic, PSTR("Alarm cancelled with user %d"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 34:
        is_arm = true;

        switch (pdx_rx_buffer[8])
        {
            ///////////
        case 0:
            doc["event"] = F("Auto-arm cancelled (on time/no movement)");
            break;
            ///////////
        case 1:
            doc["event"] = F("Disarming through WinLoad / BabyWare");
            break;
            ///////////
        case 2:
            doc["event"] = F("Disarming through WinLoad / BabyWare after alarm");
            break;
            ///////////
        case 3:
            doc["event"] = F("Alarm cancelled through WinLoad / BabyWare");
            break;
        ///////////
        case 4:
            doc["event"] = F("Paramedical alarm cancelled");
            break;
        ///////////
        case 5:
            doc["event"] = F("Disarm with keyswitch");
            break;
        ///////////
        case 6:
            doc["event"] = F("Disarm with keyswitch after an alarm");
            break;
        ///////////
        case 7:
            doc["event"] = F("Alarm cancelled with keyswitch");
            break;
            ///////////
        case 99:
            doc["event"] = F("Any special disarming");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 35:
        is_zone = true;
        doc["event"] = F("Zone bypassed");
        break;
        ////////////////////////////////////////////

    case 36:
        is_zone = true;
        doc["event"] = F("Zone in alarm");
        break;
        ////////////////////////////////////////////

    case 37:
        is_zone = true;
        doc["event"] = F("Fire alarm");
        break;
        ////////////////////////////////////////////

    case 38:
        is_zone = true;
        doc["event"] = F("Zone alarm restore");
        break;
        ////////////////////////////////////////////

    case 39:
        is_zone = true;
        doc["event"] = F("Fire alarm restore");
        break;
        ////////////////////////////////////////////

    case 40:
        is_alarm = true;

        switch (pdx_rx_buffer[8])
        {
            ///////////
        case 0:
            doc["event"] = F("Panic non-medical emergency");
            break;
            ///////////
        case 1:
            doc["event"] = F("Panic medical");
            break;
            ///////////
        case 2:
            doc["event"] = F("Panic fire");
            break;
            ///////////
        case 3:
            doc["event"] = F("Recent closing");
            break;
        ///////////
        case 4:
            doc["event"] = F("Global shutdown");
            break;
        ///////////
        case 5:
            doc["event"] = F("Duress alarm");
            break;
        ///////////
        case 6:
            doc["event"] = F("Keyboard lockout");
            break;
            ///////////
        case 99:
            doc["event"] = F("Any special alarm event");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 41:
        is_zone = true;
        doc["event"] = F("Zone shutdown");
        break;
        ////////////////////////////////////////////

    case 42:
        is_zone = true;
        doc["event"] = F("Zone tampered");
        break;
        ////////////////////////////////////////////

    case 43:
        is_zone = true;
        doc["event"] = F("Zone tamper restore");
        break;
        ////////////////////////////////////////////

    case 44:
        is_trouble = true;

        switch (pdx_rx_buffer[8])
        {
        case 1:
            doc["event"] = F("AC failure");
            break;
            ///////////
        case 2:
            doc["event"] = F("Battery failure");
            break;
            ///////////
        case 3:
            doc["event"] = F("Auxiliary current overload");
            break;
            ///////////
        case 4:
            doc["event"] = F("Bell current overload");
            break;
        ///////////
        case 5:
            doc["event"] = F("Bell disconnected");
            break;
        ///////////
        case 6:
            doc["event"] = F("Clock loss");
            break;
        ///////////
        case 7:
            doc["event"] = F("Fire loop trouble");
            break;
        ///////////
        case 8:
            doc["event"] = F("Fail to communicate to monitoring station telephone #1");
            break;
        ///////////
        case 9:
            doc["event"] = F("Fail to communicate to monitoring station telephone #2");
            break;
        ///////////
        case 11:
            doc["event"] = F("Fail to communicate to voice report");
            break;
        ///////////
        case 12:
            doc["event"] = F("RF jamming");
            break;
        ///////////
        case 13:
            doc["event"] = F("GSM RF jamming");
            break;
        ///////////
        case 14:
            doc["event"] = F("GSM no service");
            break;
        ///////////
        case 15:
            doc["event"] = F("GSM supervision lost");
            break;
        ///////////
        case 16:
            doc["event"] = F("Fail To Communicate IP Receiver 1 (GPRS)");
            break;
        ///////////
        case 17:
            doc["event"] = F("Fail To Communicate IP Receiver 2 (GPRS)");
            break;
        ///////////
        case 18:
            doc["event"] = F("IP Module No Service");
            break;
        ///////////
        case 19:
            doc["event"] = F("IP Module Supervision Loss");
            break;
        ///////////
        case 20:
            doc["event"] = F("Fail To Communicate IP Receiver 1 (IP)");
            break;
        ///////////
        case 21:
            doc["event"] = F("Fail To Communicate IP Receiver 2 (IP)");
            break;
            ///////////
        case 99:
            doc["event"] = F("Any new trouble event");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 45:
        is_trouble = true;

        switch (pdx_rx_buffer[8])
        {
        case 0:
            doc["event"] = F("Telephone line restored");
            break;
            ///////////
        case 1:
            doc["event"] = F("AC failure restore");
            break;
            ///////////
        case 2:
            doc["event"] = F("Battery failure restore");
            break;
            ///////////
        case 3:
            doc["event"] = F("Auxiliary current overload restore");
            break;
            ///////////
        case 4:
            doc["event"] = F("Bell current overload restore");
            break;
        ///////////
        case 5:
            doc["event"] = F("Bell disconnected restore");
            break;
        ///////////
        case 6:
            doc["event"] = F("Clock loss restore");
            break;
        ///////////
        case 7:
            doc["event"] = F("Fire loop trouble restore");
            break;
        ///////////
        case 8:
            doc["event"] = F("Fail to communicate to monitoring station telephone #1 restore");
            break;
        ///////////
        case 9:
            doc["event"] = F("Fail to communicate to monitoring station telephone #2 restore");
            break;
        ///////////
        case 11:
            doc["event"] = F("Fail to communicate to voice report restore");
            break;
        ///////////
        case 12:
            doc["event"] = F("RF jamming restore");
            break;
        ///////////
        case 13:
            doc["event"] = F("GSM RF jamming restore");
            break;
        ///////////
        case 14:
            doc["event"] = F("GSM no service restore");
            break;
        ///////////
        case 15:
            doc["event"] = F("GSM supervision lost restore");
            break;
        ///////////
        case 16:
            doc["event"] = F("Fail To Communicate restore IP Receiver 1 (GPRS)");
            break;
        ///////////
        case 17:
            doc["event"] = F("Fail To Communicate restore IP Receiver 2 (GPRS)");
            break;
        ///////////
        case 18:
            doc["event"] = F("IP Module No Service restore");
            break;
        ///////////
        case 19:
            doc["event"] = F("IP Module Supervision Loss restore");
            break;
        ///////////
        case 20:
            doc["event"] = F("Fail To Communicate restore IP Receiver 1 (IP)");
            break;
        ///////////
        case 21:
            doc["event"] = F("Fail To Communicate restore IP Receiver 2 (IP)");
            break;
            ///////////
        case 99:
            doc["event"] = F("Any new trouble restored event");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 46:
        is_trouble = true;

        switch (pdx_rx_buffer[8])
        {
            ///////////
        case 0:
            doc["event"] = F("Bus module communication fault");
            break;
            ///////////
        case 1:
            doc["event"] = F("Bus module tamper trouble");
            break;
            ///////////
        case 2:
            doc["event"] = F("Bus module power fail");
            break;
            ///////////
        case 3:
            doc["event"] = F("Bus module battery failure");
            break;
            ///////////
        case 99:
            doc["event"] = F("Any bus module trouble event");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 47:
        is_trouble = true;

        switch (pdx_rx_buffer[8])
        {
            ///////////
        case 0:
            doc["event"] = F("Bus module communication fault restore");
            break;
            ///////////
        case 1:
            doc["event"] = F("Bus module tamper trouble restore");
            break;
            ///////////
        case 2:
            doc["event"] = F("Bus module power fail restore");
            break;
            ///////////
        case 3:
            doc["event"] = F("Bus module battery failure restore");
            break;
            ///////////
        case 99:
            doc["event"] = F("Any bus module trouble restored event");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 48:
        switch (pdx_rx_buffer[8])
        {
            ///////////
        case 0:
            doc["event"] = F("System power up");
            break;
            ///////////
        case 1:
            doc["event"] = F("Reporting test");
            break;
            ///////////
        case 2:
            doc["event"] = F("Software log on");
            break;
            ///////////
        case 3:
            doc["event"] = F("Software log off");
            break;
            ///////////
        case 4:
            doc["event"] = F("Installer in programming mode");
            break;
            ///////////
        case 5:
            doc["event"] = F("Installer exited programming mode");
            break;
            ///////////
        case 6:
            doc["event"] = F("Maintenance in programming mode");
            break;
            ///////////
        case 7:
            doc["event"] = F("Maintenance exited programming mode");
            break;
            ///////////
        case 8:
            doc["event"] = F("Closing delinquency delay elapsed");
            break;
            ///////////
        case 99:
            doc["event"] = F("Any special event");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 49:
        is_zone = true;
        doc["event"] = F("Low battery on zone");
        break;
        ////////////////////////////////////////////

    case 50:
        is_zone = true;
        doc["event"] = F("Low battery on zone restore");
        break;
        ////////////////////////////////////////////

    case 51:
        is_zone = true;
        doc["event"] = F("Zone supervision trouble");
        break;
        ////////////////////////////////////////////

    case 52:
        is_zone = true;
        doc["event"] = F("Zone supervision restore");
        break;
        ////////////////////////////////////////////

    case 53:
        is_module = true;
        sprintf_P(topic, PSTR("Wireless module %d supervision trouble"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 54:
        is_module = true;
        sprintf_P(topic, PSTR("Wireless module %d supervision restore"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 55:
        is_module = true;
        sprintf_P(topic, PSTR("Wireless module %d tamper trouble"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 56:
        is_module = true;
        sprintf_P(topic, PSTR("Wireless module %d tamper restore"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 57:
        is_alarm = true;
        sprintf_P(topic, PSTR("Non-medical alarm (paramedic) user %d"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 58:
        is_zone = true;
        doc["event"] = F("Zone forced");
        break;
        ////////////////////////////////////////////

    case 59:
        is_zone = true;
        doc["event"] = F("Zone included");
        break;
        ////////////////////////////////////////////

    case 64:
        sprintf_P(topic, PSTR("System Status  %d"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;

    default:
        doc["event"] = F("Undocumented");
    }

    if (is_zone)
    {
        sprintf(topic, "%s/%s/%s%s/zone/%d", LOC, TIP, NAME, PUB, pdx_rx_buffer[8]);
        client.publish(topic, (const char *)doc["event"], true, 0);

        sprintf(topic, "%s/%s/%s%s/zone/%d/label", LOC, TIP, NAME, PUB, pdx_rx_buffer[8]);
        client.publish(topic, (const char *)doc["label"], true, 0);

        sprintf(topic, "%s/%s/%s%s/zone/%d/sn", LOC, TIP, NAME, PUB, pdx_rx_buffer[8]);
        client.publish(topic, (const char *)doc["sn#"], true, 0);
    }

    if (is_partition)
    {
        sprintf(topic, "%s/%s/%s%s/partition/%d", LOC, TIP, NAME, PUB, pdx_rx_buffer[9] + 1);
        client.publish(topic, (const char *)doc["event"], true, 0);

        if (pdx_rx_buffer[8] == 12) // arm partition
        {
            sprintf(topic, "%s/%s/%s%s/arm", LOC, TIP, NAME, PUB);
            client.publish(topic, "1", true, 0);
        }
        if (pdx_rx_buffer[8] == 11) // disarm partition
        {
            sprintf(topic, "%s/%s/%s%s/arm", LOC, TIP, NAME, PUB);
            client.publish(topic, "0", true, 0);
        }
    }

    if (is_bell)
    {
        sprintf(topic, "%s/%s/%s%s/bell", LOC, TIP, NAME, PUB);
        client.publish(topic, (const char *)doc["event"], true, 0);
    }

    if (is_module)
    {
        sprintf(topic, "%s/%s/%s%s/module/%d", LOC, TIP, NAME, PUB, pdx_rx_buffer[8]);
        client.publish(topic, (const char *)doc["event"], true, 0);
    }

    if (is_arm)
    {
        sprintf(topic, "%s/%s/%s%s/arm/event", LOC, TIP, NAME, PUB);
        client.publish(topic, (const char *)doc["event"], true, 0);
    }

    if (is_alarm)
    {
        sprintf(topic, "%s/%s/%s%s/alarm/event", LOC, TIP, NAME, PUB);
        client.publish(topic, (const char *)doc["event"], true, 0);
    }

    if (is_trouble)
    {
        sprintf(topic, "%s/%s/%s%s/trouble/event", LOC, TIP, NAME, PUB);
        client.publish(topic, (const char *)doc["event"], true, 0);
    }

    serializeJson(doc, mqtt_tx);
    doc.clear();

    sprintf(topic, "%s/%s/%s%s/event", LOC, TIP, NAME, PUB);
    client.publish(topic, mqtt_tx, true, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////