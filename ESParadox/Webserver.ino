//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##     ##    ###    #### ##    ##    ########     ###     ######   ########
// ###   ###   ## ##    ##  ###   ##    ##     ##   ## ##   ##    ##  ##
// #### ####  ##   ##   ##  ####  ##    ##     ##  ##   ##  ##        ##
// ## ### ## ##     ##  ##  ## ## ##    ########  ##     ## ##   #### ######
// ##     ## #########  ##  ##  ####    ##        ######### ##    ##  ##
// ##     ## ##     ##  ##  ##   ###    ##        ##     ## ##    ##  ##
// ##     ## ##     ## #### ##    ##    ##        ##     ##  ######   ########

void page_main()
{
    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    sprintf_P(buffer, page_content_title, NAME);
    server.sendContent(buffer);

    sprintf_P(buffer, menu_entry, "/device", menu_entry_device, "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/wifi", menu_entry_wifi, "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/mqtt", menu_entry_mqtt, "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/paradox", menu_entry_paradox, "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/update", menu_entry_update, menu_entry_check_update);
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/format", menu_entry_erase, menu_entry_check_erase);
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/restart", menu_entry_restart, menu_entry_check_restart);
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/sysinfo", menu_entry_sysinfo, "");
    server.sendContent(buffer);

    sprintf_P(buffer, menu_entry, "/debug", "DEBUG", "");
    server.sendContent(buffer);

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ######## ##     ## ####  ######  ########
// ##     ## ##       ##     ##  ##  ##    ## ##
// ##     ## ##       ##     ##  ##  ##       ##
// ##     ## ######   ##     ##  ##  ##       ######
// ##     ## ##        ##   ##   ##  ##       ##
// ##     ## ##         ## ##    ##  ##    ## ##
// ########  ########    ###    ####  ######  ########

void cfg_page_device()
{
    if (server.hasArg("LOC"))
        strlcpy(LOC, server.arg("LOC").c_str(), sizeof(LOC));

    if (server.hasArg("TIP"))
        strlcpy(TIP, server.arg("TIP").c_str(), sizeof(TIP));

    if (server.hasArg("NAME"))
        strlcpy(NAME, server.arg("NAME").c_str(), sizeof(NAME));

    if (server.hasArg("XTRA"))
        strlcpy(XTRA, server.arg("XTRA").c_str(), sizeof(XTRA));

    if (server.hasArg("update_url"))
        strlcpy(update_url, server.arg("update_url").c_str(), sizeof(update_url));

    if (server.hasArg("heartbeat"))
        heartbeat = server.arg("heartbeat").toInt();

    if (server.hasArg("heartbeat_minutes"))
        heartbeat_minutes = server.arg("heartbeat_minutes").toInt();

    if (server.hasArg("start_webserver"))
        start_webserver = server.arg("start_webserver").toInt();

    ///////////
    yield();
    ///////////

    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    server.sendContent_P(form_start);

    server.sendContent_P(form_start);
    sprintf_P(buffer, form_text_field_string_min, txt_loc, "LOC", LOC);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string_min, txt_tip, "TIP", TIP);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string_min, txt_name, "NAME", NAME);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string_min, txt_xtra, "XTRA", XTRA);
    server.sendContent(buffer);
    sprintf_P(buffer, txt_mqtt_main_topic, LOC, TIP, NAME, XTRA);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, txt_update_url, "update_url", update_url);
    server.sendContent(buffer);
    sprintf_P(buffer, form_yes_no, txt_heartbeat, "heartbeat", heartbeat ? "selected" : "", heartbeat ? "" : "selected");
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_int_min, txt_heartbeat_minutes, "heartbeat_minutes", heartbeat_minutes);
    server.sendContent(buffer);
    sprintf_P(buffer, form_yes_no, txt_start_webserver, "start_webserver", start_webserver ? "selected" : "", start_webserver ? "" : "selected");
    server.sendContent(buffer);

    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent_P(save_to_config);
        server.sendContent(fs_save_device());
    }

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##      ## #### ######## ####
// ##  ##  ##  ##  ##        ##
// ##  ##  ##  ##  ##        ##
// ##  ##  ##  ##  ######    ##
// ##  ##  ##  ##  ##        ##
// ##  ##  ##  ##  ##        ##
//  ###  ###  #### ##       ####

