void mqtt_loop()
{
    if (panel_status_0_read)
        send_mqtt_panel0_data();

    if (panel_status_1_read)
        send_mqtt_panel1_data();

    if (panel_status_2_read)
        send_mqtt_panel2_data();

    if (panel_status_3_read)
        send_mqtt_panel3_data();

    if (panel_status_4_read)
        send_mqtt_panel4_data();

    if (panel_status_5_read)
        send_mqtt_panel5_data();

    if ((unsigned long)(millis() - last_heartbeat) > heartbeat_period)
    {
        last_heartbeat = millis();
        client.publish(MQTT_HB_TOPIC, "pc: " + String(panel_connection) + " | psr: " + String(panel_status_request), false, 0);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void mqtt_setup()
{
    client.begin(MQTT_HOST, MQTT_PORT, net);
    client.setWill(MQTT_WILL_TOPIC, "0", true, 0);
    client.onMessage(messageReceived);
    mqtt_connect();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void mqtt_connect()
{
    uint8_t i = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        i++;
        if (i > 200)
            ESP.restart();
    }

    // certificare
    BearSSL::X509List cert(digicert);
    net.setTrustAnchors(&cert);

    while (!client.connect(HOSTNAME, MQTT_USER, MQTT_PASS))
    {
        delay(100);
        i++;
        if (i > 200)
            ESP.restart();
    }

    client.subscribe(MQTT_COMMAND_TOPIC, 0);
    client.publish(MQTT_WILL_TOPIC, "1", true, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ########  ######  ######## #### ##     ## ######## ########
// ##     ## ##       ##    ## ##        ##  ##     ## ##       ##     ##
// ##     ## ##       ##       ##        ##  ##     ## ##       ##     ##
// ########  ######   ##       ######    ##  ##     ## ######   ##     ##
// ##   ##   ##       ##       ##        ##   ##   ##  ##       ##     ##
// ##    ##  ##       ##    ## ##        ##    ## ##   ##       ##     ##
// ##     ## ########  ######  ######## ####    ###    ######## ########
//////////////////////////////////////////////////////////////////////////////////////////

void messageReceived(String &topic, String &payload)
{
    StaticJsonDocument<256> doc;
    deserializeJson(doc, payload);
    yield();

    command = doc["cmd"];
    subcommand = doc["scmd"];

    if (doc["time"])
    {
        panel_set_date_time = true;
    }

    doc.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  ######## ##    ## ########
// ##    ## ##       ###   ## ##     ##
// ##       ##       ####  ## ##     ##
//  ######  ######   ## ## ## ##     ##
//       ## ##       ##  #### ##     ##
// ##    ## ##       ##   ### ##     ##
//  ######  ######## ##    ## ########
//////////////////////////////////////////////////////////////////////////////////////////

void send_to_mqtt()
{
    got_mqtt_data = false;
    client.publish(MQTT_EVENT_TOPIC, mesaj, true, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########     ###    ##      ##
// ##     ##   ## ##   ##  ##  ##
// ##     ##  ##   ##  ##  ##  ##
// ########  ##     ## ##  ##  ##
// ##   ##   ######### ##  ##  ##
// ##    ##  ##     ## ##  ##  ##
// ##     ## ##     ##  ###  ###
//////////////////////////////////////////////////////////////////////////////////////////

void send_raw_to_mqtt()
{
    uint8_t i = 0;
    mesaj = "";

    for (i = 0; i < paradox_message_length; i++)
    {
        mesaj += String(paradox_rx[i], HEX);
        mesaj += " ";
    }

    client.publish(MQTT_RAW_TOPIC, mesaj, false, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########     ###    ##    ## ######## ##          ########     ###    ########    ###
// ##     ##   ## ##   ###   ## ##       ##          ##     ##   ## ##      ##      ## ##
// ##     ##  ##   ##  ####  ## ##       ##          ##     ##  ##   ##     ##     ##   ##
// ########  ##     ## ## ## ## ######   ##          ##     ## ##     ##    ##    ##     ##
// ##        ######### ##  #### ##       ##          ##     ## #########    ##    #########
// ##        ##     ## ##   ### ##       ##          ##     ## ##     ##    ##    ##     ##
// ##        ##     ## ##    ## ######## ########    ########  ##     ##    ##    ##     ##
//////////////////////////////////////////////////////////////////////////////////////////

void send_mqtt_panel_data()
{
    panel_data_login = false;
    StaticJsonDocument<1024> doc;

    switch (PPI)
    {
    case 0x15:
        doc["model"] = "SP5500";
        break;
    case 0x16:
        doc["model"] = "SP6000";
        break;
    case 0x17:
        doc["model"] = "SP7000";
        break;
    case 0x40:
        doc["model"] = "MG5000";
        break;
    case 0x41:
        doc["model"] = "MG5050";
        break;
    }

    doc["panel fw"] = String(PFV) + "." + String(PFB) + "." + String(PFR);

    mesaj = "";
    if (PPID1 < 0x10)
        mesaj += "0";
    mesaj += String(PPID1, HEX);
    if (PPID2 < 0x10)
        mesaj += "0";
    mesaj += String(PPID2, HEX);
    doc["prog. panel ID"] = mesaj;

    if (PPI > 0x20)
    {
        doc["transceiver"]["family"] = String(TF);
        doc["transceiver"]["hardware"] = String(THR);
        doc["transceiver"]["firmware"] = String(TFV) + "." + String(TFB) + "." + String(TFR);
        doc["transceiver"]["noise floor lvl."] = String(TNFL);
        doc["transceiver"]["noise floor"] = NFLTH ? "high" : "low, ok";
        doc["transceiver"]["Constant carrier"] = CCD ? "warning" : "absent, ok";
    }

    mesaj = "";
    serializeJson(doc, mesaj);
    doc.clear();

    client.publish(MQTT_PANEL_HW_INFO_TOPIC, mesaj, true, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########     ###    ##    ## ######## ##            #####
// ##     ##   ## ##   ###   ## ##       ##           ##   ##
// ##     ##  ##   ##  ####  ## ##       ##          ##     ##
// ########  ##     ## ## ## ## ######   ##          ##     ##
// ##        ######### ##  #### ##       ##          ##     ##
// ##        ##     ## ##   ### ##       ##           ##   ##
// ##        ##     ## ##    ## ######## ########      #####
//////////////////////////////////////////////////////////////////////////////////////////

void send_mqtt_panel0_data()
{
    panel_status_0_read = false;

    uint8_t i = 0;
    uint8_t b = 0;

    StaticJsonDocument<1024> doc;

    if (PS0[0] || PS0[1] || PS0[2] || PS0[3] || PS0[4]) // report only if events exist
    {
        // PANEL 0 BYTE 4
        if (bitRead(PS0[0], 7))
            doc.add("Timer loss");
        if (bitRead(PS0[0], 6))
            doc.add("Global Fire-Loop Trouble");
        if (bitRead(PS0[0], 5))
            doc.add("Global Module Tamper");
        if (bitRead(PS0[0], 4))
            doc.add("Global Zone Tamper");
        if (bitRead(PS0[0], 3))
            doc.add("Global Communication Trouble");
        if (bitRead(PS0[0], 2))
            doc.add("Global Bell trouble");
        if (bitRead(PS0[0], 1))
            doc.add("Global Power Trouble");
        if (bitRead(PS0[0], 0))
            doc.add("Global RF Transmitter Low Battery");

        // PANEL 0 BYTE 5
        if (bitRead(PS0[1], 7))
            doc.add("RF Interference");
        if (bitRead(PS0[1], 1))
            doc.add("Global Module Supervision");
        if (bitRead(PS0[1], 0))
            doc.add("Global Zone Supervision");

        // PANEL 0 BYTE 6
        if (bitRead(PS0[2], 6))
            doc.add("Wireless Repeater Battery Failure");
        if (bitRead(PS0[2], 5))
            doc.add("Wireless Repeater AC Loss");
        if (bitRead(PS0[2], 4))
            doc.add("Wireless Keypad Battery Failure");
        if (bitRead(PS0[2], 3))
            doc.add("Wireless Keypad AC Loss");
        if (bitRead(PS0[2], 2))
            doc.add("Auxiliary Output Overload");
        if (bitRead(PS0[2], 1))
            doc.add("AC Failure");
        if (bitRead(PS0[2], 0))
            doc.add("No/Low Battery");

        // PANEL 0 BYTE 7
        if (bitRead(PS0[3], 1))
            doc.add("Bell Output Overload");
        if (bitRead(PS0[3], 0))
            doc.add("Bell Output Disconnect");

        // PANEL 0 BYTE 8
        if (bitRead(PS0[4], 5))
            doc.add("Computer Fail to Communicate");
        if (bitRead(PS0[4], 4))
            doc.add("Voice Fail to Communicate");
        if (bitRead(PS0[4], 3))
            doc.add("Pager Fail to Communicate");
        if (bitRead(PS0[4], 2))
            doc.add("Central 2 Reporting FTC");
        if (bitRead(PS0[4], 1))
            doc.add("Central 1 Reporting FTC");
        if (bitRead(PS0[4], 0))
            doc.add("Telephone Line Trouble");

        mesaj = "";
        serializeJson(doc, mesaj);
        doc.clear();
    }
    else
    {
        mesaj = "none";
    }

    // SEND TROUBLE STATUS
    client.publish(MQTT_PANEL_TROUBLE_TOPIC, mesaj, false, 0);

    if (PS0[15] || PS0[16] || PS0[17] || PS0[18]) // report only if events exist
    {
        // PANEL 0 BYTE 19-22
        for (i = 0; i < 4; i++)
        {
            for (b = 0; b < 8; b++)
            {
                if (bitRead(PS0[15 + i], b))
                    doc.add("Zone " + String(i * 8 + b + 1));
            }
        }
        mesaj = "";
        serializeJson(doc, mesaj);
        doc.clear();
    }
    else
    {
        mesaj = "none";
    }

    // SEND OPEN STATUS
    client.publish(MQTT_PANEL_OPEN_TOPIC, mesaj, false, 0);

    if (PS0[19] || PS0[20] || PS0[21] || PS0[22] || PS0[23] || PS0[24] || PS0[25] || PS0[26]) // report only if events exist
    {
        // PANEL 0 BYTE 23-26
        for (i = 0; i < 4; i++)
        {
            for (b = 0; b < 8; b++)
            {
                if (bitRead(PS0[19 + i], b))
                    doc.add("Zone " + String(i * 8 + b + 1));
            }
        }

        // PANEL 0 BYTE 27-28
        for (i = 0; i < 2; i++)
        {
            for (b = 0; b < 8; b++)
            {
                if (bitRead(PS0[23 + i], b))
                    doc.add("PGM " + String(i * 8 + b + 1));
            }
        }

        // PANEL 0 BYTE 29-30
        for (i = 0; i < 2; i++)
        {
            for (b = 0; b < 8; b++)
            {
                if (bitRead(PS0[25 + i], b))
                    doc.add("Bus Module " + String(i * 8 + b + 1));
            }
        }

        mesaj = "";
        serializeJson(doc, mesaj);
        doc.clear();
    }
    else
    {
        mesaj = "none";
    }

    // SEND TAMPER STATUS
    client.publish(MQTT_PANEL_TAMPER_TOPIC, mesaj, false, 0);

    if (PS0[27] || PS0[28] || PS0[29] || PS0[30]) // report only if events exist
    {
        // PANEL 0 BYTE 31-34
        for (i = 0; i < 4; i++)
        {
            for (b = 0; b < 8; b++)
            {
                if (bitRead(PS0[27 + i], b))
                    doc.add("Zone " + String(i * 8 + b + 1));
            }
        }

        mesaj = "";
        serializeJson(doc, mesaj);
        doc.clear();
    }
    else
    {
        mesaj = "none";
    }

    // SEND FIRE STATUS
    client.publish(MQTT_PANEL_FIRE_TOPIC, mesaj, false, 0);

    // PANEL 0 BYTE 15
    doc["AC voltage"] = PS0[11];
    // PANEL 0 BYTE 16
    doc["DC voltage"] = PS0[12];
    // PANEL 0 BYTE 17
    doc["Battery voltage"] = PS0[13];
    // PANEL 0 BYTE 18
    doc["RF Noise floor"] = PS0[14];

    mesaj = "";
    serializeJson(doc, mesaj);
    doc.clear();

    client.publish(MQTT_PANEL_LEVELS_TOPIC, mesaj, false, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########     ###    ##    ## ######## ##             ##
// ##     ##   ## ##   ###   ## ##       ##           ####
// ##     ##  ##   ##  ####  ## ##       ##             ##
// ########  ##     ## ## ## ## ######   ##             ##
// ##        ######### ##  #### ##       ##             ##
// ##        ##     ## ##   ### ##       ##             ##
// ##        ##     ## ##    ## ######## ########     ######
//////////////////////////////////////////////////////////////////////////////////////////

void send_mqtt_panel1_data()
{
    panel_status_1_read = false;

    uint8_t i = 0;
    uint8_t b = 0;

    StaticJsonDocument<1024> doc;

    if (PS1[13] || PS1[14] || PS1[15] || PS1[16]) // report only if events exist
    {
        // PANEL 1 BYTE 17-20
        for (i = 0; i < 4; i++)
        {
            for (b = 0; b < 8; b++)
            {
                if (bitRead(PS1[13 + i], b))
                {
                    panel_1_partition_status_names(i, b); // loads mesaj with proper text
                    doc.add(mesaj);
                }
            }
        }

        mesaj = "";
        serializeJson(doc, mesaj);
        doc.clear();

        client.publish(MQTT_PANEL_PARTITION_TOPIC + String(1), mesaj, false, 0);
    }

    if (PS1[17] || PS1[18] || PS1[19] || PS1[20]) // report only if events exist
    {
        // PANEL 1 BYTE 21-24
        for (i = 0; i < 4; i++)
        {
            for (b = 0; b < 8; b++)
            {
                if (bitRead(PS1[17 + i], b))
                {
                    panel_1_partition_status_names(i, b); // loads mesaj with proper text
                    doc.add(mesaj);
                }
            }
        }

        mesaj = "";
        serializeJson(doc, mesaj);
        doc.clear();

        client.publish(MQTT_PANEL_PARTITION_TOPIC + String(2), mesaj, false, 0);
    }

    if (PS1[0] || PS1[1] || PS1[2] || PS1[3] || PS1[4] || PS1[5] || PS1[6] || PS1[7] || PS1[8] || PS1[25]) // report only if events exist
    {
        // PANEL 1 BYTE 4-7
        for (i = 0; i < 4; i++)
        {
            for (b = 0; b < 8; b++)
            {
                if (bitRead(PS1[i], b))
                    doc.add("Zone " + String(i * 8 + b + 1));
            }
        }

        // PANEL 1 BYTE 8-9
        for (i = 0; i < 2; i++)
        {
            for (b = 0; b < 8; b++)
            {
                if (bitRead(PS1[4 + i], b))
                    doc.add("PGM " + String(i * 8 + b + 1));
            }
        }

        // PANEL 1 BYTE 10-11
        for (i = 0; i < 2; i++)
        {
            for (b = 0; b < 8; b++)
            {
                if (bitRead(PS1[6 + i], b))
                    doc.add("Bus Module " + String(i * 8 + b + 1));
            }
        }

        // PANEL 1 BYTE 12
        for (b = 0; b < 8; b++)
        {
            if (bitRead(PS1[8], b))
                doc.add("Wireless Repeater/Keyboard " + String(b + 1));
        }

        // PANEL 1 BYTE 29
        for (b = 0; b < 8; b++)
        {
            if (bitRead(PS1[25], b))
                doc.add("Wireless Keypad " + String(b + 1));
        }

        mesaj = "";
        serializeJson(doc, mesaj);
        doc.clear();
    }
    else
    {
        mesaj = "none";
    }

    // REPORT RF TROUBLE MESSAGE
    client.publish(MQTT_PANEL_RF_TROUBLE_TOPIC, mesaj, false, 0);

    if (PS1[9] || PS1[10] || PS1[11] || PS1[12] || PS1[22] || PS1[24]) // report only if events exist
    {
        // PANEL 1 BYTE 13-16
        for (i = 0; i < 4; i++)
        {
            for (b = 0; b < 8; b++)
            {
                if (bitRead(PS1[9 + i], b))
                    doc.add("Zone " + String(i * 8 + b + 1));
            }
        }

        // PANEL 1 BYTE 26
        for (b = 0; b < 8; b++)
        {
            if (bitRead(PS1[22], b))
                doc.add("Wireless Repeater " + String(b + 1));
        }

        // PANEL 1 BYTE 28
        for (b = 0; b < 8; b++)
        {
            if (bitRead(PS1[24], b))
                doc.add("Wireless Keypad " + String(b + 1));
        }

        mesaj = "";
        serializeJson(doc, mesaj);
        doc.clear();
    }
    else
    {
        mesaj = "none";
    }

    // REPORT RF LOW BATTERY MESSAGE
    client.publish(MQTT_PANEL_RF_LOW_BATTERY_TOPIC, mesaj, false, 0);

    if (PS1[21] || PS1[23]) // report only if events exist
    {
        // PANEL 1 BYTE 25
        for (b = 0; b < 8; b++)
        {
            if (bitRead(PS1[21], b))
                doc.add("Wireless Repeater " + String(b + 1));
        }

        // PANEL 1 BYTE 27
        for (b = 0; b < 8; b++)
        {
            if (bitRead(PS1[23], b))
                doc.add("Wireless Keypad " + String(b + 1));
        }

        mesaj = "";
        serializeJson(doc, mesaj);
        doc.clear();
    }
    else
    {
        mesaj = "none";
    }
    client.publish(MQTT_PANEL_AC_LOST_TOPIC, mesaj, false, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########     ###    ##    ## ######## ##           #######
// ##     ##   ## ##   ###   ## ##       ##          ##     ##
// ##     ##  ##   ##  ####  ## ##       ##                 ##
// ########  ##     ## ## ## ## ######   ##           #######
// ##        ######### ##  #### ##       ##          ##
// ##        ##     ## ##   ### ##       ##          ##
// ##        ##     ## ##    ## ######## ########    #########
//////////////////////////////////////////////////////////////////////////////////////////

void send_mqtt_panel2_data()
{
    panel_status_2_read = false;

    uint8_t i = 0;
    uint8_t b = 0;

    boolean bit5 = false;
    boolean bit4 = false;

    StaticJsonDocument<1024> doc;

    for (i = 0; i < 32; i++)
    {
        if (PS2[i] > 0) // report only if status exists
        {
            b = 0;

            if (bitRead(PS2[i], 7))
                doc[b++] = "Zone was in Alarm";
            if (bitRead(PS2[i], 6))
                doc[b++] = "Zone is in Alarm";

            if (bitRead(PS2[i], 5))
                bit5 = true;
            if (bitRead(PS2[i], 4))
                bit4 = true;

            if (bit5 && bit4)
                doc[b++] = "In Fire Delay";
            if (!bit5 && bit4)
                doc[b++] = "In Entry Delay";
            if (bit5 && !bit4)
                doc[b++] = "In Intellizone Delay";
            if (!bit5 && !bit4)
                doc[b++] = "In No Delay";

            if (bitRead(PS2[i], 3))
                doc[b++] = "Zone Bypassed";
            if (bitRead(PS2[i], 2))
                doc[b++] = "Zone Shutdown";
            if (bitRead(PS2[i], 1))
                doc[b++] = "Zone is in TX Delay";
            if (bitRead(PS2[i], 0))
                doc[b++] = "Zone was Bypassed";

            mesaj = "";
            serializeJson(doc, mesaj);
            doc.clear();

            client.publish(MQTT_ZONE_TOPIC + String(i + 1) + "/status", mesaj, false, 0);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########     ###    ##    ## ######## ##           #######
// ##     ##   ## ##   ###   ## ##       ##          ##     ##
// ##     ##  ##   ##  ####  ## ##       ##                 ##
// ########  ##     ## ## ## ## ######   ##           #######
// ##        ######### ##  #### ##       ##                 ##
// ##        ##     ## ##   ### ##       ##          ##     ##
// ##        ##     ## ##    ## ######## ########     #######
//////////////////////////////////////////////////////////////////////////////////////////

void send_mqtt_panel3_data()
{
    panel_status_3_read = false;

    uint8_t i = 0;

    for (i = 0; i < 32; i++)
    {
        if (PS3[i] < 10) // report only if signal less than max
        {
            client.publish(MQTT_ZONE_TOPIC + String(i + 1) + "/signal", String(PS3[i]), false, 0);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########     ###    ##    ## ######## ##          ##
// ##     ##   ## ##   ###   ## ##       ##          ##    ##
// ##     ##  ##   ##  ####  ## ##       ##          ##    ##
// ########  ##     ## ## ## ## ######   ##          ##    ##
// ##        ######### ##  #### ##       ##          #########
// ##        ##     ## ##   ### ##       ##                ##
// ##        ##     ## ##    ## ######## ########          ##
//////////////////////////////////////////////////////////////////////////////////////////

void send_mqtt_panel4_data()
{
    panel_status_4_read = false;

    uint8_t i = 0;

    for (i = 0; i < 16; i++)
    {
        if (PS4[i] < 10) // report only if signal less than max
        {
            client.publish(MQTT_PGM_TOPIC + String(i + 1) + "/signal", String(PS4[i]), false, 0);
        }
    }

    for (i = 16; i < 18; i++)
    {
        if (PS4[i] < 10) // report only if signal less than max
        {
            client.publish(MQTT_WREP_TOPIC + String(i + 1) + "/signal", String(PS4[i]), false, 0);
        }
    }

    for (i = 18; i < 26; i++)
    {
        if (PS4[i] < 10) // report only if signal less than max
        {
            client.publish(MQTT_WKEY_TOPIC + String(i + 1) + "/signal", String(PS4[i]), false, 0);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########     ###    ##    ## ######## ##          ########
// ##     ##   ## ##   ###   ## ##       ##          ##
// ##     ##  ##   ##  ####  ## ##       ##          ##
// ########  ##     ## ## ## ## ######   ##          #######
// ##        ######### ##  #### ##       ##                ##
// ##        ##     ## ##   ### ##       ##          ##    ##
// ##        ##     ## ##    ## ######## ########     ######
//////////////////////////////////////////////////////////////////////////////////////////

void send_mqtt_panel5_data()
{
    panel_status_5_read = false;

    uint8_t i = 0;
    uint8_t b = 0;

    for (i = 0; i < 4; i++)
    {
        for (b = 0; b < 8; b++)
        {
            if (bitRead(PS5[i], b))
                client.publish(MQTT_ZONE_TOPIC + String(i * 8 + b + 1) + "/status", "Exit Delay", false, 0);
        }
    }
}
