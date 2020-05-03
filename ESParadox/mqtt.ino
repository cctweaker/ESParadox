void mqtt_setup()
{
    client.begin(MQTT_HOST, MQTT_PORT, net);
    client.setWill(MQTT_WILL_TOPIC, "0", true, 0);
    client.onMessage(messageReceived);
    mqtt_connect();
}

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

void messageReceived(String &topic, String &payload)
{
    StaticJsonDocument<256> doc;
    deserializeJson(doc, payload);
    yield();

    command = doc["cmd"];
    subcommand = doc["scmd"];
    set_time = doc["time"];

    doc.clear();
}

void send_to_mqtt()
{
    StaticJsonDocument<1024> doc;

    uint8_t i = 0;

    String label = "";

    for (i = 15; i <= 30; i++)
    {
        if (paradox_rx[i] > 0)
        {
            label = label + char(paradox_rx[i]);
        }
        else
        {
            label = label + " ";
        }
    }

    doc["g"] = paradox_rx[7];
    doc["s"] = paradox_rx[8];
    doc["p"] = paradox_rx[9];
    doc["l"] = label;
    // doc["lt"] = paradox_rx[14];

    String mesaj;
    serializeJson(doc, mesaj);
    doc.clear();

    client.publish(MQTT_PUB_TOPIC, mesaj, false, 0);

    got_paradox_data = false;
    got_data = false;
}

void send_raw_to_mqtt()
{
    if (!DEBUG)
        return;

    uint8_t i = 0;
    String raw = "";

    for (i = 0; i < paradox_message_length; i++)
    {
        raw += String(paradox_rx[i], HEX);
        raw += " ";
    }

    client.publish(MQTT_RAW_TOPIC, raw, false, 0);
}

void send_panel_status_to_mqtt()
{
    StaticJsonDocument<1024> doc;

    if (timer_loss)
        doc["trouble"]["timer"] = "Timer loss";
    if (power_trouble)
        doc["trouble"]["power"] = "Power trouble";
    if (ac_failure)
        doc["trouble"]["ac"] = "AC failure";
    if (low_battery)
        doc["trouble"]["battery"] = "Low battery";
    if (telephone_trouble)
        doc["trouble"]["tel"] = "Telephone trouble";
    doc["acv"] = ac_voltage;
    doc["dcv"] = dc_voltage;
    doc["bat"] = battery_voltage;

    if (fire)
        doc["alarm"] = "Fire";
    if (audible)
        doc["alarm"] = "Audible";
    if (silent)
        doc["alarm"] = "Silent";
    if (alarm)
        doc["alarm"] = "Alarm";
    if (stay)
        doc["alarm"] = "Stay";
    if (sleep)
        doc["alarm"] = "Sleep";
    if (arm)
        doc["alarm"] = "Arm";

    String mesaj;
    serializeJson(doc, mesaj);
    doc.clear();

    client.publish(MQTT_PANEL_TOPIC, mesaj, false, 0);
}