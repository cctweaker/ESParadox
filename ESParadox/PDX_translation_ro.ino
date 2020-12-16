#if LNG == 1
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######                                 ###
//  #     #   ##   #    # ###### #          #  #    # ######  ####
//  #     #  #  #  ##   # #      #          #  ##   # #      #    #
//  ######  #    # # #  # #####  #          #  # #  # #####  #    #
//  #       ###### #  # # #      #          #  #  # # #      #    #
//  #       #    # #   ## #      #          #  #   ## #      #    #
//  #       #    # #    # ###### ######    ### #    # #       ####

void translate_panel_info()
{

    DynamicJsonDocument doc(256);

    switch (PPI)
    {
    case 0x15:
        doc["Model"] = F("SP5500");
        break;
    case 0x16:
        doc["Model"] = F("SP6000");
        break;
    case 0x17:
        doc["Model"] = F("SP7000");
        break;
    case 0x40:
        doc["Model"] = F("MG5000");
        break;
    case 0x41:
        doc["Model"] = F("MG5050");
        break;
    }

    doc["Firmware"] = String(PFV) + "." + String(PFB) + "." + String(PFR);

    sprintf(mqtt_tx, "%02X%02X", PPID1, PPID2);
    doc["PPID"] = mqtt_tx;

    if (PPI > 0x20)
    {
        doc["Transceiver"]["Family"] = String(TF);
        doc["Transceiver"]["HW"] = String(THR);
        doc["Transceiver"]["FW"] = String(TFV) + "." + String(TFB) + "." + String(TFR);
        doc["Transceiver"]["Noise floor lvl."] = String(TNFL);
        doc["Transceiver"]["Noise floor"] = NFLTH ? "high" : "low, ok";
        doc["Transceiver"]["Constant carrier"] = CCD ? "warning" : "absent, ok";
    }

    serializeJson(doc, mqtt_tx);
    doc.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######                                   ###
//  #     #   ##   #    # ###### #          #   #     ##### #####   ####  #    # #####  #      ######  ####
//  #     #  #  #  ##   # #      #         #     #      #   #    # #    # #    # #    # #      #      #
//  ######  #    # # #  # #####  #         #     #      #   #    # #    # #    # #####  #      #####   ####
//  #       ###### #  # # #      #         #     #      #   #####  #    # #    # #    # #      #           #
//  #       #    # #   ## #      #          #   #       #   #   #  #    # #    # #    # #      #      #    #
//  #       #    # #    # ###### ######      ###        #   #    #  ####   ####  #####  ###### ######  ####

void translate_panel_0_trouble()
{
    strcpy(mqtt_tx, "no trouble");

    DynamicJsonDocument doc(256);

    if (pdx_rx_buffer[4] || pdx_rx_buffer[5] || pdx_rx_buffer[6] || pdx_rx_buffer[7] || pdx_rx_buffer[8]) // report only if events exist
    {
        // PANEL 0 BYTE 4
        if (bitRead(pdx_rx_buffer[4], 7))
            doc.add(F("Ceas dereglat"));
        if (bitRead(pdx_rx_buffer[4], 6))
            doc.add(F("Alarma foc globala"));
        if (bitRead(pdx_rx_buffer[4], 5))
            doc.add(F("Acces neautorizat global"));
        if (bitRead(pdx_rx_buffer[4], 4))
            doc.add(F("Acces neautorizat zona global"));
        if (bitRead(pdx_rx_buffer[4], 3))
            doc.add(F("Problema comunicare globala"));
        if (bitRead(pdx_rx_buffer[4], 2))
            doc.add(F("Problema sirena globala"));
        if (bitRead(pdx_rx_buffer[4], 1))
            doc.add(F("Problema alimentare globala"));
        if (bitRead(pdx_rx_buffer[4], 0))
            doc.add(F("Bateria slaba transmitator RF global"));

        // PANEL 0 BYTE 5
        if (bitRead(pdx_rx_buffer[5], 7))
            doc.add(F("Interferenta RF"));
        if (bitRead(pdx_rx_buffer[5], 1))
            doc.add(F("Supervizare globala modul"));
        if (bitRead(pdx_rx_buffer[5], 0))
            doc.add(F("Supervizare zona globala"));

        // PANEL 0 BYTE 6
        if (bitRead(pdx_rx_buffer[6], 6))
            doc.add(F("Baterie slaba repetor RF"));
        if (bitRead(pdx_rx_buffer[6], 5))
            doc.add(F("Pierdere alimentare AC repetor RF"));
        if (bitRead(pdx_rx_buffer[6], 4))
            doc.add(F("Baterie slaba tastatura fara fir"));
        if (bitRead(pdx_rx_buffer[6], 3))
            doc.add(F("Pierdere alimentare AC tastatura fara fir"));
        if (bitRead(pdx_rx_buffer[6], 2))
            doc.add(F("Supraincarcare consum auxiliar"));
        if (bitRead(pdx_rx_buffer[6], 1))
            doc.add(F("Pierdere alimentare AC"));
        if (bitRead(pdx_rx_buffer[6], 0))
            doc.add(F("Baterie slaba / defecta"));

        // PANEL 0 BYTE 7
        if (bitRead(pdx_rx_buffer[7], 1))
            doc.add(F("Supraincarcare iesire sirena"));
        if (bitRead(pdx_rx_buffer[7], 0))
            doc.add(F("Iesire sirena deconectata"));

        // PANEL 0 BYTE 8
        if (bitRead(pdx_rx_buffer[8], 5))
            doc.add(F("Eroare comunicare computer"));
        if (bitRead(pdx_rx_buffer[8], 4))
            doc.add(F("Eroare comunicare voce"));
        if (bitRead(pdx_rx_buffer[8], 3))
            doc.add(F("Eroare comunicare pager"));
        if (bitRead(pdx_rx_buffer[8], 2))
            doc.add(F("Eroare comunicare telefon centrala 2"));
        if (bitRead(pdx_rx_buffer[8], 1))
            doc.add(F("Eroare comunicare telefon centrala 1"));
        if (bitRead(pdx_rx_buffer[8], 0))
            doc.add(F("Probleme linie telefon"));

        serializeJson(doc, mqtt_tx);
        doc.clear();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######                                   ###
//  #     #   ##   #    # ###### #          #   #     #    #   ##   #      #    # ######  ####
//  #     #  #  #  ##   # #      #         #     #    #    #  #  #  #      #    # #      #
//  ######  #    # # #  # #####  #         #     #    #    # #    # #      #    # #####   ####
//  #       ###### #  # # #      #         #     #    #    # ###### #      #    # #           #
//  #       #    # #   ## #      #          #   #      #  #  #    # #      #    # #      #    #
//  #       #    # #    # ###### ######      ###        ##   #    # ######  ####  ######  ####

void translate_panel_0_values()
{
    DynamicJsonDocument doc(256);

    doc["Tensiune AC"] = pdx_rx_buffer[15];
    doc["Tensiune DC"] = pdx_rx_buffer[16];
    doc["Tensiune baterie"] = pdx_rx_buffer[17];
    doc["Zgomot RF bazal"] = pdx_rx_buffer[18];

    serializeJson(doc, mqtt_tx);
    doc.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######                                 ### ######
//  #     #   ##   #    # ###### #          #  #     #  ####
//  #     #  #  #  ##   # #      #          #  #     # #
//  ######  #    # # #  # #####  #          #  #     #  ####
//  #       ###### #  # # #      #          #  #     #      #
//  #       #    # #   ## #      #          #  #     # #    #
//  #       #    # #    # ###### ######    ### ######   ####

void translate_panel_ids(uint8_t position, uint8_t max)
{
    uint8_t i = 0;
    uint8_t b = 0;

    DynamicJsonDocument doc(256);

    for (i = 0; i < max; i++)
        for (b = 0; b < 8; b++)
            if (bitRead(pdx_rx_buffer[position + i], b))
                doc.add((i * 8 + b + 1));

    serializeJson(doc, mqtt_tx);
    doc.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######
//  #     #   ##   #####  ##### # ##### #  ####  #    #     ####  #####   ##   ##### #    #  ####
//  #     #  #  #  #    #   #   #   #   # #    # ##   #    #        #    #  #    #   #    # #
//  ######  #    # #    #   #   #   #   # #    # # #  #     ####    #   #    #   #   #    #  ####
//  #       ###### #####    #   #   #   # #    # #  # #         #   #   ######   #   #    #      #
//  #       #    # #   #    #   #   #   # #    # #   ##    #    #   #   #    #   #   #    # #    #
//  #       #    # #    #   #   #   #   #  ####  #    #     ####    #   #    #   #    ####   ####

void translate_partition_status(uint8_t position)
{
    DynamicJsonDocument doc(256);

    if (bitRead(pdx_rx_buffer[position], 7))
        doc.add(F("Alarma puls (alarma foc)."));
    if (bitRead(pdx_rx_buffer[position], 6))
        doc.add(F("Alarma cu sirena."));
    if (bitRead(pdx_rx_buffer[position], 5))
        doc.add(F("Alarma muta."));
    if (bitRead(pdx_rx_buffer[position], 4))
        doc.add(F("Indicare optica alarma."));
    if (bitRead(pdx_rx_buffer[position], 3))
        doc.add(F("N/U"));
    if (bitRead(pdx_rx_buffer[position], 2))
        doc.add(F("Stay Arm"));
    if (bitRead(pdx_rx_buffer[position], 1))
        doc.add(F("Sleep Arm"));
    if (bitRead(pdx_rx_buffer[position], 0))
        doc.add(F("Arm (Stay/Sleep/Full)"));

    //////////

    if (bitRead(pdx_rx_buffer[position + 1], 7))
        doc.add(F("Sirena activa"));
    if (bitRead(pdx_rx_buffer[position + 1], 6))
        doc.add(F("Auto-armare"));
    if (bitRead(pdx_rx_buffer[position + 1], 5))
        doc.add(F("Intarziere inchidere"));
    if (bitRead(pdx_rx_buffer[position + 1], 4))
        doc.add(F("Intarziere intellizone"));
    if (bitRead(pdx_rx_buffer[position + 1], 3))
        doc.add(F("Zona / zone evitate"));
    if (bitRead(pdx_rx_buffer[position + 1], 2))
        doc.add(F("Alarma/e in memorie"));
    if (bitRead(pdx_rx_buffer[position + 1], 1))
        doc.add(F("Intarziere intrare"));
    if (bitRead(pdx_rx_buffer[position + 1], 0))
        doc.add(F("Intarziere iesire"));

    //////////

    if (bitRead(pdx_rx_buffer[position + 2], 7))
        doc.add(F("Alarma paramedic"));
    if (bitRead(pdx_rx_buffer[position + 2], 6))
        doc.add(F("Reserved"));
    if (bitRead(pdx_rx_buffer[position + 2], 5))
        doc.add(F("Armare cu telecomanda"));
    if (bitRead(pdx_rx_buffer[position + 2], 4))
        doc.add(F("Intarziere transmisie expirata"));
    if (bitRead(pdx_rx_buffer[position + 2], 3))
        doc.add(F("Intarziere sirena expirata"));
    if (bitRead(pdx_rx_buffer[position + 2], 2))
        doc.add(F("Intarziere intrare expirata"));
    if (bitRead(pdx_rx_buffer[position + 2], 1))
        doc.add(F("Intarziere iesire expirata"));
    if (bitRead(pdx_rx_buffer[position + 2], 0))
        doc.add(F("Intarziere intellizone expirata"));

    //////////

    if (bitRead(pdx_rx_buffer[position + 3], 7))
        doc.add(F("N/U"));
    if (bitRead(pdx_rx_buffer[position + 3], 6))
        doc.add(F("Fereastra asteptare."));
    if (bitRead(pdx_rx_buffer[position + 3], 5))
        doc.add(F("N/U"));
    if (bitRead(pdx_rx_buffer[position + 3], 4))
        doc.add(F("Intarziare la distanta."));
    if (bitRead(pdx_rx_buffer[position + 3], 3))
        doc.add(F("N/U"));
    if (bitRead(pdx_rx_buffer[position + 3], 2))
        doc.add(F("Mod StayD activ."));
    if (bitRead(pdx_rx_buffer[position + 3], 1))
        doc.add(F("Armare fortata"));
    if (bitRead(pdx_rx_buffer[position + 3], 0))
        doc.add(F("Pregatit"));

    //////////

    serializeJson(doc, mqtt_tx);
    doc.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######
//  #     # ######   ##   #####  ######   ##   ##### ###### #####
//  #     # #       #  #  #    # #       #  #    #   #      #    #
//  ######  #####  #    # #    # #####  #    #   #   #####  #    #
//  #   #   #      ###### #####  #      ######   #   #      #####
//  #    #  #      #    # #      #      #    #   #   #      #   #
//  #     # ###### #    # #      ###### #    #   #   ###### #    #

//  #    #
//  #   #  ###### #   # #####    ##   #####
//  #  #   #       # #  #    #  #  #  #    #
//  ###    #####    #   #    # #    # #    #
//  #  #   #        #   #####  ###### #    #
//  #   #  #        #   #      #    # #    #
//  #    # ######   #   #      #    # #####

void translate_panel_repeater_keypad(uint8_t position, bool repeater)
{
    DynamicJsonDocument doc(256);

    if (repeater)
    {
        if (bitRead(pdx_rx_buffer[position], 7))
            doc.add(2);
        if (bitRead(pdx_rx_buffer[position], 6))
            doc.add(1);
    }
    else
    {
        if (bitRead(pdx_rx_buffer[position + 17], 7))
            doc.add(6);
        if (bitRead(pdx_rx_buffer[position + 17], 6))
            doc.add(5);

        if (bitRead(pdx_rx_buffer[position], 5))
            doc.add(6);
        if (bitRead(pdx_rx_buffer[position], 4))
            doc.add(5);
        if (bitRead(pdx_rx_buffer[position], 3))
            doc.add(4);
        if (bitRead(pdx_rx_buffer[position], 2))
            doc.add(3);
        if (bitRead(pdx_rx_buffer[position], 1))
            doc.add(2);
        if (bitRead(pdx_rx_buffer[position], 0))
            doc.add(1);
    }

    serializeJson(doc, mqtt_tx);
    doc.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  #######
//       #   ####  #    # ######     ####  #####   ##   ##### #    #  ####
//      #   #    # ##   # #         #        #    #  #    #   #    # #
//     #    #    # # #  # #####      ####    #   #    #   #   #    #  ####
//    #     #    # #  # # #              #   #   ######   #   #    #      #
//   #      #    # #   ## #         #    #   #   #    #   #   #    # #    #
//  #######  ####  #    # ######     ####    #   #    #   #    ####   ####

void translate_zone_status(uint8_t position)
{
    bool bit5 = false;
    bool bit4 = false;

    DynamicJsonDocument doc(256);

    if (bitRead(pdx_rx_buffer[position], 7))
        doc.add(F("Zona a fost in alarma"));

    if (bitRead(pdx_rx_buffer[position], 6))
        doc.add(F("Zone este in alarma"));

    if (bitRead(pdx_rx_buffer[position], 5))
        bit5 = true;
    if (bitRead(pdx_rx_buffer[position], 4))
        bit4 = true;

    if (bit5 && bit4)
        doc.add(F("In intarziere foc"));
    if (!bit5 && bit4)
        doc.add(F("In intarziere intrare"));
    if (bit5 && !bit4)
        doc.add(F("In intarziare intellizone"));
    if (!bit5 && !bit4)
        doc.add(F("Fara intarziere"));

    if (bitRead(pdx_rx_buffer[position], 3))
        doc.add(F("Zona evitata"));
    if (bitRead(pdx_rx_buffer[position], 2))
        doc.add(F("Zona dezactivata"));
    if (bitRead(pdx_rx_buffer[position], 1))
        doc.add(F("Zona in intarziere transmitere"));
    if (bitRead(pdx_rx_buffer[position], 0))
        doc.add(F("Zone a fost evitata"));

    serializeJson(doc, mqtt_tx);
    doc.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  #######
//  #       #    # ###### #    # #####
//  #       #    # #      ##   #   #
//  #####   #    # #####  # #  #   #
//  #       #    # #      #  # #   #
//  #        #  #  #      #   ##   #
//  #######   ##   ###### #    #   #

void translate_event()
{
    uint8_t i = 0;

    bool is_zone = false;
    bool is_partition = false;
    bool is_bell = false;
    bool is_module = false;
    bool is_arm = false;
    bool is_alarm = false;
    bool is_trouble = false;

    char topic[128];

    DynamicJsonDocument doc(256);

    // event date/time
    sprintf(topic, "%02d%02d-%02d-%02d %02d:%02d", pdx_rx_buffer[1], pdx_rx_buffer[2], pdx_rx_buffer[3], pdx_rx_buffer[4], pdx_rx_buffer[5], pdx_rx_buffer[6]);
    doc["dt"] = topic;

    // event group
    doc["eg#"] = pdx_rx_buffer[7];

    // event subgroup
    doc["sg#"] = pdx_rx_buffer[8];

    // event partition
    doc["prt"] = pdx_rx_buffer[9];

    // device serial number
    if (pdx_rx_buffer[10] || pdx_rx_buffer[11] || pdx_rx_buffer[12] || pdx_rx_buffer[13])
    {
        sprintf(topic, "%02x%02x%02x%02x", pdx_rx_buffer[10], pdx_rx_buffer[11], pdx_rx_buffer[12], pdx_rx_buffer[13]);
        doc["sn#"] = topic;
    }

    // device label (name)
    for (i = 15; i <= 30; i++)
        topic[i - 15] = (pdx_rx_buffer[i] == 0 ? 0x20 : pdx_rx_buffer[i]);
    topic[16] = 0;
    for (i = 15; i >= 0; i--)
    {
        if (topic[i] != 0x20)
            break;
        else
            topic[i] = 0;
    }

    doc["label"] = topic;

    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //

    switch (pdx_rx_buffer[7])
    {
    case 0:
        doc["event"] = F("Zona OK");
        is_zone = true;
        break;
        ////////////////////////////////////////////

    case 1:
        doc["event"] = F("Zona deschisa");
        is_zone = true;
        break;
        ////////////////////////////////////////////

    case 2:
        is_partition = true;

        switch (pdx_rx_buffer[8])
        {
        case 2:
            doc["event"] = F("Alarma silentioasa");
            break;
            ///////////
        case 3:
            doc["event"] = F("Alarma buzzer");
            break;
            ///////////
        case 4:
            doc["event"] = F("Alarma continua");
            break;
            ///////////
        case 5:
            doc["event"] = F("Alarma pulsatorie");
            break;
            ///////////
        case 6:
            doc["event"] = F("Stroboscop");
            break;
            ///////////
        case 7:
            doc["event"] = F("Alarma oprita");
            break;
            ///////////
        case 8:
            doc["event"] = F("Semnal acustic tastatura pornit");
            break;
            ///////////
        case 9:
            doc["event"] = F("Semnal acustic tastatura oprit");
            break;
            ///////////
        case 10:
            doc["event"] = F("Inceput de baza");
            break;
            ///////////
        case 11:
            doc["event"] = F("Dezarmare partitie");
            break;
            ///////////
        case 12:
            doc["event"] = F("Armare partitie");
            break;
            ///////////
        case 13:
            doc["event"] = F("Intarziere intrare pornita");
            break;
            ///////////
        case 14:
            doc["event"] = F("Intarziere iesire pornita");
            sprintf(topic, "%s%s%s%s%s/arm", LOC, TIP, NAME, XTRA, PUB);
            client.publish(topic, "1", true, 0);
            break;
            ///////////
        case 15:
            doc["event"] = F("Intarziere pre-alarma");
            break;
            ///////////
        case 16:
            doc["event"] = F("Confirmare raport");
            break;
            ///////////
        case 99:
            doc["event"] = F("Orice eveniment stare partitie");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 3:
        is_bell = true;

        switch (pdx_rx_buffer[8])
        {
        case 0:
            doc["event"] = F("Sirena oprita");
            break;
            ///////////
        case 1:
            doc["event"] = F("Sirena pornita");
            break;
            ///////////
        case 2:
            doc["event"] = F("Sirena atentionare armare");
            break;
            ///////////
        case 3:
            doc["event"] = F("Sirena atentionare dezarmare");
            break;
            ///////////
        case 99:
            doc["event"] = F("Orice eveniment stare sirena");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 6:

        switch (pdx_rx_buffer[8])
        {
        case 0:
            doc["event"] = F("Probleme linie telefonica");
            break;
            ///////////
        case 1:
            doc["event"] = F("S-a apasat ENTER + CLEAR + POWER");
            break;
            ///////////
        case 2:
            doc["event"] = F("N/A");
            break;
            ///////////
        case 3:
            doc["event"] = F("Armare in mod Stay");
            break;
            ///////////
        case 4:
            doc["event"] = F("Armare in mod Sleep");
            break;
            ///////////
        case 5:
            doc["event"] = F("Armare fortata");
            break;
            ///////////
        case 6:
            doc["event"] = F("Armare completa in mod Stay");
            break;
            ///////////
        case 7:
            doc["event"] = F("Eroare comunicare PC");
            break;
            ///////////
        case 8:
            doc["event"] = F("Apasare tasta utilitara 1 (1+2)");
            break;
            ///////////
        case 9:
            doc["event"] = F("Apasare tasta utilitara 2 (4+5)");
            break;
            ///////////
        case 10:
            doc["event"] = F("Apasare tasta utilitara 3 (7+8)");
            break;
            ///////////
        case 11:
            doc["event"] = F("Apasare tasta utilitara 4 (2+3)");
            break;
            ///////////
        case 12:
            doc["event"] = F("Apasare tasta utilitara 5 (5+6)");
            break;
            ///////////
        case 13:
            doc["event"] = F("Apasare tasta utilitara 6 (8+9)");
            break;
            ///////////
        case 14:
            doc["event"] = F("Alarma acces neautorizat");
            break;
            ///////////
        case 15:
            doc["event"] = F("Alarma pierdere supervizare");
            break;
            ///////////
        case 20:
            doc["event"] = F("Armare completa mod Sleep");
            break;
            ///////////
        case 21:
            doc["event"] = F("Firmware upgrade");
            break;
            ///////////
        case 23:
            doc["event"] = F("Mod StayD activat");
            break;
            ///////////
        case 24:
            doc["event"] = F("Mod StayD dezactivat");
            break;
            ///////////
        case 25:
            doc["event"] = F("Schimbare inregistrare IP");
            break;
            ///////////
        case 26:
            doc["event"] = F("Schimbare inregistrare GPRS");
            break;
            ///////////
        case 99:
            doc["event"] = F("Orice eveniment neraportabil");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 8:
        sprintf_P(topic, PSTR("Telecomanda %d, apasare buton B"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 9:
        sprintf_P(topic, PSTR("Telecomanda %d, apasare buton C"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 10:
        sprintf_P(topic, PSTR("Telecomanda %d, apasare buton D"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 11:
        sprintf_P(topic, PSTR("Telecomanda %d, apasare buton E"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 12:
        is_zone = true;
        doc["event"] = F("Pornire la rece zona RF");
        break;
        ////////////////////////////////////////////

    case 13:
        is_module = true;
        sprintf_P(topic, PSTR("Pornire la rece modul RF %d"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 14:
        sprintf_P(topic, PSTR("Programare ocolire utilizator %d"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 15:
        sprintf_P(topic, PSTR("Utilizator %d iesire activata prin cod"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 16:
        is_zone = true;
        doc["event"] = F("Semnal mentenanta senzor fum RF");
        break;
        ////////////////////////////////////////////

    case 17:
        is_zone = true;
        doc["event"] = F("Intarziere transmisie alarma zona");
        break;
        ////////////////////////////////////////////

    case 18:
        is_zone = true;
        doc["event"] = F("Semnal zona slab 1");
        break;
        ////////////////////////////////////////////

    case 19:
        is_zone = true;
        doc["event"] = F("Semnal zona slab  2");
        break;
        ////////////////////////////////////////////

    case 20:
        is_zone = true;
        doc["event"] = F("Semnal zona slab  3");
        break;
        ////////////////////////////////////////////

    case 21:
        is_zone = true;
        doc["event"] = F("Semnal zona slab  4");
        break;
        ////////////////////////////////////////////

    case 22:
        sprintf_P(topic, PSTR("Telecomanda %d, optiune buton 5"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 23:
        sprintf_P(topic, PSTR("Telecomanda %d, optiune buton 6"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 24:
        is_zone = true;
        doc["event"] = F("Intarziere foc pornita");
        break;
        ////////////////////////////////////////////

    case 26:
        switch (pdx_rx_buffer[8])
        {
            ///////////
        case 1:
            doc["event"] = F("Acces soft: WinLoad/Babyware direct");
            break;
            ///////////
        case 2:
            doc["event"] = F("Acces soft: WinLoad/Babyware modul IP");
            break;
            ///////////
        case 3:
            doc["event"] = F("Acces soft: WinLoad/Babyware modul GSM");
            break;
            ///////////
        case 4:
            doc["event"] = F("Acces soft: WinLoad/Babyware modem");
            break;
            ///////////
        case 9:
            doc["event"] = F("Acces soft: IP100 direct");
            break;
            ///////////
        case 10:
            doc["event"] = F("Acces soft: VDMP3 direct");
            break;
            ///////////
        case 11:
            doc["event"] = F("Acces soft: Voce prin modul GSM");
            break;
            ///////////
        case 12:
            doc["event"] = F("Acces soft: la distanta");
            break;
            ///////////
        case 13:
            doc["event"] = F("Acces soft: SMS prin modul GSM");
            break;
            ///////////
        case 99:
            doc["event"] = F("Acces soft: orice tip de acces");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 27:
        switch (pdx_rx_buffer[8])
        {
            ///////////
        case 0:
            doc["event"] = F("Un modul a fost adaugat");
            break;
            ///////////
        case 1:
            doc["event"] = F("Un modul a fost indepartat");
            break;
            ///////////
        case 2:
            doc["event"] = F("Eroare comunicare bidirectionala modul RF");
            break;
            ///////////
        case 3:
            doc["event"] = F("Restabilire comunicare bidirectionala modul RF");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 28:
        is_zone = true;
        doc["event"] = F("Prola StayD confirmata");
        break;
        ////////////////////////////////////////////

    case 29:
        is_arm = true;
        sprintf_P(topic, PSTR("Armare utilizator %d"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 30:
        is_arm = true;

        switch (pdx_rx_buffer[8])
        {
            ///////////
        case 0:
            doc["event"] = F("Auto-armare (cronometru/fara miscare)");
            break;
            ///////////
        case 1:
            doc["event"] = F("Inchidere tarzie");
            break;
            ///////////
        case 2:
            doc["event"] = F("Armare lipsa miscare");
            break;
            ///////////
        case 3:
            doc["event"] = F("Armare partiala");
            break;
        ///////////
        case 4:
            doc["event"] = F("Armare rapida");
            break;
        ///////////
        case 5:
            doc["event"] = F("Armare prin WinLoad / BabyWare");
            break;
        ///////////
        case 6:
            doc["event"] = F("Armare din buton");
            break;
            ///////////
        case 99:
            doc["event"] = F("Orice armare speciala");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 31:
        is_arm = true;
        sprintf_P(topic, PSTR("Dezarmare utilizator %d"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 32:
        is_arm = true;
        sprintf_P(topic, PSTR("Dezarmare dupa alarma utilizator %d"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 33:
        is_arm = true;
        sprintf_P(topic, PSTR("Alarma anulata utilizator %d"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 34:
        is_arm = true;

        switch (pdx_rx_buffer[8])
        {
            ///////////
        case 0:
            doc["event"] = F("Auto-armare anulata (cronometru/fara miscare)");
            break;
            ///////////
        case 1:
            doc["event"] = F("Dezarmare prin WinLoad / BabyWare");
            break;
            ///////////
        case 2:
            doc["event"] = F("Dezarmare prin WinLoad / BabyWare dupa alarma");
            break;
            ///////////
        case 3:
            doc["event"] = F("Alarma anulata prin WinLoad / BabyWare");
            break;
        ///////////
        case 4:
            doc["event"] = F("Alarma paramedic anulata");
            break;
        ///////////
        case 5:
            doc["event"] = F("Dezarmare din buton");
            break;
        ///////////
        case 6:
            doc["event"] = F("Dezarmare din buton dupa alarma");
            break;
        ///////////
        case 7:
            doc["event"] = F("Alarma anulata din buton");
            break;
            ///////////
        case 99:
            doc["event"] = F("Orice dezarmare speciala");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 35:
        is_zone = true;
        doc["event"] = F("Zona ocolita");
        break;
        ////////////////////////////////////////////

    case 36:
        is_zone = true;
        doc["event"] = F("Zona in alarma");
        break;
        ////////////////////////////////////////////

    case 37:
        is_zone = true;
        doc["event"] = F("Alarma foc");
        break;
        ////////////////////////////////////////////

    case 38:
        is_zone = true;
        doc["event"] = F("Alarma zona restabilita");
        break;
        ////////////////////////////////////////////

    case 39:
        is_zone = true;
        doc["event"] = F("Alarma foc restabilita");
        break;
        ////////////////////////////////////////////

    case 40:
        is_alarm = true;

        switch (pdx_rx_buffer[8])
        {
            ///////////
        case 0:
            doc["event"] = F("Panica urgenta");
            break;
            ///////////
        case 1:
            doc["event"] = F("Panica medicala");
            break;
            ///////////
        case 2:
            doc["event"] = F("Panica foc");
            break;
            ///////////
        case 3:
            doc["event"] = F("Inchidere recenta");
            break;
        ///////////
        case 4:
            doc["event"] = F("Inchidere globala");
            break;
        ///////////
        case 5:
            doc["event"] = F("Alarma constrangere");
            break;
        ///////////
        case 6:
            doc["event"] = F("Tastatura blocata");
            break;
            ///////////
        case 99:
            doc["event"] = F("Orice eveniment alarma speciala");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 41:
        is_zone = true;
        doc["event"] = F("Zona dezactivata");
        break;
        ////////////////////////////////////////////

    case 42:
        is_zone = true;
        doc["event"] = F("Zona alterata");
        break;
        ////////////////////////////////////////////

    case 43:
        is_zone = true;
        doc["event"] = F("Zona restabilire alterare");
        break;
        ////////////////////////////////////////////

    case 44:
        is_trouble = true;

        switch (pdx_rx_buffer[8])
        {
        case 1:
            doc["event"] = F("Lipsa alimentare AC");
            break;
            ///////////
        case 2:
            doc["event"] = F("Eroare baterie");
            break;
            ///////////
        case 3:
            doc["event"] = F("Consum auxiliar excesiv");
            break;
            ///////////
        case 4:
            doc["event"] = F("Consum sirena excesiv");
            break;
        ///////////
        case 5:
            doc["event"] = F("Sirena deconectata");
            break;
        ///////////
        case 6:
            doc["event"] = F("Pierdere ceas");
            break;
        ///////////
        case 7:
            doc["event"] = F("Eroare foc");
            break;
        ///////////
        case 8:
            doc["event"] = F("Eroare comunicare telefonica centrala 1");
            break;
        ///////////
        case 9:
            doc["event"] = F("Eroare comunicare telefonica centrala 2");
            break;
        ///////////
        case 11:
            doc["event"] = F("Eroare comunicare raport vocal");
            break;
        ///////////
        case 12:
            doc["event"] = F("Bruiaj RF");
            break;
        ///////////
        case 13:
            doc["event"] = F("Bruiaj GSM");
            break;
        ///////////
        case 14:
            doc["event"] = F("Fara semnal GSM");
            break;
        ///////////
        case 15:
            doc["event"] = F("Supervizare GSM pierduta");
            break;
        ///////////
        case 16:
            doc["event"] = F("Eroare comunicare IP1 (GPRS)");
            break;
        ///////////
        case 17:
            doc["event"] = F("Eroare comunicare IP2 (GPRS)");
            break;
        ///////////
        case 18:
            doc["event"] = F("Modul IP neconectabil");
            break;
        ///////////
        case 19:
            doc["event"] = F("Supervizare modul IP pierduta");
            break;
        ///////////
        case 20:
            doc["event"] = F("Eroare comunicare IP1 (IP)");
            break;
        ///////////
        case 21:
            doc["event"] = F("Eroare comunicare IP2 (IP)");
            break;
            ///////////
        case 99:
            doc["event"] = F("Orice eveniment eroare nou");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 45:
        is_trouble = true;

        switch (pdx_rx_buffer[8])
        {
        case 0:
            doc["event"] = F("Linie telephone restaurata");
            break;
            ///////////
        case 1:
            doc["event"] = F("Lipsa AC restaurata");
            break;
            ///////////
        case 2:
            doc["event"] = F("Eroare baterie restaurata");
            break;
            ///////////
        case 3:
            doc["event"] = F("Suprasarcina auxiliara restaurata");
            break;
            ///////////
        case 4:
            doc["event"] = F("Suprasarcina sirena restaurata");
            break;
        ///////////
        case 5:
            doc["event"] = F("Deconectare sirena restaurata");
            break;
        ///////////
        case 6:
            doc["event"] = F("Pierdere ceas restaurata");
            break;
        ///////////
        case 7:
            doc["event"] = F("Eroare foc restaurata");
            break;
        ///////////
        case 8:
            doc["event"] = F("Comunicare telefon centrala 1 restaurat");
            break;
        ///////////
        case 9:
            doc["event"] = F("Comunicare telefon centrala 2 restaurat");
            break;
        ///////////
        case 11:
            doc["event"] = F("Comunicare raport vocal restaurat");
            break;
        ///////////
        case 12:
            doc["event"] = F("Bruiaj RF restaurat");
            break;
        ///////////
        case 13:
            doc["event"] = F("Briaj GSM restaurat");
            break;
        ///////////
        case 14:
            doc["event"] = F("Lipsa serviciu GSM restaurat");
            break;
        ///////////
        case 15:
            doc["event"] = F("Supervizare GSM restaurata");
            break;
        ///////////
        case 16:
            doc["event"] = F("Eroare comunicare IP1 (GPRS) restaurata");
            break;
        ///////////
        case 17:
            doc["event"] = F("Eroare comunicare IP2 (GPRS) restaurata");
            break;
        ///////////
        case 18:
            doc["event"] = F("Serviciu modul IP restaurat");
            break;
        ///////////
        case 19:
            doc["event"] = F("Supervizare modul IP restaurata");
            break;
        ///////////
        case 20:
            doc["event"] = F("Eroare comunicare IP1 (IP) restaurata");
            break;
        ///////////
        case 21:
            doc["event"] = F("Eroare comunicare IP2 (IP) restaurata");
            break;
            ///////////
        case 99:
            doc["event"] = F("Orice eveniment restarurare eroare");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 46:
        is_trouble = true;

        switch (pdx_rx_buffer[8])
        {
            ///////////
        case 0:
            doc["event"] = F("Eroare comunicare modul bus");
            break;
            ///////////
        case 1:
            doc["event"] = F("Acces neautorizat modul bus");
            break;
            ///////////
        case 2:
            doc["event"] = F("Pierdere alimentare modul bus");
            break;
            ///////////
        case 3:
            doc["event"] = F("Eroare baterie modul bus");
            break;
            ///////////
        case 99:
            doc["event"] = F("Orice problema modul bus");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 47:
        is_trouble = true;

        switch (pdx_rx_buffer[8])
        {
            ///////////
        case 0:
            doc["event"] = F("Modul bus, comunicatie restaurata");
            break;
            ///////////
        case 1:
            doc["event"] = F("Modul bus, restaurare acces neautorizat");
            break;
            ///////////
        case 2:
            doc["event"] = F("Modul bus, restaurare alimentare AC");
            break;
            ///////////
        case 3:
            doc["event"] = F("Modul bus, restaurare eroare baterie");
            break;
            ///////////
        case 99:
            doc["event"] = F("Modul bus, restaurare orice problema");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 48:
        switch (pdx_rx_buffer[8])
        {
            ///////////
        case 0:
            doc["event"] = F("Pornire sistem");
            break;
            ///////////
        case 1:
            doc["event"] = F("Test raportare");
            break;
            ///////////
        case 2:
            doc["event"] = F("Autentificare software");
            break;
            ///////////
        case 3:
            doc["event"] = F("Deconectare software");
            break;
            ///////////
        case 4:
            doc["event"] = F("Intalator in mod programare");
            break;
            ///////////
        case 5:
            doc["event"] = F("Instalator deconectat");
            break;
            ///////////
        case 6:
            doc["event"] = F("Mentenanta in mod programare");
            break;
            ///////////
        case 7:
            doc["event"] = F("Mentenanta decontectata");
            break;
            ///////////
        case 8:
            doc["event"] = F("Intarziere deliventa inchidere terminata");
            break;
            ///////////
        case 99:
            doc["event"] = F("Orice eveniment special");
            break;
        }
        break;
        ////////////////////////////////////////////

    case 49:
        is_zone = true;
        doc["event"] = F("Zona cu baterie descarcata");
        break;
        ////////////////////////////////////////////

    case 50:
        is_zone = true;
        doc["event"] = F("Zona cu baterie restaurata");
        break;
        ////////////////////////////////////////////

    case 51:
        is_zone = true;
        doc["event"] = F("Zona cu eroare supervizare");
        break;
        ////////////////////////////////////////////

    case 52:
        is_zone = true;
        doc["event"] = F("Zona cu supervizare restaurata");
        break;
        ////////////////////////////////////////////

    case 53:
        is_module = true;
        sprintf_P(topic, PSTR("Modul RF %d cu eroare supervizare"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 54:
        is_module = true;
        sprintf_P(topic, PSTR("Modul RF %d cu supervizare restaurata"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 55:
        is_module = true;
        sprintf_P(topic, PSTR("Modul RF %d cu acces neautorizat"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 56:
        is_module = true;
        sprintf_P(topic, PSTR("Modul RF %d restaurare acces neautorizat"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 57:
        is_alarm = true;
        sprintf_P(topic, PSTR("Alarma non-medicala (paramedic) utilizator %d"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;
        ////////////////////////////////////////////

    case 58:
        is_zone = true;
        doc["event"] = F("Zona fortata");
        break;
        ////////////////////////////////////////////

    case 59:
        is_zone = true;
        doc["event"] = F("Zona inclusa");
        break;
        ////////////////////////////////////////////

    case 64:
        sprintf_P(topic, PSTR("Stare sistem %d"), pdx_rx_buffer[8]);
        doc["event"] = topic;
        break;

    default:
        doc["event"] = F("Nedocumentat");
    }

    if (is_zone)
    {
        sprintf(topic, "%s%s%s%s%s/zone/%d", LOC, TIP, NAME, XTRA, PUB, pdx_rx_buffer[8]);
        client.publish(topic, (const char *)doc["event"], true, 0);

        sprintf(topic, "%s%s%s%s%s/zone/%d/label", LOC, TIP, NAME, XTRA, PUB, pdx_rx_buffer[8]);
        client.publish(topic, (const char *)doc["label"], true, 0);

        sprintf(topic, "%s%s%s%s%s/zone/%d/sn", LOC, TIP, NAME, XTRA, PUB, pdx_rx_buffer[8]);
        client.publish(topic, (const char *)doc["sn#"], true, 0);
    }

    if (is_partition)
    {
        sprintf(topic, "%s%s%s%s%s/partition/%d", LOC, TIP, NAME, XTRA, PUB, pdx_rx_buffer[9] + 1);
        client.publish(topic, (const char *)doc["event"], true, 0);

        if (pdx_rx_buffer[8] == 12) // arm partition
        {
            sprintf(topic, "%s%s%s%s%s/arm", LOC, TIP, NAME, XTRA, PUB);
            client.publish(topic, "1", true, 0);
        }
        if (pdx_rx_buffer[8] == 11) // disarm partition
        {
            sprintf(topic, "%s%s%s%s%s/arm", LOC, TIP, NAME, XTRA, PUB);
            client.publish(topic, "0", true, 0);
        }
    }

    if (is_bell)
    {
        sprintf(topic, "%s%s%s%s%s/bell", LOC, TIP, NAME, XTRA, PUB);
        client.publish(topic, (const char *)doc["event"], true, 0);
    }

    if (is_module)
    {
        sprintf(topic, "%s%s%s%s%s/module/%d", LOC, TIP, NAME, XTRA, PUB, pdx_rx_buffer[8]);
        client.publish(topic, (const char *)doc["event"], true, 0);
    }

    if (is_arm)
    {
        sprintf(topic, "%s%s%s%s%s/arm/event", LOC, TIP, NAME, XTRA, PUB);
        client.publish(topic, (const char *)doc["event"], true, 0);
    }

    if (is_alarm)
    {
        sprintf(topic, "%s%s%s%s%s/alarm/event", LOC, TIP, NAME, XTRA, PUB);
        client.publish(topic, (const char *)doc["event"], true, 0);
    }

    if (is_trouble)
    {
        sprintf(topic, "%s%s%s%s%s/trouble/event", LOC, TIP, NAME, XTRA, PUB);
        client.publish(topic, (const char *)doc["event"], true, 0);
    }

    serializeJson(doc, mqtt_tx);
    doc.clear();

    sprintf(topic, "%s%s%s%s%s/event", LOC, TIP, NAME, XTRA, PUB);
    client.publish(topic, mqtt_tx, true, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#endif