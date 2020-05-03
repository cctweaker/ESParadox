void check_data()
{
    uint8_t first_byte = (paradox_rx[0] & 0xF0);

    send_raw_to_mqtt();

    if (first_byte != 0xE0 && first_byte != 0x70 && first_byte != 0x50 && first_byte != 0x40 && first_byte != 0x30)
    {
        // data not usable
        got_data = false;
        flush_serial_buffer();
        return;
    }

    if (!check_checksum())
    {
        // data not usable
        got_data = false;
        flush_serial_buffer();
        return;
    }

    got_paradox_data = true;

    if (first_byte == 0xE0)
    {
        if (paradox_rx[7] == 0x30 && paradox_rx[8] == 3)
        {
            panel_connected = false;
            got_paradox_data = false; // data dealt with
        }
        else if (paradox_rx[7] == 0x30 && paradox_rx[8] == 2)
        {
            panel_connected = true;
            got_paradox_data = false; // data dealt with
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void panel_login()
{
    // wait for current message to be received
    if (Serial.available() > 0)
        return;
    // wait for curret data to be checked
    if (got_data)
        return;
    // wait for current checked data to be transmitted to mqtt
    if (got_paradox_data)
        return;

    uint8_t i = 0;

    // STEP 1 of login
    clear_paradox_tx();
    paradox_tx[0] = 0x5F;
    paradox_tx[1] = 0x20;
    paradox_tx[33] = 0x05;

    send_data();

    i = 0;
    while (!read_data())
    {
        delay(25);
        i++;
        if (i > 200) // wait max 5sec.
            return;
    }

    // STEP 2 of login
    clear_paradox_tx();
    paradox_tx[4] = paradox_rx[4];
    paradox_tx[5] = paradox_rx[5];
    paradox_tx[6] = paradox_rx[6];
    paradox_tx[7] = paradox_rx[7];
    paradox_tx[8] = paradox_rx[8];
    paradox_tx[9] = paradox_rx[9];
    paradox_tx[13] = 0x55;
    // paradox_tx[14] = PCpassword1;
    // paradox_tx[15] = PCpassword2;
    paradox_tx[14] = Mpassword1;
    paradox_tx[15] = Mpassword2;
    paradox_tx[33] = 0x05;

    send_data();

    i = 0;
    while (!read_data())
    {
        delay(25);
        i++;
        if (i > 200) // wait max 5sec.
            return;
    }

    if ((paradox_rx[0] & 0xF0) == 0x10)
    {
        panel_connected = true;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void panel_status()
{
    // wait for current message to be received
    if (Serial.available() > 0)
        return;
    // wait for curret data to be checked
    if (got_data)
        return;
    // wait for current checked data to be transmitted to mqtt
    if (got_paradox_data)
        return;
    // return if panel not connected
    if (!panel_connected)
        return;

    uint8_t i = 0;

    clear_paradox_tx();
    paradox_tx[0] = 0x50;
    paradox_tx[2] = 0x80;
    paradox_tx[33] = 0x05;

    send_data();

    i = 0;
    while (!read_data())
    {
        delay(25);
        i++;
        if (i > 200) // wait max 5sec.
            return;
    }

    timer_loss = bitRead(paradox_rx[4], 7);
    power_trouble = bitRead(paradox_rx[4], 1);
    ac_failure = bitRead(paradox_rx[6], 1);
    low_battery = bitRead(paradox_rx[6], 0);
    telephone_trouble = bitRead(paradox_rx[8], 0);
    ac_voltage = paradox_rx[15];
    battery_voltage = paradox_rx[16];
    dc_voltage = paradox_rx[17];

    // some zone data can be read here, bytes 19-22

    paradox_tx[3] = 0x01;
    send_data();

    i = 0;
    while (!read_data())
    {
        delay(25);
        i++;
        if (i > 200) // wait max 5sec.
            return;
    }

    fire = bitRead(paradox_rx[17], 7);
    audible = bitRead(paradox_rx[17], 6);
    silent = bitRead(paradox_rx[17], 5);
    alarm = bitRead(paradox_rx[17], 4);
    stay = bitRead(paradox_rx[17], 2);
    sleep = bitRead(paradox_rx[17], 1);
    arm = bitRead(paradox_rx[17], 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void panel_command()
{
    // wait for current message to be received
    if (Serial.available() > 0)
        return;
    // wait for curret data to be checked
    if (got_data)
        return;
    // wait for current checked data to be transmitted to mqtt
    if (got_paradox_data)
        return;
    // return if panel not connected
    if (!panel_connected)
        return;
    // return if no panel command
    if (!command)
        return;

    uint8_t i = 0;

    clear_paradox_tx();
    paradox_tx[0] = 0x40;
    paradox_tx[2] = command;
    paradox_tx[3] = subcommand;
    paradox_tx[33] = 0x05;

    send_data();

    command = 0;
    subcommand = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void panel_set_time()
{
    // wait for current message to be received
    if (Serial.available() > 0)
        return;
    // wait for curret data to be checked
    if (got_data)
        return;
    // wait for current checked data to be transmitted to mqtt
    if (got_paradox_data)
        return;
    // return if panel not connected
    if (!panel_connected)
        return;
    // return if no panel command
    if (!command)
        return;

    uint8_t i = 0;

    struct tm *timeinfo;

    uint8_t year = timeinfo->tm_year - 100;

    clear_paradox_tx();
    paradox_tx[0] = 0x30;
    paradox_tx[4] = 20;
    paradox_tx[5] = year;
    paradox_tx[6] = timeinfo->tm_mon;
    paradox_tx[7] = timeinfo->tm_mday;
    paradox_tx[8] = timeinfo->tm_hour;
    paradox_tx[9] = timeinfo->tm_min;
    paradox_tx[33] = 0x05;

    send_data();

    set_time = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void panel_disconnect()
{
    // wait for current message to be received
    if (Serial.available() > 0)
        return;
    // wait for curret data to be checked
    if (got_data)
        return;
    // wait for current checked data to be transmitted to mqtt
    if (got_paradox_data)
        return;
    // return if panel not connected
    if (!panel_connected)
        return;

    uint8_t i = 0;

    clear_paradox_tx();
    paradox_tx[0] = 0x70;
    paradox_tx[2] = 0x05;
    paradox_tx[33] = 0x05;

    send_data();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void send_data()
{
    uint16_t checksum = 0;
    uint8_t i = 0;

    for (i = 0; i < paradox_checksum_length; i++)
    {
        checksum += paradox_tx[i];
    }
    paradox_tx[36] = checksum;

    Serial.write(paradox_tx, paradox_message_length);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

boolean read_data()
{
    if (Serial.available() < paradox_message_length)
        return false;

    for (uint8_t i = 0; i < paradox_message_length; i++)
    {
        paradox_rx[i] = Serial.read();
        yield();
    }

    if (DEBUG)
        send_raw_to_mqtt();

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void clear_paradox_tx()
{
    for (uint8_t i = 0; i < paradox_message_length; i++)
    {
        paradox_tx[i] = 0x00;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

boolean check_checksum()
{
    uint16_t checksum = 0;
    uint8_t calc = 0;
    uint8_t i = 0;

    for (i = 0; i < paradox_checksum_length; i++)
    {
        checksum += paradox_rx[i];
    }
    calc = checksum;

    if (calc == paradox_rx[36])
        return true;

    return false;
}