void cfg_page_wifi()
{
    if (server.hasArg("scan"))
        WiFi.scanNetworks();

    if (server.hasArg("SSIDa"))
        strlcpy(SSIDa, server.arg("SSIDa").c_str(), sizeof(SSIDa));

    if (server.hasArg("PASSa"))
        strlcpy(PASSa, server.arg("PASSa").c_str(), sizeof(PASSa));

    if (server.hasArg("SSIDb"))
        strlcpy(SSIDb, server.arg("SSIDb").c_str(), sizeof(SSIDb));

    if (server.hasArg("PASSb"))
        strlcpy(PASSb, server.arg("PASSb").c_str(), sizeof(PASSb));

    ///////////
    yield();
    ///////////

    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    int i = 0;
    int n = WiFi.scanComplete();

    if (n >= 0)
    {
        server.sendContent_P(form_start);
        for (i = 0; i < n; i++)
        {
            server.sendContent(F("<input type='submit' name='SSIDa' value='"));
            server.sendContent(String() + WiFi.SSID(i) + "'> - ch." + WiFi.channel(i) + " (" + WiFi.RSSI(i) + " dBm)<br>");
        }
        server.sendContent_P(form_end);
        server.sendContent_P(html_hr);
    }

    server.sendContent_P(menu_entry_scan_wifi);
    server.sendContent_P(html_hr);

    server.sendContent_P(form_start);

    sprintf_P(buffer, form_text_field_string, "SSID 1", "SSIDa", SSIDa);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "PASS 1", "PASSa", PASSa);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "SSID 2", "SSIDb", SSIDb);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "PASS 2", "PASSb", PASSb);
    server.sendContent(buffer);

    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent_P(save_to_config);
        server.sendContent(fs_save_wifi());
    }

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##     ##  #######  ######## ########
// ###   ### ##     ##    ##       ##
// #### #### ##     ##    ##       ##
// ## ### ## ##     ##    ##       ##
// ##     ## ##  ## ##    ##       ##
// ##     ## ##    ##     ##       ##
// ##     ##  ##### ##    ##       ##

void cfg_page_mqtt()
{
    if (server.hasArg("MQTT_HOST"))
        strlcpy(MQTT_HOST, server.arg("MQTT_HOST").c_str(), sizeof(MQTT_HOST));

    if (server.hasArg("MQTT_PORT"))
        MQTT_PORT = server.arg("MQTT_PORT").toInt();

    if (server.hasArg("MQTT_USER"))
        strlcpy(MQTT_USER, server.arg("MQTT_USER").c_str(), sizeof(MQTT_USER));

    if (server.hasArg("MQTT_PASS"))
        strlcpy(MQTT_PASS, server.arg("MQTT_PASS").c_str(), sizeof(MQTT_PASS));

    if (server.hasArg("WILL"))
        strlcpy(WILL, server.arg("WILL").c_str(), sizeof(WILL));

    if (server.hasArg("PUB"))
        strlcpy(PUB, server.arg("PUB").c_str(), sizeof(PUB));

    if (server.hasArg("SUB"))
        strlcpy(SUB, server.arg("SUB").c_str(), sizeof(SUB));

    if (server.hasArg("STAT"))
        strlcpy(STAT, server.arg("STAT").c_str(), sizeof(STAT));

    if (server.hasArg("start_mqtt"))
        start_mqtt = server.arg("start_mqtt").toInt();

    ///////////
    yield();
    ///////////

    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    server.sendContent_P(form_start);

    sprintf_P(buffer, form_text_field_string, txt_mqtt_host, "MQTT_HOST", MQTT_HOST);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_int, txt_mqtt_port, "MQTT_PORT", MQTT_PORT);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, txt_mqtt_user, "MQTT_USER", MQTT_USER);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, txt_mqtt_pass, "MQTT_PASS", MQTT_PASS);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, txt_mqtt_will, "WILL", WILL);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, txt_mqtt_pub, "PUB", PUB);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, txt_mqtt_sub, "SUB", SUB);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, txt_mqtt_stat, "STAT", STAT);
    server.sendContent(buffer);

    sprintf_P(buffer, form_yes_no, txt_mqtt_start, "start_mqtt", start_mqtt ? "selected" : "", start_mqtt ? "" : "selected");
    server.sendContent(buffer);

    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent_P(save_to_config);
        server.sendContent(fs_save_mqtt());
    }

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########     ###    ########     ###    ########   #######  ##     ##
// ##     ##   ## ##   ##     ##   ## ##   ##     ## ##     ##  ##   ##
// ##     ##  ##   ##  ##     ##  ##   ##  ##     ## ##     ##   ## ##
// ########  ##     ## ########  ##     ## ##     ## ##     ##    ###
// ##        ######### ##   ##   ######### ##     ## ##     ##   ## ##
// ##        ##     ## ##    ##  ##     ## ##     ## ##     ##  ##   ##
// ##        ##     ## ##     ## ##     ## ########   #######  ##     ##

