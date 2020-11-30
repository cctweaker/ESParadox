//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##          ##      ## #### ######## ####
// ##          ##  ##  ##  ##  ##        ##
// ##          ##  ##  ##  ##  ##        ##
// ##          ##  ##  ##  ##  ######    ##
// ##          ##  ##  ##  ##  ##        ##
// ##          ##  ##  ##  ##  ##        ##
// ########     ###  ###  #### ##       ####

bool fs_load_wifi()
{
    if (!LittleFS.begin())
        return false;

    // open config file for reading
    File configFile = LittleFS.open(wifi_cfg_file, "r");
    if (!configFile)
        return false;

    DynamicJsonDocument doc(512);

    // Read content from config file.
    deserializeJson(doc, configFile);
    configFile.close();
    LittleFS.end();

    if (doc.containsKey("SSIDa"))
        strlcpy(SSIDa, doc["SSIDa"], sizeof(SSIDa));
    if (doc.containsKey("PASSa"))
        strlcpy(PASSa, doc["PASSa"], sizeof(PASSa));

    if (doc.containsKey("SSIDb"))
        strlcpy(SSIDb, doc["SSIDb"], sizeof(SSIDb));
    if (doc.containsKey("PASSb"))
        strlcpy(PASSb, doc["PASSb"], sizeof(PASSb));

    doc.clear();

    // if ((strlen(SSIDa) && strlen(PASSa)) || (strlen(SSIDb) && strlen(PASSb)))
    return true;

    // return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##          ########  ######## ##     ## ####  ######  ########
// ##          ##     ## ##       ##     ##  ##  ##    ## ##
// ##          ##     ## ##       ##     ##  ##  ##       ##
// ##          ##     ## ######   ##     ##  ##  ##       ######
// ##          ##     ## ##        ##   ##   ##  ##       ##
// ##          ##     ## ##         ## ##    ##  ##    ## ##
// ########    ########  ########    ###    ####  ######  ########

bool fs_load_device()
{
    if (!LittleFS.begin())
        return false;

    // open config file for reading
    File configFile = LittleFS.open(dev_cfg_file, "r");
    if (!configFile)
        return false;

    DynamicJsonDocument doc(512);

    // Read content from config file.
    deserializeJson(doc, configFile);
    configFile.close();
    LittleFS.end();

    if (doc.containsKey("LOC"))
        strlcpy(LOC, doc["LOC"], sizeof(LOC));
    if (doc.containsKey("TIP"))
        strlcpy(TIP, doc["TIP"], sizeof(TIP));
    if (doc.containsKey("NAME"))
        strlcpy(NAME, doc["NAME"], sizeof(NAME));
    if (doc.containsKey("XTRA"))
        strlcpy(XTRA, doc["XTRA"], sizeof(XTRA));

    if (doc.containsKey("update_url"))
        strlcpy(update_url, doc["update_url"], sizeof(update_url));

    if (doc.containsKey("heartbeat"))
        heartbeat = doc["heartbeat"];
    if (doc.containsKey("heartbeat_minutes"))
        heartbeat_minutes = doc["heartbeat_minutes"];

    if (doc.containsKey("start_webserver"))
        start_webserver = doc["start_webserver"];

    doc.clear();

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##          ##     ##  #######  ######## ########
// ##          ###   ### ##     ##    ##       ##
// ##          #### #### ##     ##    ##       ##
// ##          ## ### ## ##     ##    ##       ##
// ##          ##     ## ##  ## ##    ##       ##
// ##          ##     ## ##    ##     ##       ##
// ########    ##     ##  ##### ##    ##       ##

bool fs_load_mqtt()
{
    if (!LittleFS.begin())
        return false;

    // open config file for reading
    File configFile = LittleFS.open(mqtt_cfg_file, "r");
    if (!configFile)
        return false;

    DynamicJsonDocument doc(512);

    // Read content from config file.
    deserializeJson(doc, configFile);
    configFile.close();
    LittleFS.end();

    if (doc.containsKey("MQTT_HOST"))
        strlcpy(MQTT_HOST, doc["MQTT_HOST"], sizeof(MQTT_HOST));
    if (doc.containsKey("MQTT_PORT"))
        MQTT_PORT = doc["MQTT_PORT"];
    if (doc.containsKey("MQTT_USER"))
        strlcpy(MQTT_USER, doc["MQTT_USER"], sizeof(MQTT_USER));
    if (doc.containsKey("MQTT_PASS"))
        strlcpy(MQTT_PASS, doc["MQTT_PASS"], sizeof(MQTT_PASS));

    if (doc.containsKey("WILL"))
        strlcpy(WILL, doc["WILL"], sizeof(WILL));
    if (doc.containsKey("PUB"))
        strlcpy(PUB, doc["PUB"], sizeof(PUB));
    if (doc.containsKey("SUB"))
        strlcpy(SUB, doc["SUB"], sizeof(SUB));
    if (doc.containsKey("STAT"))
        strlcpy(STAT, doc["STAT"], sizeof(STAT));

    if (doc.containsKey("start_mqtt"))
        start_mqtt = doc["start_mqtt"];

    doc.clear();

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##          ########     ###    ########     ###    ########   #######  ##     ##
// ##          ##     ##   ## ##   ##     ##   ## ##   ##     ## ##     ##  ##   ##
// ##          ##     ##  ##   ##  ##     ##  ##   ##  ##     ## ##     ##   ## ##
// ##          ########  ##     ## ########  ##     ## ##     ## ##     ##    ###
// ##          ##        ######### ##   ##   ######### ##     ## ##     ##   ## ##
// ##          ##        ##     ## ##    ##  ##     ## ##     ## ##     ##  ##   ##
// ########    ##        ##     ## ##     ## ##     ## ########   #######  ##     ##

bool fs_load_paradox()
{
    if (!LittleFS.begin())
        return false;

    // open config file for reading
    File configFile = LittleFS.open(paradox_cfg_file, "r");
    if (!configFile)
        return false;

    DynamicJsonDocument doc(512);

    // Read content from config file.
    deserializeJson(doc, configFile);
    configFile.close();
    LittleFS.end();

    if (doc.containsKey("start_paradox"))
        start_paradox = doc["start_paradox"];

    if (doc.containsKey("UserID"))
        UserID = doc["UserID"];

    if (doc.containsKey("UserPASS"))
        UserPASS = doc["UserPASS"];

    if (doc.containsKey("pdx_panel_refresh"))
        pdx_panel_refresh = doc["pdx_panel_refresh"];

    if (doc.containsKey("pdx_panel_data_periodic"))
        pdx_panel_data_periodic = doc["pdx_panel_data_periodic"];

    if (doc.containsKey("pdx_panel_data_period"))
        pdx_panel_data_period = doc["pdx_panel_data_period"];

    if (doc.containsKey("pdx_panel_refresh_time"))
        pdx_panel_refresh_time = doc["pdx_panel_refresh_time"];

    doc.clear();

    return true;
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

void fs_format()
{

    if (!LittleFS.begin())
        return;

    LittleFS.format();
    LittleFS.end();

    return;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######     ##      ## #### ######## ####
// ##    ##    ##  ##  ##  ##  ##        ##
// ##          ##  ##  ##  ##  ##        ##
//  ######     ##  ##  ##  ##  ######    ##
//       ##    ##  ##  ##  ##  ##        ##
// ##    ##    ##  ##  ##  ##  ##        ##
//  ######      ###  ###  #### ##       ####

const char *fs_save_wifi()
{
    if (!LittleFS.begin())
        return littlefs_failure;

    DynamicJsonDocument doc(512);

    doc["SSIDa"] = SSIDa;
    doc["PASSa"] = PASSa;
    doc["SSIDb"] = SSIDb;
    doc["PASSb"] = PASSb;

    yield();

    // serialize & write config
    File configFile = LittleFS.open(wifi_cfg_file, "w");
    if (!configFile)
        return file_write_failure;

    serializeJson(doc, configFile);
    configFile.close();

    doc.clear();

    return file_write_success;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######     ########  ######## ##     ## ####  ######  ########
// ##    ##    ##     ## ##       ##     ##  ##  ##    ## ##
// ##          ##     ## ##       ##     ##  ##  ##       ##
//  ######     ##     ## ######   ##     ##  ##  ##       ######
//       ##    ##     ## ##        ##   ##   ##  ##       ##
// ##    ##    ##     ## ##         ## ##    ##  ##    ## ##
//  ######     ########  ########    ###    ####  ######  ########

const char *fs_save_device()
{
    if (!LittleFS.begin())
        return littlefs_failure;

    DynamicJsonDocument doc(512);

    doc["LOC"] = LOC;
    doc["TIP"] = TIP;
    doc["NAME"] = NAME;
    doc["XTRA"] = XTRA;

    doc["update_url"] = update_url;

    doc["heartbeat"] = heartbeat;
    doc["heartbeat_minutes"] = heartbeat_minutes;

    doc["start_webserver"] = start_webserver;

    yield();

    // serialize & write config
    File configFile = LittleFS.open(dev_cfg_file, "w");
    if (!configFile)
        return file_write_failure;

    serializeJson(doc, configFile);
    configFile.close();

    doc.clear();

    return file_write_success;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######     ##     ##  #######  ######## ########
// ##    ##    ###   ### ##     ##    ##       ##
// ##          #### #### ##     ##    ##       ##
//  ######     ## ### ## ##     ##    ##       ##
//       ##    ##     ## ##  ## ##    ##       ##
// ##    ##    ##     ## ##    ##     ##       ##
//  ######     ##     ##  ##### ##    ##       ##

const char *fs_save_mqtt()
{
    if (!LittleFS.begin())
        return littlefs_failure;

    DynamicJsonDocument doc(512);

    doc["MQTT_HOST"] = MQTT_HOST;
    doc["MQTT_PORT"] = MQTT_PORT;
    doc["MQTT_USER"] = MQTT_USER;
    doc["MQTT_PASS"] = MQTT_PASS;
    doc["WILL"] = WILL;
    doc["PUB"] = PUB;
    doc["SUB"] = SUB;
    doc["STAT"] = STAT;
    doc["start_mqtt"] = start_mqtt;

    yield();

    // serialize & write config
    File configFile = LittleFS.open(mqtt_cfg_file, "w");
    if (!configFile)
        return file_write_failure;

    serializeJson(doc, configFile);
    configFile.close();

    doc.clear();

    return file_write_success;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######     ########     ###    ########     ###    ########   #######  ##     ##
// ##    ##    ##     ##   ## ##   ##     ##   ## ##   ##     ## ##     ##  ##   ##
// ##          ##     ##  ##   ##  ##     ##  ##   ##  ##     ## ##     ##   ## ##
//  ######     ########  ##     ## ########  ##     ## ##     ## ##     ##    ###
//       ##    ##        ######### ##   ##   ######### ##     ## ##     ##   ## ##
// ##    ##    ##        ##     ## ##    ##  ##     ## ##     ## ##     ##  ##   ##
//  ######     ##        ##     ## ##     ## ##     ## ########   #######  ##     ##

const char *fs_save_paradox()
{
    if (!LittleFS.begin())
        return littlefs_failure;

    DynamicJsonDocument doc(512);

    doc["start_paradox"] = start_paradox;
    doc["UserID"] = UserID;
    doc["UserPASS"] = UserPASS;
    doc["pdx_panel_refresh"] = pdx_panel_refresh;
    doc["pdx_panel_data_periodic"] = pdx_panel_data_periodic;
    doc["pdx_panel_data_period"] = pdx_panel_data_period;
    doc["pdx_panel_refresh_time"] = pdx_panel_refresh_time;

    yield();

    // serialize & write config
    File configFile = LittleFS.open(paradox_cfg_file, "w");
    if (!configFile)
        return file_write_failure;

    serializeJson(doc, configFile);
    configFile.close();

    doc.clear();

    return file_write_success;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////