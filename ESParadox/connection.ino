void init_wifi()
{
    uint8_t i = 0;

    // initializare WIFI
    WiFi.hostname(HOSTNAME);
    WiFi.mode(WIFI_STA);

    // cautare retea
    int n = WiFi.scanNetworks();
    for (i = 0; i < n; ++i)
    {
        if (WiFi.SSID(i) == ssid)
        { // try current ssid
            WiFi.begin(ssid, pass);
            break;
        }
        if (WiFi.SSID(i) == ssidb)
        { // try backup ssid
            WiFi.begin(ssidb, passb);
            break;
        }
    }

    // astept conexiunea 20sec.
    i = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        i++;
        if (i > 200)
            ESP.restart();
    }
}

void ntp_time()
{
    // NTP time
    configTime(+3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    now = time(nullptr);
    while (now < 1510592825)
    {
        delay(200);
        now = time(nullptr);
    }
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
}

void certification()
{
    BearSSL::X509List cert(digicert);
    net.setTrustAnchors(&cert);
}