void cfg_page_paradox()
{

    if (server.hasArg("start_paradox"))
        start_paradox = server.arg("start_paradox").toInt();

    if (server.hasArg("UserID"))
        UserID = strtoul(server.arg("UserID").c_str(), 0, 16);

    if (server.hasArg("UserPASS"))
    {
        uint32_t UserPASS_temp = strtoul(server.arg("UserPASS").c_str(), 0, 16);
        if (UserPASS_temp > 0)
            UserPASS = UserPASS_temp;
    }

    if (server.hasArg("pdx_panel_refresh"))
        pdx_panel_refresh = server.arg("pdx_panel_refresh").toInt();

    if (server.hasArg("pdx_panel_data_periodic"))
        pdx_panel_data_periodic = server.arg("pdx_panel_data_periodic").toInt();

    if (server.hasArg("pdx_panel_data_period") && server.hasArg("ppdpu"))
        pdx_panel_data_period = server.arg("pdx_panel_data_period").toInt() * server.arg("ppdpu").toInt();

    if (server.hasArg("pdx_panel_refresh_time"))
        pdx_panel_refresh_time = server.arg("pdx_panel_refresh_time").toInt();

    if (server.hasArg("year"))
    {
        uint16_t year = server.arg("year").toInt();
        uint8_t month = server.arg("month").toInt();
        uint8_t day = server.arg("day").toInt();
        uint8_t hour = server.arg("hour").toInt();
        uint8_t minute = server.arg("minute").toInt();

        if (year && month && day)
            panel_set_time(year, month, day, hour, minute);
    }

    ///////////
    yield();
    ///////////

    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    server.sendContent_P(form_start);

    sprintf_P(buffer, form_yes_no, txt_paradox_start, "start_paradox", start_paradox ? "selected" : "", start_paradox ? "" : "selected");
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_hex4, txt_paradox_userid, "UserID", UserID);
    server.sendContent(buffer);

    // this shows the current user password, depending on length
    // if (UserPASS > 0xFFFF)
    //     sprintf_P(buffer, form_text_field_hex6, txt_paradox_userpass, "UserPASS", UserPASS);
    // else
    //     sprintf_P(buffer, form_text_field_hex4, txt_paradox_userpass, "UserPASS", UserPASS);
    // server.sendContent(buffer);

    // this doesn't show the current user passord
    sprintf_P(buffer, form_text_field_user_password, txt_paradox_userpass, "UserPASS", UserPASS > 0 ? "is set up" : "absent");
    server.sendContent(buffer);

    sprintf_P(buffer, form_yes_no, txt_paradox_panel_fetch_data, "pdx_panel_refresh", pdx_panel_refresh ? "selected" : "", pdx_panel_refresh ? "" : "selected");
    server.sendContent(buffer);

    sprintf_P(buffer, form_yes_no, txt_paradox_panel_data_periodic, "pdx_panel_data_periodic", pdx_panel_data_periodic ? "selected" : "", pdx_panel_data_periodic ? "" : "selected");
    server.sendContent(buffer);

    bool ms = true;
    bool sec = false;
    bool min = false;
    bool hour = false;
    bool day = false;
    uint16_t temp = pdx_panel_data_period;

    if (pdx_panel_data_period % 86400000 == 0) // days
    {
        temp = pdx_panel_data_period / 86400000;
        ms = false;
        day = true;
    }
    else if (pdx_panel_data_period % 3600000 == 0) // hours
    {
        temp = pdx_panel_data_period / 3600000;
        ms = false;
        hour = true;
    }
    else if (pdx_panel_data_period % 60000 == 0) // minutes
    {
        temp = pdx_panel_data_period / 60000;
        ms = false;
        min = true;
    }
    else if (pdx_panel_data_period % 1000 == 0) // seconds
    {
        temp = pdx_panel_data_period / 1000;
        ms = false;
        sec = true;
    }

    sprintf_P(buffer, txt_paradox_panel_data_period, temp);
    server.sendContent(buffer);
    sprintf_P(buffer, form_timeout_select, "ppdpu", ms ? "selected" : "", sec ? "selected" : "", min ? "selected" : "", hour ? "selected" : "", day ? "selected" : "");
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_int, txt_paradox_panel_refresh_time, "pdx_panel_refresh_time", pdx_panel_refresh_time);
    server.sendContent(buffer);

    server.sendContent_P(html_hr);
    server.sendContent_P(form_buttons);

    server.sendContent_P(html_hr);
    server.sendContent_P(txt_paradox_update_time);
    server.sendContent_P(form_start);
    sprintf_P(buffer, form_text_field_string, txt_year, "year", "");
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, txt_month, "month", "");
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, txt_day, "day", "");
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, txt_hour, "hour", "");
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, txt_minute, "minute", "");
    server.sendContent(buffer);
    server.sendContent_P(txt_paradox_update_time_submit);
    server.sendContent_P(form_end);

    if (server.hasArg("save"))
    {
        server.sendContent_P(save_to_config);
        server.sendContent(fs_save_paradox());
    }

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  ##    ##  ######  #### ##    ## ########  #######
// ##    ##  ##  ##  ##    ##  ##  ###   ## ##       ##     ##
// ##         ####   ##        ##  ####  ## ##       ##     ##
//  ######     ##     ######   ##  ## ## ## ######   ##     ##
//       ##    ##          ##  ##  ##  #### ##       ##     ##
// ##    ##    ##    ##    ##  ##  ##   ### ##       ##     ##
//  ######     ##     ######  #### ##    ## ##        #######

