void paradox_loop()
{
    if (Serial.available() >= paradox_message_length)
    {
        // read_data returns true if paradox_rx is filled
        got_data = read_data();
        return;
    }

    if (got_paradox_data)
    {
        send_to_mqtt();
        return;
    }

    if (got_data)
    {
        check_data();
        return;
    }

    if (!panel_connected)
    {
        panel_login();
        return;
    }

    if (command)
    {
        panel_command();
        return;
    }

    if (panel_set_date_time)
    {
        panel_set_time();
        return;
    }

    // if we need a valid panel data
    if (panel_status_request < 6)
    {
        panel_get_status();
        return;
    }

    if ((unsigned long)(millis() - pdlh) > PDHP)
    {
        pdlh = millis();
        panel_status_request = 0;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void check_data()
{
    uint8_t first_byte = (paradox_rx[0] & 0xF0); // command portion of command byte

    if (first_byte != 0xE0 && first_byte != 0x90 && first_byte != 0x80 && first_byte != 0x70 && first_byte != 0x60 && first_byte != 0x50 && first_byte != 0x40 && first_byte != 0x30 && first_byte != 0x00)
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

    // read LSB from command byte
    SIA = bitRead(paradox_rx[0], 2);
    WLC = bitRead(paradox_rx[0], 1);
    WDC = bitRead(paradox_rx[0], 0);

    switch (first_byte)
    {
    case 0x00: // panel answer to PC request
        PPI = paradox_rx[4];
        PFV = paradox_rx[5];
        PFR = paradox_rx[6];
        PFB = paradox_rx[7];
        PPID1 = paradox_rx[8];
        PPID2 = paradox_rx[9];
        if ((PPI & 0xF0) == 0x40)
        {
            TFB = paradox_rx[15];
            TF = paradox_rx[16];
            TFV = paradox_rx[17];
            TFR = paradox_rx[18];
            TNFL = paradox_rx[19];
            THR = paradox_rx[21];
            NFLTH = bitRead(paradox_rx[20], 1);
            CCD = bitRead(paradox_rx[20], 0);
        }
        panel_start_communication_response = true;
        break;

    case 0x10:
        if (NEWARE)
        {
            if (paradox_rx[1] == 0x25 && paradox_rx[2] == 0x10)
            {
                partition_rights_access_1 = bitRead(paradox_rx[4], 1);
                partition_rights_access_2 = bitRead(paradox_rx[4], 0);
                panel_start_communication = false;
                panel_start_communication_response = false;
                panel_initialize = false;
                panel_connected = true;
            }
        }
        else
        {
            // Winload
            panel_start_communication = false;
            panel_start_communication_response = false;
            panel_initialize = false;
            panel_connected = true;
        }
        break;

    case 0x30:
        // time set ok
        break;

    case 0x40:
        // paradox_rx[2] returns executed action
        break;

    case 0x50:
        if (paradox_rx[2] == 0x80) // it's a panel status response
        {
            switch (paradox_rx[3])
            {
            case 0x00: // Panel Status 0
                for (uint8_t i = 0; i < PS0len; i++)
                {
                    PS0[i] = paradox_rx[4 + i];
                }
                panel_status_0_read = true;
                break;

            case 0x01: // Panel Status 1
                for (uint8_t i = 0; i < PS1len; i++)
                {
                    PS1[i] = paradox_rx[4 + i];
                }
                panel_status_1_read = true;
                break;

            case 0x02: // Panel Status 2
                for (uint8_t i = 0; i < PS2len; i++)
                {
                    PS2[i] = paradox_rx[4 + i];
                }
                panel_status_2_read = true;
                break;

            case 0x03: // Panel Status 3
                for (uint8_t i = 0; i < PS3len; i++)
                {
                    PS3[i] = paradox_rx[4 + i];
                }
                panel_status_3_read = true;
                break;

            case 0x04: // Panel Status 4
                for (uint8_t i = 0; i < PS4len; i++)
                {
                    PS4[i] = paradox_rx[4 + i];
                }
                panel_status_4_read = true;
                break;

            case 0x05: // Panel Status 5
                for (uint8_t i = 0; i < PS5len; i++)
                {
                    PS5[i] = paradox_rx[4 + i];
                }
                panel_status_5_read = true;
                break;
            }
        }
        break;

    case 0x70:
        // error command
        panel_start_communication = false;
        panel_start_communication_response = false;
        panel_initialize = false;
        panel_connected = false;
        break;
    }

    // got_paradox_data = true;

    // if (first_byte == 0xE0)
    // {
    //     if (paradox_rx[7] == 0x30 && paradox_rx[8] == 3)
    //     {
    //         panel_connected = false;
    //         got_paradox_data = false; // data dealt with
    //     }
    //     else if (paradox_rx[7] == 0x30 && paradox_rx[8] == 2)
    //     {
    //         panel_connected = true;
    //         got_paradox_data = false; // data dealt with
    //     }
    // }
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

    // if it is the first step in establishing communications
    if (!panel_start_communication)
    {
        panel_start_communication = true;

        clear_paradox_tx(); // all bytes to 0

        // STEP 1 of login, start communication command
        paradox_tx[0] = 0x5F;
        paradox_tx[1] = 0x20;
        paradox_tx[33] = SourceID;
        paradox_tx[34] = UserIDh;
        paradox_tx[35] = UserIDl;

        send_data();
        return;
    }

    // if we got response from panel and not sent the initialize communication command yet
    if (panel_start_communication_response && !panel_initialize)
    {
        panel_initialize = true;

        clear_paradox_tx(); // all bytes to 0

        // STEP 2 of login, initialize communication command
        paradox_tx[0] = 0x00;
        paradox_tx[4] = PPI;
        paradox_tx[5] = PFV;
        paradox_tx[6] = PFR;
        paradox_tx[7] = PFB;

        if (WINLOAD)
        {
            paradox_tx[8] = PPID1;
            paradox_tx[9] = PPID2;
            paradox_tx[10] = PCpassh;
            paradox_tx[11] = PCpassl;
            paradox_tx[13] = 0x00;
        }

        if (NEWARE)
        {
            paradox_tx[13] = 0x55;
            paradox_tx[14] = UpassD1;
            paradox_tx[15] = UpassD2;
            paradox_tx[16] = UpassD3;
        }

        paradox_tx[33] = SourceID;
        paradox_tx[34] = UserIDh;
        paradox_tx[35] = UserIDl;

        send_data();
        return;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void panel_get_status()
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
    // return if panel not 0-5
    if (panel_status_request > 5)
        return;

    clear_paradox_tx();

    paradox_tx[0] = 0x50;
    paradox_tx[2] = 0x80; // validation to distinguish from EEprom read
    paradox_tx[3] = panel_status_request;
    paradox_tx[33] = SourceID;
    paradox_tx[34] = UserIDh;
    paradox_tx[35] = UserIDl;

    panel_status_request++;

    send_data();

    return;
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
    paradox_tx[33] = SourceID;
    paradox_tx[34] = UserIDh;
    paradox_tx[35] = UserIDl;

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
    paradox_tx[33] = SourceID;
    paradox_tx[34] = UserIDh;
    paradox_tx[35] = UserIDl;

    send_data();

    panel_set_date_time = false;
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

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////