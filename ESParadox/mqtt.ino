void mqtt_loop()
{
    if (panel_status_0_read)
        send_mqtt_panel0_data();
    if (panel_status_1_read)
        send_mqtt_panel1_data();

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

    client.subscribe(MQTT_SUB_TOPIC, 0);
    client.publish(MQTT_WILL_TOPIC, "1", true, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////////////

void send_to_mqtt()
{
    got_mqtt_data = false;
    client.publish(topic, mesaj, false, 0);
    // client.publish(MQTT_PUB_TOPIC, mesaj, false, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
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

    client.publish(MQTT_PANEL_HW_TOPIC, mesaj, true, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void send_mqtt_panel0_data()
{
    panel_status_0_read = false;

    uint8_t i = 0;
    uint8_t b = 0;

    StaticJsonDocument<1024> doc;

    JsonArray trbl = doc.createNestedArray("trouble");

    // PANEL 0 BYTE 4
    if (bitRead(PS0[0], 7))
        trbl.add("Timer loss");
    if (bitRead(PS0[0], 6))
        trbl.add("Global Fire-Loop Trouble");
    if (bitRead(PS0[0], 5))
        trbl.add("Global Module Tamper");
    if (bitRead(PS0[0], 4))
        trbl.add("Global Zone Tamper");
    if (bitRead(PS0[0], 3))
        trbl.add("Global Communication Trouble");
    if (bitRead(PS0[0], 2))
        trbl.add("Global Bell trouble");
    if (bitRead(PS0[0], 1))
        trbl.add("Global Power Trouble");
    if (bitRead(PS0[0], 0))
        trbl.add("Global RF Transmitter Low Battery");

    // PANEL 0 BYTE 5
    if (bitRead(PS0[1], 7))
        trbl.add("RF Interference");
    if (bitRead(PS0[1], 1))
        trbl.add("Global Module Supervision");
    if (bitRead(PS0[1], 0))
        trbl.add("Global Zone Supervision");

    // PANEL 0 BYTE 6
    if (bitRead(PS0[2], 6))
        trbl.add("Wireless Repeater Battery Failure");
    if (bitRead(PS0[2], 5))
        trbl.add("Wireless Repeater AC Loss");
    if (bitRead(PS0[2], 4))
        trbl.add("Wireless Keypad Battery Failure");
    if (bitRead(PS0[2], 3))
        trbl.add("Wireless Keypad AC Loss");
    if (bitRead(PS0[2], 2))
        trbl.add("Auxiliary Output Overload");
    if (bitRead(PS0[2], 1))
        trbl.add("AC Failure");
    if (bitRead(PS0[2], 0))
        trbl.add("No/Low Battery");

    // PANEL 0 BYTE 7
    if (bitRead(PS0[3], 1))
        trbl.add("Bell Output Overload");
    if (bitRead(PS0[3], 0))
        trbl.add("Bell Output Disconnect");

    // PANEL 0 BYTE 8
    if (bitRead(PS0[4], 5))
        trbl.add("Computer Fail to Communicate");
    if (bitRead(PS0[4], 4))
        trbl.add("Voice Fail to Communicate");
    if (bitRead(PS0[4], 3))
        trbl.add("Pager Fail to Communicate");
    if (bitRead(PS0[4], 2))
        trbl.add("Central 2 Reporting FTC");
    if (bitRead(PS0[4], 1))
        trbl.add("Central 1 Reporting FTC");
    if (bitRead(PS0[4], 0))
        trbl.add("Telephone Line Trouble");

    // PANEL 0 BYTE 15
    doc["AC voltage level"] = PS0[11];
    // PANEL 0 BYTE 16
    doc["DC voltage level"] = PS0[12];
    // PANEL 0 BYTE 17
    doc["Battery voltage level"] = PS0[13];
    // PANEL 0 BYTE 18
    doc["RF Noise floor level"] = PS0[14];

    // PANEL 0 BYTE 19-22
    JsonArray open = doc.createNestedArray("open");
    for (i = 0; i < 4; i++)
    {
        for (b = 0; b < 8; b++)
        {
            if (bitRead(PS0[15 + i], b))
                open.add("Zone " + String(i * 8 + b + 1));
        }
    }

    JsonArray tamp = doc.createNestedArray("tamper");
    // PANEL 0 BYTE 23-26
    for (i = 0; i < 4; i++)
    {
        for (b = 0; b < 8; b++)
        {
            if (bitRead(PS0[19 + i], b))
                tamp.add("Zone " + String(i * 8 + b + 1));
        }
    }

    // PANEL 0 BYTE 27-28
    for (i = 0; i < 4; i++)
    {
        for (b = 0; b < 8; b++)
        {
            if (bitRead(PS0[23 + i], b))
                tamp.add("PGM " + String(i * 8 + b + 1));
        }
    }

    // PANEL 0 BYTE 29-30
    for (i = 0; i < 4; i++)
    {
        for (b = 0; b < 8; b++)
        {
            if (bitRead(PS0[25 + i], b))
                tamp.add("Bus Module " + String(i * 8 + b + 1));
        }
    }

    JsonArray fire = doc.createNestedArray("Fire-Loop");
    // PANEL 0 BYTE 31-34
    for (i = 0; i < 4; i++)
    {
        for (b = 0; b < 8; b++)
        {
            if (bitRead(PS0[27 + i], b))
                fire.add("Zone " + String(i * 8 + b + 1));
        }
    }

    mesaj = "";
    serializeJson(doc, mesaj);
    doc.clear();

    client.publish(MQTT_PANEL_TOPIC, mesaj, false, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void send_mqtt_panel1_data()
{
    panel_status_1_read = false;
}
void send_mqtt_panel2_data()
{
    panel_status_2_read = false;
}
void send_mqtt_panel3_data()
{
    panel_status_3_read = false;
}
void send_mqtt_panel4_data()
{
    panel_status_4_read = false;
}
void send_mqtt_panel5_data()
{
    panel_status_5_read = false;
}