void cfg_page_sysinfo()
{
    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);
    server.sendContent_P(html_table_i);

    ////////////////////////

    sprintf_P(buffer, txt_fw_name, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_fw_version, VERSION);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_fw_size, ESP.getSketchSize() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_free_flash_size, ESP.getFreeSketchSpace() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_fw_md5, ESP.getSketchMD5().c_str());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_fw_crc, ESP.checkFlashCRC() ? "OK" : "ERROR!!!");
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_core_version, ESP.getCoreVersion().c_str());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_sdk_version, ESP.getSdkVersion());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    ////////////////////////
    server.sendContent_P(html_table_tr_hr);
    ////////////////////////

    sprintf_P(buffer, txt_reset_reason, ESP.getResetReason().c_str());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_free_heap, ESP.getFreeHeap() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_max_free_block, ESP.getMaxFreeBlockSize() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_heap_fragmentation, ESP.getHeapFragmentation());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    ////////////////////////
    server.sendContent_P(html_table_tr_hr);
    ////////////////////////

    sprintf_P(buffer, txt_esp_chip_id, ESP.getChipId());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_cpu_freq, ESP.getCpuFreqMHz());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_flash_chip_id, ESP.getFlashChipId());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_flash_freq, ESP.getFlashChipSpeed() / 1000000);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_used_flash_size, ESP.getFlashChipSize() / 1024);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_real_flash_size, ESP.getFlashChipRealSize() / 1024);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    ////////////////////////

    server.sendContent_P(html_table_s);

    server.sendContent_P(back_button);

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ########  ######  ########    ###    ########  ########
// ##     ## ##       ##    ##    ##      ## ##   ##     ##    ##
// ##     ## ##       ##          ##     ##   ##  ##     ##    ##
// ########  ######    ######     ##    ##     ## ########     ##
// ##   ##   ##             ##    ##    ######### ##   ##      ##
// ##    ##  ##       ##    ##    ##    ##     ## ##    ##     ##
// ##     ## ########  ######     ##    ##     ## ##     ##    ##

