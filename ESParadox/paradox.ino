//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// #### ##    ## #### ########    ########     ###    ########     ###    ########   #######  ##     ##
//  ##  ###   ##  ##     ##       ##     ##   ## ##   ##     ##   ## ##   ##     ## ##     ##  ##   ##
//  ##  ####  ##  ##     ##       ##     ##  ##   ##  ##     ##  ##   ##  ##     ## ##     ##   ## ##
//  ##  ## ## ##  ##     ##       ########  ##     ## ########  ##     ## ##     ## ##     ##    ###
//  ##  ##  ####  ##     ##       ##        ######### ##   ##   ######### ##     ## ##     ##   ## ##
//  ##  ##   ###  ##     ##       ##        ##     ## ##    ##  ##     ## ##     ## ##     ##  ##   ##
// #### ##    ## ####    ##       ##        ##     ## ##     ## ##     ## ########   #######  ##     ##

void init_paradox()
{
    pdx_panel_disconnect();    // make sure paradox panel connection is closed
    pdx_flush_serial_buffer(); // flush serial buffer before loop

    if (start_paradox)
    {
        use_paradox = true;
        if (pdx_panel_refresh)
            pdx_do_panel_login = true;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ########  ##     ##    ##        #######   #######  ########
// ##     ## ##     ##  ##   ##     ##       ##     ## ##     ## ##     ##
// ##     ## ##     ##   ## ##      ##       ##     ## ##     ## ##     ##
// ########  ##     ##    ###       ##       ##     ## ##     ## ########
// ##        ##     ##   ## ##      ##       ##     ## ##     ## ##
// ##        ##     ##  ##   ##     ##       ##     ## ##     ## ##
// ##        ########  ##     ##    ########  #######   #######  ##

void paradox_loop()
{
    if (Serial.available() >= PDX_MSG_LENGTH)
    {
        pdx_read_data();
    }

    if (pdx_got_data)
    {
        pdx_check_data();
        return;
    }

    if (pdx_do_panel_login)
        if (pdx_panel_connection < 4)
        {
            if ((unsigned long)(millis() - pdx_panel_login_start) > 2000)
                pdx_panel_connection = 0;

            panel_login();
            return;
        }

    if (pdx_panel_connection > 3) // if logged in
    {
        if (!WDC) // bit0 of command byte from Paradox
        {
            pdx_panel_connection = 0;
            return;
        }
        if (command > 0)
        {
            panel_command();
            if (!pdx_panel_refresh)
                pdx_do_panel_login = false;
            return;
        }
    }
    else
    {
        if (command > 0)
        {
            pdx_do_panel_login = true;
            return;
        }
    }

    if (pdx_panel_refresh) // should we get panel data?
    {
        if (pdx_panel_data_periodic)
        {
            // this is periodic refresh, default each hour
            // check if it is time to start a panel data fetch
            if ((unsigned long)(millis() - pdx_panel_data_periodic_last_refresh) > pdx_panel_data_period)
            {
                // loop through all 6 panels to get data
                if ((unsigned long)(millis() - pdx_panel_last_refresh) > (pdx_panel_refresh_time * 1000))
                {
                    if (pdx_panel_connection < 4)
                    {
                        pdx_do_panel_login = true;
                        return;
                    }

                    pdx_panel_get_status();
                    pdx_panel_number_request++;

                    pdx_panel_last_refresh = millis();

                    if (pdx_panel_number_request > 5)
                    {
                        // reset panel number to 0 for next time
                        pdx_panel_number_request = 0;
                        // mark the time when we got all panel data
                        pdx_panel_data_periodic_last_refresh = millis();
                        // connection not needed anymore
                        pdx_do_panel_login = false;
                        pdx_panel_disconnect();
                    }
                }
            }
        }
        else
        {
            // this is constant refresh of panel data
            // check if we are logged in
            if (pdx_panel_connection < 4)
            {
                pdx_do_panel_login = true;
                return;
            }

            if ((unsigned long)(millis() - pdx_panel_last_refresh) > (pdx_panel_refresh_time * 1000))
            {
                pdx_panel_get_status();
                pdx_panel_number_request++;

                pdx_panel_last_refresh = millis();

                if (pdx_panel_number_request > 5)
                    pdx_panel_number_request = 0;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ########    ###    ########     ########     ###    ########    ###
// ##     ## ##         ## ##   ##     ##    ##     ##   ## ##      ##      ## ##
// ##     ## ##        ##   ##  ##     ##    ##     ##  ##   ##     ##     ##   ##
// ########  ######   ##     ## ##     ##    ##     ## ##     ##    ##    ##     ##
// ##   ##   ##       ######### ##     ##    ##     ## #########    ##    #########
// ##    ##  ##       ##     ## ##     ##    ##     ## ##     ##    ##    ##     ##
// ##     ## ######## ##     ## ########     ########  ##     ##    ##    ##     ##

void pdx_read_data()
{
    if (Serial.available() < PDX_MSG_LENGTH)
        return;

    for (uint8_t i = 0; i < PDX_MSG_LENGTH; i++)
    {
        pdx_rx_buffer[i] = Serial.read();
        yield();
    }

    pdx_got_data = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  ##     ## ########  ######  ##    ##    ########     ###    ########    ###
// ##    ## ##     ## ##       ##    ## ##   ##     ##     ##   ## ##      ##      ## ##
// ##       ##     ## ##       ##       ##  ##      ##     ##  ##   ##     ##     ##   ##
// ##       ######### ######   ##       #####       ##     ## ##     ##    ##    ##     ##
// ##       ##     ## ##       ##       ##  ##      ##     ## #########    ##    #########
// ##    ## ##     ## ##       ##    ## ##   ##     ##     ## ##     ##    ##    ##     ##
//  ######  ##     ## ########  ######  ##    ##    ########  ##     ##    ##    ##     ##

void pdx_check_data()
{
    pdx_got_data = false;

    uint8_t command = (pdx_rx_buffer[0] & 0xF0); // command portion of command (first) byte

    if (command != 0xE0 && command != 0x90 && command != 0x80 && command != 0x70 && command != 0x60 && command != 0x50 && command != 0x40 && command != 0x30 && command != 0x00)
    {
        // data not usable
        pdx_flush_serial_buffer();
        return;
    }

    if (!pdx_check_checksum())
    {
        // data not usable
        pdx_flush_serial_buffer();
        return;
    }

    // read LSB from command byte
    WDC = bitRead(pdx_rx_buffer[0], 0);
    WLC = bitRead(pdx_rx_buffer[0], 1);
    SIA = bitRead(pdx_rx_buffer[0], 2);

    char topic[128];
    sprintf(topic, "%s%s%s%s/wdc", LOC, TIP, NAME, XTRA);
    client.publish(topic, String(bitRead(pdx_rx_buffer[0], 0)), false, 0);

    switch (command)
    {
    case 0x00: // panel answer to PC request
        if (pdx_panel_connection != 1)
        {
            // data not usable
            pdx_flush_serial_buffer();
            break;
        }
        PPI = pdx_rx_buffer[4];
        PFV = pdx_rx_buffer[5];
        PFR = pdx_rx_buffer[6];
        PFB = pdx_rx_buffer[7];
        PPID1 = pdx_rx_buffer[8];
        PPID2 = pdx_rx_buffer[9];
        if ((PPI & 0xF0) == 0x40)
        {
            TFB = pdx_rx_buffer[15];
            TF = pdx_rx_buffer[16];
            TFV = pdx_rx_buffer[17];
            TFR = pdx_rx_buffer[18];
            TNFL = pdx_rx_buffer[19];
            THR = pdx_rx_buffer[21];
            NFLTH = bitRead(pdx_rx_buffer[20], 1);
            CCD = bitRead(pdx_rx_buffer[20], 0);
        }
        pdx_panel_connection = 2;
        pdx_panel_data_login = true;
        break;

    case 0x10:
        pdx_panel_connection = 4;
        WDC = true;
        if (pdx_rx_buffer[1] == 0x25 && pdx_rx_buffer[2] == 0x10)
        {
            partition_rights_access_1 = bitRead(pdx_rx_buffer[4], 0);
            partition_rights_access_2 = bitRead(pdx_rx_buffer[4], 1);
        }
        break;

    case 0x30:
        // time set ok
        break;

    case 0x40:
        // pdx_rx_buffer[2] returns executed action
        break;

    case 0x50:
        if (pdx_rx_buffer[2] == 0x80) // it is a panel status response
            pdx_got_panel_data = true;
        break;

    case 0x70:
        // error command
        pdx_panel_connection = 0;
        break;

    case 0xE0:
        // event data
        pdx_got_event = true;

        if (pdx_rx_buffer[7] == 0x30 && pdx_rx_buffer[8] == 0x02)
            pdx_panel_connection = 4;

        if (pdx_rx_buffer[7] == 0x30 && pdx_rx_buffer[8] == 0x03)
            pdx_panel_connection = 0;
        break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  ##     ## ########  ######  ##    ##     ######  ##     ## ########  ######  ##    ##  ######  ##     ## ##     ##
// ##    ## ##     ## ##       ##    ## ##   ##     ##    ## ##     ## ##       ##    ## ##   ##  ##    ## ##     ## ###   ###
// ##       ##     ## ##       ##       ##  ##      ##       ##     ## ##       ##       ##  ##   ##       ##     ## #### ####
// ##       ######### ######   ##       #####       ##       ######### ######   ##       #####     ######  ##     ## ## ### ##
// ##       ##     ## ##       ##       ##  ##      ##       ##     ## ##       ##       ##  ##         ## ##     ## ##     ##
// ##    ## ##     ## ##       ##    ## ##   ##     ##    ## ##     ## ##       ##    ## ##   ##  ##    ## ##     ## ##     ##
//  ######  ##     ## ########  ######  ##    ##     ######  ##     ## ########  ######  ##    ##  ######   #######  ##     ##

bool pdx_check_checksum()
{
    uint16_t checksum = 0;
    uint8_t calc = 0;
    uint8_t i = 0;

    for (i = 0; i < PDX_CHK_LENGTH; i++)
    {
        checksum += pdx_rx_buffer[i];
    }
    calc = checksum & 0xFF;

    if (calc == pdx_rx_buffer[36])
        return true;

    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ##    ## ##          ##        #######   ######   #### ##    ##
// ##     ## ###   ## ##          ##       ##     ## ##    ##   ##  ###   ##
// ##     ## ####  ## ##          ##       ##     ## ##         ##  ####  ##
// ########  ## ## ## ##          ##       ##     ## ##   ####  ##  ## ## ##
// ##        ##  #### ##          ##       ##     ## ##    ##   ##  ##  ####
// ##        ##   ### ##          ##       ##     ## ##    ##   ##  ##   ###
// ##        ##    ## ########    ########  #######   ######   #### ##    ##

void panel_login()
{
    // if it is the first step in establishing communications
    if (pdx_panel_connection == 0)
    {
        pdx_panel_login_start = millis();
        pdx_panel_connection = 1;
        pdx_clear_tx_buffer();

        // STEP 1 of login, start communication command
        pdx_tx_buffer[0] = 0x5F;
        pdx_tx_buffer[1] = 0x20;
        pdx_tx_buffer[33] = SourceID;
        pdx_tx_buffer[34] = UserID >> 8;
        pdx_tx_buffer[35] = UserID;

        send_data();
        return;
    }

    // if we got response from panel to our previous start communication command
    if (pdx_panel_connection == 2)
    {
        pdx_panel_connection = 3;

        pdx_clear_tx_buffer();

        // STEP 2 of login, initialize communication command
        pdx_tx_buffer[0] = 0x00;
        pdx_tx_buffer[4] = PPI;
        pdx_tx_buffer[5] = PFV;
        pdx_tx_buffer[6] = PFR;
        pdx_tx_buffer[7] = PFB;

        pdx_tx_buffer[13] = 0x55;
        if (UserPASS > 0xFFFF)
        {
            pdx_tx_buffer[14] = UserPASS >> 16;
            pdx_tx_buffer[15] = UserPASS >> 8;
            pdx_tx_buffer[16] = UserPASS;
        }
        else
        {
            pdx_tx_buffer[14] = UserPASS >> 8;
            pdx_tx_buffer[15] = UserPASS;
            pdx_tx_buffer[16] = 0x00;
        }

        pdx_tx_buffer[33] = SourceID;
        pdx_tx_buffer[34] = UserID >> 8;
        pdx_tx_buffer[35] = UserID;

        send_data();
        return;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ##    ## ##           ######   ######## ########     ######  ########    ###    ######## ##     ##  ######
// ##     ## ###   ## ##          ##    ##  ##          ##       ##    ##    ##      ## ##      ##    ##     ## ##    ##
// ##     ## ####  ## ##          ##        ##          ##       ##          ##     ##   ##     ##    ##     ## ##
// ########  ## ## ## ##          ##   #### ######      ##        ######     ##    ##     ##    ##    ##     ##  ######
// ##        ##  #### ##          ##    ##  ##          ##             ##    ##    #########    ##    ##     ##       ##
// ##        ##   ### ##          ##    ##  ##          ##       ##    ##    ##    ##     ##    ##    ##     ## ##    ##
// ##        ##    ## ########     ######   ########    ##        ######     ##    ##     ##    ##     #######   ######

void pdx_panel_get_status()
{
    pdx_clear_tx_buffer();

    pdx_tx_buffer[0] = 0x50;
    pdx_tx_buffer[2] = 0x80; // validation to distinguish from EEprom read
    pdx_tx_buffer[3] = pdx_panel_number_request;
    pdx_tx_buffer[33] = SourceID;
    pdx_tx_buffer[34] = UserID >> 8;
    pdx_tx_buffer[35] = UserID;

    send_data();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ##    ## ##           ######   #######  ##     ## ##     ##    ###    ##    ## ########
// ##     ## ###   ## ##          ##    ## ##     ## ###   ### ###   ###   ## ##   ###   ## ##     ##
// ##     ## ####  ## ##          ##       ##     ## #### #### #### ####  ##   ##  ####  ## ##     ##
// ########  ## ## ## ##          ##       ##     ## ## ### ## ## ### ## ##     ## ## ## ## ##     ##
// ##        ##  #### ##          ##       ##     ## ##     ## ##     ## ######### ##  #### ##     ##
// ##        ##   ### ##          ##    ## ##     ## ##     ## ##     ## ##     ## ##   ### ##     ##
// ##        ##    ## ########     ######   #######  ##     ## ##     ## ##     ## ##    ## ########

void panel_command()
{
    pdx_clear_tx_buffer();

    pdx_tx_buffer[0] = 0x40;
    pdx_tx_buffer[2] = command;
    pdx_tx_buffer[3] = subcommand;
    pdx_tx_buffer[33] = SourceID;
    pdx_tx_buffer[34] = UserID >> 8;
    pdx_tx_buffer[35] = UserID;

    send_data();

    command = 0;
    subcommand = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  ######## ########    ######## #### ##     ## ########
// ##    ## ##          ##          ##     ##  ###   ### ##
// ##       ##          ##          ##     ##  #### #### ##
//  ######  ######      ##          ##     ##  ## ### ## ######
//       ## ##          ##          ##     ##  ##     ## ##
// ##    ## ##          ##          ##     ##  ##     ## ##
//  ######  ########    ##          ##    #### ##     ## ########

void panel_set_time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute)
{
    pdx_clear_tx_buffer();

    pdx_tx_buffer[0] = 0x30;
    pdx_tx_buffer[4] = 20;
    pdx_tx_buffer[5] = year - 2000;
    pdx_tx_buffer[6] = month;
    pdx_tx_buffer[7] = day;
    pdx_tx_buffer[8] = hour;
    pdx_tx_buffer[9] = minute;
    pdx_tx_buffer[33] = SourceID;
    pdx_tx_buffer[34] = UserID >> 8;
    pdx_tx_buffer[35] = UserID;

    send_data();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ##    ## ##          ########  ####  ######   ######   #######  ##    ## ##    ## ########  ######  ########
// ##     ## ###   ## ##          ##     ##  ##  ##    ## ##    ## ##     ## ###   ## ###   ## ##       ##    ##    ##
// ##     ## ####  ## ##          ##     ##  ##  ##       ##       ##     ## ####  ## ####  ## ##       ##          ##
// ########  ## ## ## ##          ##     ##  ##   ######  ##       ##     ## ## ## ## ## ## ## ######   ##          ##
// ##        ##  #### ##          ##     ##  ##        ## ##       ##     ## ##  #### ##  #### ##       ##          ##
// ##        ##   ### ##          ##     ##  ##  ##    ## ##    ## ##     ## ##   ### ##   ### ##       ##    ##    ##
// ##        ##    ## ########    ########  ####  ######   ######   #######  ##    ## ##    ## ########  ######     ##

void pdx_panel_disconnect()
{

    pdx_clear_tx_buffer();
    pdx_tx_buffer[0] = 0x70;
    pdx_tx_buffer[2] = 0x05; // validation byte
    pdx_tx_buffer[33] = SourceID;
    pdx_tx_buffer[34] = UserID >> 8;
    pdx_tx_buffer[35] = UserID;

    send_data();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  ######## ##    ## ########        ########     ###    ########    ###
// ##    ## ##       ###   ## ##     ##       ##     ##   ## ##      ##      ## ##
// ##       ##       ####  ## ##     ##       ##     ##  ##   ##     ##     ##   ##
//  ######  ######   ## ## ## ##     ##       ##     ## ##     ##    ##    ##     ##
//       ## ##       ##  #### ##     ##       ##     ## #########    ##    #########
// ##    ## ##       ##   ### ##     ##       ##     ## ##     ##    ##    ##     ##
//  ######  ######## ##    ## ########        ########  ##     ##    ##    ##     ##

void send_data()
{
    uint16_t checksum = 0;
    uint8_t i = 0;

    for (i = 0; i < PDX_CHK_LENGTH; i++)
    {
        checksum += pdx_tx_buffer[i];
    }
    pdx_tx_buffer[36] = checksum;

    Serial.write(pdx_tx_buffer, PDX_MSG_LENGTH);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  ##       ########     ######## ##     ##    ########  ##     ## ######## ######## ######## ########
// ##    ## ##       ##     ##       ##     ##   ##     ##     ## ##     ## ##       ##       ##       ##     ##
// ##       ##       ##     ##       ##      ## ##      ##     ## ##     ## ##       ##       ##       ##     ##
// ##       ##       ########        ##       ###       ########  ##     ## ######   ######   ######   ########
// ##       ##       ##   ##         ##      ## ##      ##     ## ##     ## ##       ##       ##       ##   ##
// ##    ## ##       ##    ##        ##     ##   ##     ##     ## ##     ## ##       ##       ##       ##    ##
//  ######  ######## ##     ##       ##    ##     ##    ########   #######  ##       ##       ######## ##     ##

void pdx_clear_tx_buffer()
{
    for (uint8_t i = 0; i < PDX_MSG_LENGTH; i++)
        pdx_tx_buffer[i] = 0x00;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ######## ##       ##     ##  ######  ##     ##     ######  ######## ########  ####    ###    ##
// ##       ##       ##     ## ##    ## ##     ##    ##    ## ##       ##     ##  ##    ## ##   ##
// ##       ##       ##     ## ##       ##     ##    ##       ##       ##     ##  ##   ##   ##  ##
// ######   ##       ##     ##  ######  #########     ######  ######   ########   ##  ##     ## ##
// ##       ##       ##     ##       ## ##     ##          ## ##       ##   ##    ##  ######### ##
// ##       ##       ##     ## ##    ## ##     ##    ##    ## ##       ##    ##   ##  ##     ## ##
// ##       ########  #######   ######  ##     ##     ######  ######## ##     ## #### ##     ## ########

void pdx_flush_serial_buffer()
{
    Serial.flush();
    while (Serial.read() >= 0)
        yield();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////