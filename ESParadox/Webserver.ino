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

    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_end);
    server.sendContent_P(page_content_title);

    char buffer[256];

    sprintf_P(buffer, menu_entry, "/device", "Device", "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/wifi", "WiFi", "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/mqtt", "MQTT", "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/paradox", "Paradox", "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/update", "Update firmware", menu_entry_check_update);
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/format", "Erase settings", menu_entry_check_erase);
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/restart", "Restart", menu_entry_check_restart);
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/sysinfo", "System information", "");
    server.sendContent(buffer);

    server.sendContent_P(page_footer);
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

    server.sendContent_P(page_header_start);
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

    char buffer[256];

    server.sendContent_P(form_start);

    sprintf_P(buffer, form_text_field_string, "SSIDa", "SSIDa", SSIDa);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "PASSa", "PASSa", PASSa);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "SSIDb", "SSIDb", SSIDb);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "PASSb", "PASSb", PASSb);
    server.sendContent(buffer);

    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent(F("<hr>Save to config file: "));
        server.sendContent(save_wifi());
    }

    server.sendContent_P(page_footer);
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

    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_end);

    char buffer[256];

    server.sendContent_P(form_start);

    sprintf_P(buffer, form_text_field_string_min, "LOC", "LOC", LOC);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string_min, "TIP", "TIP", TIP);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string_min, "NAME", "NAME", NAME);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, "Update URL", "update_url", update_url);
    server.sendContent(buffer);
    sprintf_P(buffer, form_yes_no, "Heartbeat", "heartbeat", heartbeat ? "selected" : "", heartbeat ? "" : "selected");
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_int_min, "Heartbeat minutes", "heartbeat_minutes", heartbeat_minutes);
    server.sendContent(buffer);
    sprintf_P(buffer, form_yes_no, "Start webserver", "start_webserver", start_webserver ? "selected" : "", start_webserver ? "" : "selected");
    server.sendContent(buffer);

    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent(F("<hr>Save to config file: "));
        server.sendContent(save_device());
    }

    server.sendContent_P(page_footer);
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

    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_end);

    char buffer[256];

    server.sendContent_P(form_start);

    sprintf_P(buffer, form_text_field_string, "MQTT host/ip", "MQTT_HOST", MQTT_HOST);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_int, "MQTT port", "MQTT_PORT", MQTT_PORT);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "MQTT user", "MQTT_USER", MQTT_USER);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "MQTT password", "MQTT_PASS", MQTT_PASS);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "WILL subtopic", "WILL", WILL);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, "PUB subtopic", "PUB", PUB);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, "SUB subtopic", "SUB", SUB);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, "STAT subtopic", "STAT", STAT);
    server.sendContent(buffer);

    sprintf_P(buffer, form_yes_no, "Start MQTT<br>(needs WiFi enabled)", "start_mqtt", start_mqtt ? "selected" : "", start_mqtt ? "" : "selected");
    server.sendContent(buffer);

    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent(F("<hr>Save to config file: "));
        server.sendContent(save_mqtt());
    }

    server.sendContent_P(page_footer);
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

    if (server.hasArg("UserID"))
        UserID = strtoul(server.arg("UserID").c_str(), 0, 16);

    if (server.hasArg("UserPASS"))
        UserPASS = strtoul(server.arg("UserPASS").c_str(), 0, 16);

    if (server.hasArg("pdx_panel_refresh_time"))
        pdx_panel_refresh_time = server.arg("pdx_panel_refresh_time").toInt();

    if (server.hasArg("pdx_panel_refresh"))
        pdx_panel_refresh = server.arg("pdx_panel_refresh").toInt();

    if (server.hasArg("start_paradox"))
        start_paradox = server.arg("start_paradox").toInt();

    ///////////
    yield();
    ///////////

    if (!chunked_response(200))
        return;

    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_end);

    char buffer[256];

    server.sendContent_P(form_start);

    sprintf_P(buffer, form_text_field_hex4, "User ID", "UserID", UserID);
    server.sendContent(buffer);

    if (UserPASS > 0xFFFF)
        sprintf_P(buffer, form_text_field_hex6, "User Password", "UserPASS", UserPASS);
    else
        sprintf_P(buffer, form_text_field_hex4, "User Password", "UserPASS", UserPASS);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_int, "Panel refresh time<br>(seconds)", "pdx_panel_refresh_time", pdx_panel_refresh_time);
    server.sendContent(buffer);

    sprintf_P(buffer, form_yes_no, "Fetch panel data", "pdx_panel_refresh", pdx_panel_refresh ? "selected" : "", pdx_panel_refresh ? "" : "selected");
    server.sendContent(buffer);

    sprintf_P(buffer, form_yes_no, "Start Paradox", "start_paradox", start_paradox ? "selected" : "", start_paradox ? "" : "selected");
    server.sendContent(buffer);

    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent(F("<hr>Save to config file: "));
        server.sendContent(save_paradox());
    }

    server.sendContent_P(page_footer);
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

    char buffer[256] = {0};

    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_end);
    server.sendContent_P(html_table_i);

    ////////////////////////

    sprintf_P(buffer, PSTR("<td>FW name</td><td>%s</td>"), FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>FW version</td><td>%.2f</td>"), VERSION);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>FW size</td><td>%.2f KB</td>"), ESP.getSketchSize() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>free flash size</td><td>%.2f KB</td>"), ESP.getFreeSketchSpace() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>FW MD5</td><td>%s</td>"), ESP.getSketchMD5().c_str());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>FW CRC</td><td>%s</td>"), ESP.checkFlashCRC() ? "OK" : "ERROR!!!");
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Core version</td><td>%s</td>"), ESP.getCoreVersion().c_str());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>SDK version</td><td>%s</td>"), ESP.getSdkVersion());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    ////////////////////////
    server.sendContent_P(html_table_tr_hr);
    ////////////////////////

    sprintf_P(buffer, PSTR("<td>Reset reason</td><td>%s</td>"), ESP.getResetReason().c_str());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Free Heap</td><td>%.2f KB</td>"), ESP.getFreeHeap() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Max Free Block</td><td>%.2f KB</td>"), ESP.getMaxFreeBlockSize() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Heap Fragmentation</td><td>%d%%</td>"), ESP.getHeapFragmentation());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    ////////////////////////
    server.sendContent_P(html_table_tr_hr);
    ////////////////////////

    sprintf_P(buffer, PSTR("<td>ESP Chip ID</td><td>%06X</td>"), ESP.getChipId());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>CPU frequency</td><td>%d MHz</td>"), ESP.getCpuFreqMHz());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Flash Chip ID</td><td>%06X</td>"), ESP.getFlashChipId());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Flash frequency</td><td>%d MHz</td>"), ESP.getFlashChipSpeed() / 1000000);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Used flash size</td><td>%d KB</td>"), ESP.getFlashChipSize() / 1024);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Flash size</td><td>%d KB</td>"), ESP.getFlashChipRealSize() / 1024);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    ////////////////////////

    server.sendContent_P(html_table_s);

    server.sendContent(F("<br><br><form action='/'><input type='submit' value='Main menu'></form>"));
    server.sendContent_P(page_footer);
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

    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_refresh);
    server.sendContent_P(page_header_end);
    server.sendContent_P(page_content_title);
    server.sendContent_P(page_content_restart);
    server.sendContent_P(page_content_main_menu_button);
    server.sendContent_P(page_footer);
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

    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_refresh);
    server.sendContent_P(page_header_end);
    server.sendContent_P(page_content_title);
    server.sendContent_P(page_content_restart);
    server.sendContent_P(page_content_main_menu_button);
    server.sendContent_P(page_footer);
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

    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_refresh);
    server.sendContent_P(page_header_end);
    server.sendContent_P(page_content_title);
    server.sendContent_P(page_content_update);
    server.sendContent_P(page_content_main_menu_button);
    server.sendContent_P(page_footer);
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
    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_refresh);
    server.sendContent_P(page_header_end);
    server.sendContent_P(page_content_title);
    server.sendContent_P(page_content_not_found);
    server.sendContent_P(page_content_main_menu_button);
    server.sendContent_P(page_footer);
    server.chunkedResponseFinalize();
}