void page_restart()
{

    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    sprintf_P(buffer, page_header_refresh, 10);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    sprintf_P(buffer, page_content_title, NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_content_restart);
    server.sendContent_P(page_content_main_menu_button);

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();

    delay(200);
    ESP.restart();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  #######  ########  ##     ##    ###    ########
// ##       ##     ## ##     ## ###   ###   ## ##      ##
// ##       ##     ## ##     ## #### ####  ##   ##     ##
// ######   ##     ## ########  ## ### ## ##     ##    ##
// ##       ##     ## ##   ##   ##     ## #########    ##
// ##       ##     ## ##    ##  ##     ## ##     ##    ##
// ##        #######  ##     ## ##     ## ##     ##    ##

void page_format()
{
    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    sprintf_P(buffer, page_header_refresh, 15);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    sprintf_P(buffer, page_content_title, NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_content_erase_settings);
    server.sendContent_P(page_content_main_menu_button);

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();

    delay(200);

    fs_format();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  ##     ## ##     ## ##    ## ##    ## ######## ########
// ##    ## ##     ## ##     ## ###   ## ##   ##  ##       ##     ##
// ##       ##     ## ##     ## ####  ## ##  ##   ##       ##     ##
// ##       ######### ##     ## ## ## ## #####    ######   ##     ##
// ##       ##     ## ##     ## ##  #### ##  ##   ##       ##     ##
// ##    ## ##     ## ##     ## ##   ### ##   ##  ##       ##     ##
//  ######  ##     ##  #######  ##    ## ##    ## ######## ########

bool chunked_response(uint16_t code)
{
    // use HTTP/1.1 Chunked response to avoid building a huge temporary string
    if (!server.chunkedResponseModeStart(code, "text/html"))
    {
        server.send(505, F("text/html"), F("HTTP1.1 required"));
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##     ## ########  ########     ###    ######## ########
// ##     ## ##     ## ##     ##   ## ##      ##    ##
// ##     ## ##     ## ##     ##  ##   ##     ##    ##
// ##     ## ########  ##     ## ##     ##    ##    ######
// ##     ## ##        ##     ## #########    ##    ##
// ##     ## ##        ##     ## ##     ##    ##    ##
//  #######  ##        ########  ##     ##    ##    ########

void page_update()
{
    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    sprintf_P(buffer, page_header_refresh, 40);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    sprintf_P(buffer, page_content_title, NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_content_update);
    server.sendContent_P(page_content_main_menu_button);

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();

    delay(200);
    do_ota_update = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##    ##  #######  ########    ########  #######  ##     ## ##    ## ########
// ###   ## ##     ##    ##       ##       ##     ## ##     ## ###   ## ##     ##
// ####  ## ##     ##    ##       ##       ##     ## ##     ## ####  ## ##     ##
// ## ## ## ##     ##    ##       ######   ##     ## ##     ## ## ## ## ##     ##
// ##  #### ##     ##    ##       ##       ##     ## ##     ## ##  #### ##     ##
// ##   ### ##     ##    ##       ##       ##     ## ##     ## ##   ### ##     ##
// ##    ##  #######     ##       ##        #######   #######  ##    ## ########

void page_not_found()
{
    if (!chunked_response(404))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    sprintf_P(buffer, page_header_refresh, 10);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    sprintf_P(buffer, page_content_title, NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_content_not_found);
    server.sendContent_P(page_content_main_menu_button);

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ######## ########  ##     ##  ######
// ##     ## ##       ##     ## ##     ## ##    ##
// ##     ## ##       ##     ## ##     ## ##
// ##     ## ######   ########  ##     ## ##   ####
// ##     ## ##       ##     ## ##     ## ##    ##
// ##     ## ##       ##     ## ##     ## ##    ##
// ########  ######## ########   #######   ######

void cfg_page_debug()
{
    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    sprintf_P(buffer, page_header_refresh_same, 1);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);
    server.sendContent_P(html_table_i);

    ////////////////////////
    server.sendContent_P(html_table_tr_hr);
    ////////////////////////

    sprintf_P(buffer, txt_sysinfo_line_d, "login", pdx_do_panel_login);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_sysinfo_line_lu, "time", pdx_panel_login_start);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_sysinfo_line_d, "level", pdx_panel_connection);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    ////////////////////////
    server.sendContent_P(html_table_tr_hr);
    ////////////////////////

    server.sendContent_P(html_table_s);

    server.sendContent_P(back_button);

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}