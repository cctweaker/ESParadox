//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// #### ##    ## #### ########    ##     ##  #######  ######## ########
//  ##  ###   ##  ##     ##       ###   ### ##     ##    ##       ##
//  ##  ####  ##  ##     ##       #### #### ##     ##    ##       ##
//  ##  ## ## ##  ##     ##       ## ### ## ##     ##    ##       ##
//  ##  ##  ####  ##     ##       ##     ## ##  ## ##    ##       ##
//  ##  ##   ###  ##     ##       ##     ## ##    ##     ##       ##
// #### ##    ## ####    ##       ##     ##  ##### ##    ##       ##

void init_mqtt()
{
    if (!start_mqtt || !use_wifi)
        return;

    char topic[128];
    sprintf(topic, "%s%s%s%s%s", LOC, TIP, NAME, XTRA, WILL);

    client.begin(MQTT_HOST, MQTT_PORT, net);
    client.setWill(topic, "0", true, 0);
    client.onMessage(messageReceived);

    connect_mqtt();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######   #######  ##    ## ##    ## ########  ######  ########    ##     ##  #######  ######## ########
// ##    ## ##     ## ###   ## ###   ## ##       ##    ##    ##       ###   ### ##     ##    ##       ##
// ##       ##     ## ####  ## ####  ## ##       ##          ##       #### #### ##     ##    ##       ##
// ##       ##     ## ## ## ## ## ## ## ######   ##          ##       ## ### ## ##     ##    ##       ##
// ##       ##     ## ##  #### ##  #### ##       ##          ##       ##     ## ##  ## ##    ##       ##
// ##    ## ##     ## ##   ### ##   ### ##       ##    ##    ##       ##     ## ##    ##     ##       ##
//  ######   #######  ##    ## ##    ## ########  ######     ##       ##     ##  ##### ##    ##       ##

void connect_mqtt()
{
    uint8_t i = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        i++;
        if (i > 200)
            ESP.restart();
    }

#ifdef USE_SSL
    BearSSL::X509List cert(digicert);
    net.setTrustAnchors(&cert);
#endif

    i = 0;
    while (!client.connect(NAME, MQTT_USER, MQTT_PASS))
    {
        delay(100);
        i++;
        if (i > 200)
        {
            use_mqtt = false;
            return;
        }
    }

    char topic[128];

    sprintf(topic, "%s%s%s%s%s", LOC, TIP, NAME, XTRA, SUB);
    client.subscribe(topic, 0);

    sprintf(topic, "%s%s%s%s%s", LOC, TIP, NAME, XTRA, WILL);
    client.publish(topic, "1", true, 0);

    mqtt_heartbeat();

    use_mqtt = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##        #######   #######  ########
// ##       ##     ## ##     ## ##     ##
// ##       ##     ## ##     ## ##     ##
// ##       ##     ## ##     ## ########
// ##       ##     ## ##     ## ##
// ##       ##     ## ##     ## ##
// ########  #######   #######  ##

void mqtt_loop()
{
    mqtt_send_SIA();

    if (pdx_panel_data_login)
        mqtt_send_panel_info();

    if (pdx_got_panel_data)
        mqtt_send_panel_data();

    if (pdx_got_event)
        mqtt_send_event_data();

    if (heartbeat)
        if ((unsigned long)(millis() - last_heartbeat) > (heartbeat_minutes * 60 * 1000))
        {
            last_heartbeat = millis();
            mqtt_heartbeat();
        }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  ####    ###
// ##    ##  ##    ## ##
// ##        ##   ##   ##
//  ######   ##  ##     ##
//       ##  ##  #########
// ##    ##  ##  ##     ##
//  ######  #### ##     ##

void mqtt_send_SIA()
{
    if (!SIA && mqtt_SIA_sent)
    {
        mqtt_SIA_sent = false;
        char topic[128];
        sprintf(topic, "%s%s%s%s%s/system in alarm", LOC, TIP, NAME, XTRA, PUB);
        client.publish(topic, "0", true, 0);
    }

    if (SIA && !mqtt_SIA_sent)
    {
        mqtt_SIA_sent = true;
        char topic[128];
        sprintf(topic, "%s%s%s%s%s/system in alarm", LOC, TIP, NAME, XTRA, PUB);
        client.publish(topic, "1", true, 0);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########     ###    ##    ## ######## ##          #### ##    ## ########  #######
// ##     ##   ## ##   ###   ## ##       ##           ##  ###   ## ##       ##     ##
// ##     ##  ##   ##  ####  ## ##       ##           ##  ####  ## ##       ##     ##
// ########  ##     ## ## ## ## ######   ##           ##  ## ## ## ######   ##     ##
// ##        ######### ##  #### ##       ##           ##  ##  #### ##       ##     ##
// ##        ##     ## ##   ### ##       ##           ##  ##   ### ##       ##     ##
// ##        ##     ## ##    ## ######## ########    #### ##    ## ##        #######

void mqtt_send_panel_info()
{
    pdx_panel_data_login = false;

    char topic[128];

    translate_panel_info();

    if (strlen(mqtt_tx))
    {
        sprintf(topic, "%s%s%s%s%s/panel/info", LOC, TIP, NAME, XTRA, PUB);
        client.publish(topic, mqtt_tx, true, 0);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########     ###    ##    ## ######## ##          ########     ###    ########    ###
// ##     ##   ## ##   ###   ## ##       ##          ##     ##   ## ##      ##      ## ##
// ##     ##  ##   ##  ####  ## ##       ##          ##     ##  ##   ##     ##     ##   ##
// ########  ##     ## ## ## ## ######   ##          ##     ## ##     ##    ##    ##     ##
// ##        ######### ##  #### ##       ##          ##     ## #########    ##    #########
// ##        ##     ## ##   ### ##       ##          ##     ## ##     ##    ##    ##     ##
// ##        ##     ## ##    ## ######## ########    ########  ##     ##    ##    ##     ##

void mqtt_send_panel_data()
{
    pdx_got_panel_data = false;

    switch (pdx_rx_buffer[3])
    {
    case 0x00:
        mqtt_send_panel_0_data();
        break;

    case 0x01:
        mqtt_send_panel_1_data();
        break;

    case 0x02:
        mqtt_send_panel_2_data();
        break;

    case 0x03:
        mqtt_send_panel_3_data();
        break;

    case 0x04:
        mqtt_send_panel_4_data();
        break;

    case 0x05:
        mqtt_send_panel_5_data();
        break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########     ###    ##    ## ######## ##            #####
// ##     ##   ## ##   ###   ## ##       ##           ##   ##
// ##     ##  ##   ##  ####  ## ##       ##          ##     ##
// ########  ##     ## ## ## ## ######   ##          ##     ##
// ##        ######### ##  #### ##       ##          ##     ##
// ##        ##     ## ##   ### ##       ##           ##   ##
// ##        ##     ## ##    ## ######## ########      #####

void mqtt_send_panel_0_data()
{
    char topic[128];

    translate_panel_0_trouble();
    sprintf(topic, "%s%s%s%s%s/trouble", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_panel_ids(19, 4); // array of open zones byte 19
    sprintf(topic, "%s%s%s%s%s/zone/open", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_panel_ids(23, 4); // array of tampered zones byte 23
    sprintf(topic, "%s%s%s%s%s/zone/tamper", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_panel_ids(27, 2); // array of tampered zones byte 23
    sprintf(topic, "%s%s%s%s%s/pgm/tamper", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_panel_ids(29, 2); // array of tampered zones byte 23
    sprintf(topic, "%s%s%s%s%s/module/tamper", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_panel_ids(31, 4); // array of open zones byte 19
    sprintf(topic, "%s%s%s%s%s/zone/fire", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_panel_0_values();
    sprintf(topic, "%s%s%s%s%s/panel/values", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########     ###    ##    ## ######## ##             ##
// ##     ##   ## ##   ###   ## ##       ##           ####
// ##     ##  ##   ##  ####  ## ##       ##             ##
// ########  ##     ## ## ## ## ######   ##             ##
// ##        ######### ##  #### ##       ##             ##
// ##        ##     ## ##   ### ##       ##             ##
// ##        ##     ## ##    ## ######## ########     ######

void mqtt_send_panel_1_data()
{
    char topic[128];

    translate_panel_ids(4, 4);
    sprintf(topic, "%s%s%s%s%s/zone/RF trouble", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_panel_ids(8, 2);
    sprintf(topic, "%s%s%s%s%s/pgm/RF trouble", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_panel_ids(10, 2);
    sprintf(topic, "%s%s%s%s%s/module/supervision", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_panel_repeater_keypad(12, true);
    sprintf(topic, "%s%s%s%s%s/repeater/supervision", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_panel_repeater_keypad(12, false);
    sprintf(topic, "%s%s%s%s%s/keypad/supervision", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_panel_ids(13, 4);
    sprintf(topic, "%s%s%s%s%s/zone/low battery", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_partition_status(17);
    sprintf(topic, "%s%s%s%s%s/partition/1/status", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_partition_status(21);
    sprintf(topic, "%s%s%s%s%s/partition/2/status", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_panel_ids(25, 1);
    sprintf(topic, "%s%s%s%s%s/repeater/AC lost", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_panel_ids(26, 1);
    sprintf(topic, "%s%s%s%s%s/repeater/battery failure", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_panel_ids(27, 1);
    sprintf(topic, "%s%s%s%s%s/keypad/AC lost", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);

    translate_panel_ids(28, 1);
    sprintf(topic, "%s%s%s%s%s/keypad/battery failure", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);
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

void mqtt_send_panel_2_data()
{
    char topic[128];
    uint8_t i = 0;

    for (i = 1; i < 33; i++)
    {
        translate_zone_status(i + 3);
        sprintf(topic, "%s%s%s%s%s/zone/%d/status", LOC, TIP, NAME, XTRA, PUB, i);
        client.publish(topic, mqtt_tx, true, 0);
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

void mqtt_send_panel_3_data()
{
    char topic[128];
    uint8_t i = 0;

    for (i = 1; i < 33; i++)
    {
        sprintf(topic, "%s%s%s%s%s/zone/%d/signal", LOC, TIP, NAME, XTRA, PUB, i);
        client.publish(topic, String(pdx_rx_buffer[i + 3]), true, 0);
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

void mqtt_send_panel_4_data()
{
    char topic[128];
    uint8_t i = 0;

    for (i = 1; i < 17; i++)
    {
        sprintf(topic, "%s%s%s%s%s/pgm/%d/signal", LOC, TIP, NAME, XTRA, PUB, i);
        client.publish(topic, String(pdx_rx_buffer[i + 3]), true, 0);
    }

    for (i = 1; i < 3; i++)
    {
        sprintf(topic, "%s%s%s%s%s/repeater/%d/signal", LOC, TIP, NAME, XTRA, PUB, i);
        client.publish(topic, String(pdx_rx_buffer[i + 19]), true, 0);
    }

    for (i = 1; i < 3; i++)
    {
        sprintf(topic, "%s%s%s%s%s/keypad/%d/signal", LOC, TIP, NAME, XTRA, PUB, i);
        client.publish(topic, String(pdx_rx_buffer[i + 21]), true, 0);
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

void mqtt_send_panel_5_data()
{
    char topic[128];
    uint8_t i = 0;
    uint8_t b = 0;

    for (i = 0; i < 4; i++)
    {
        for (b = 0; b < 8; b++)
        {
            sprintf(topic, "%s%s%s%s%s/zone/%d/exit delay", LOC, TIP, NAME, XTRA, PUB, i * 8 + b + 1);
            client.publish(topic, String(bitRead(pdx_rx_buffer[i + 4], b)), true, 0);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##     ## ########    ###    ########  ######## ########  ########    ###    ########
// ##     ## ##         ## ##   ##     ##    ##    ##     ## ##         ## ##      ##
// ##     ## ##        ##   ##  ##     ##    ##    ##     ## ##        ##   ##     ##
// ######### ######   ##     ## ########     ##    ########  ######   ##     ##    ##
// ##     ## ##       ######### ##   ##      ##    ##     ## ##       #########    ##
// ##     ## ##       ##     ## ##    ##     ##    ##     ## ##       ##     ##    ##
// ##     ## ######## ##     ## ##     ##    ##    ########  ######## ##     ##    ##

void mqtt_heartbeat()
{
    char topic[128];

    DynamicJsonDocument doc(256);

    doc["PDX conn"] = pdx_panel_connection > 3 ? true : false;
    doc["C lvl"] = pdx_panel_connection;
#ifndef USE_SSL
    doc["fw"] = FW_NAME;
#else
    doc["fw"] = String(FW_NAME) + "_SSL";
#endif
    doc["ver"] = VERSION;
    doc["ID"] = String(ESP.getChipId(), HEX);
    doc["vcc"] = ESP.getVcc();
    doc["SSID"] = WiFi.SSID();
    doc["RSSI"] = WiFi.RSSI();
    doc["BSSID"] = WiFi.BSSIDstr();
    doc["IP"] = WiFi.localIP().toString();

    serializeJson(doc, mqtt_tx);
    doc.clear();
    sprintf(topic, "%s%s%s%s%s", LOC, TIP, NAME, XTRA, STAT);
    client.publish(topic, mqtt_tx, true, 0);

    doc["free heap"] = ESP.getFreeHeap();
    doc["heap frag"] = ESP.getHeapFragmentation();
    doc["max block"] = ESP.getMaxFreeBlockSize();
    doc["core v"] = ESP.getCoreVersion();
    doc["sdk v"] = ESP.getSdkVersion();
    doc["sketch"] = ESP.getSketchSize();
    doc["sketch free"] = ESP.getFreeSketchSpace();
    doc["MD5"] = ESP.getSketchMD5();
    doc["crc"] = ESP.checkFlashCRC();

    serializeJson(doc, mqtt_tx);
    doc.clear();
    sprintf(topic, "%s%s%s%s%s/1", LOC, TIP, NAME, XTRA, STAT);
    client.publish(topic, mqtt_tx, true, 0);

    doc["CPU freq"] = ESP.getCpuFreqMHz();
    doc["flash freq"] = ESP.getFlashChipSpeed() / 1000000;
    doc["flash ID"] = String(ESP.getFlashChipId(), HEX);
    doc["flash size"] = ESP.getFlashChipSize();
    doc["real flash size"] = ESP.getFlashChipRealSize();
    doc["CPU cycles"] = ESP.getCycleCount();

    serializeJson(doc, mqtt_tx);
    doc.clear();
    sprintf(topic, "%s%s%s%s%s/2", LOC, TIP, NAME, XTRA, STAT);
    client.publish(topic, mqtt_tx, true, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ########  ######  ######## #### ##     ## ######## ########
// ##     ## ##       ##    ## ##        ##  ##     ## ##       ##     ##
// ##     ## ##       ##       ##        ##  ##     ## ##       ##     ##
// ########  ######   ##       ######    ##  ##     ## ######   ##     ##
// ##   ##   ##       ##       ##        ##   ##   ##  ##       ##     ##
// ##    ##  ##       ##    ## ##        ##    ## ##   ##       ##     ##
// ##     ## ########  ######  ######## ####    ###    ######## ########

void messageReceived(String &topic, String &payload)
{

    uint8_t idx = topic.lastIndexOf('/') + 1;
    String cmnd = topic.substring(idx);

    if (cmnd == "AD")
    {
        command = 0x05 - payload.toInt();
        subcommand = 0;
        return;
    }

    if (cmnd == "arm")
    {
        command = 0x04;
        subcommand = 0;
        return;
    }

    if (cmnd == "disarm")
    {
        command = 0x05;
        subcommand = 0;
        return;
    }

    if (cmnd == "stay_arm")
    {
        command = 0x01;
        subcommand = 0;
        return;
    }

    if (cmnd == "sleep_arm")
    {
        command = 0x03;
        subcommand = 0;
        return;
    }

    if (cmnd == "stay_arm_d")
    {
        command = 0x06;
        subcommand = 0;
        return;
    }

    if (cmnd == "sleep_arm_d")
    {
        command = 0x07;
        subcommand = 0;
        return;
    }

    if (cmnd == "disarm_both")
    {
        command = 0x08;
        subcommand = 0;
        return;
    }

    if (cmnd == "bypass")
    {
        command = 0x10;
        subcommand = payload.toInt();
        return;
    }

    if (cmnd == "pgm_on_ow")
    {
        command = 0x30;
        subcommand = payload.toInt();
        return;
    }

    if (cmnd == "pgm_off_ow")
    {
        command = 0x31;
        subcommand = payload.toInt();
        return;
    }

    if (cmnd == "pgm_on")
    {
        command = 0x32;
        subcommand = payload.toInt();
        return;
    }

    if (cmnd == "pgm_off")
    {
        command = 0x33;
        subcommand = payload.toInt();
        return;
    }

    DynamicJsonDocument doc(256);

    deserializeJson(doc, payload);
    yield();

    if (doc.containsKey("cmd"))
        command = doc["cmd"];

    if (doc.containsKey("scmd"))
        subcommand = doc["scmd"];

    if (doc.containsKey("time"))
    {
        uint16_t year = doc["year"];
        uint8_t month = doc["month"];
        uint8_t day = doc["day"];
        uint8_t hour = doc["hour"];
        uint8_t minute = doc["minute"];

        panel_set_time(year, month, day, hour, minute);
    }

    if (doc.containsKey("UserID"))
        UserID = strtoul(doc["UserID"], 0, 16);

    if (doc.containsKey("UserPASS"))
        UserPASS = strtoul(doc["UserPASS"], 0, 16);

    if (doc.containsKey("update"))
        do_ota_update = true;

    if (doc.containsKey("reset"))
        ESP.restart();

    if (doc.containsKey("disconnect"))
    {
        pdx_panel_disconnect();
        pdx_panel_connection = 0;
    }

    doc.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ######## ##     ## ######## ##    ## ########
// ##       ##     ## ##       ###   ##    ##
// ##       ##     ## ##       ####  ##    ##
// ######   ##     ## ######   ## ## ##    ##
// ##        ##   ##  ##       ##  ####    ##
// ##         ## ##   ##       ##   ###    ##
// ########    ###    ######## ##    ##    ##
void mqtt_send_event_data()
{
    pdx_got_event = false;
    translate_event();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////