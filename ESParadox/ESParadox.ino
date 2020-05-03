#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <time.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <ESP8266httpUpdate.h>

#include "private.h"
#include "mqtt.h"
#include "paradox.h"

BearSSL::WiFiClientSecure net;
MQTTClient client;
time_t now;

void setup()
{
  // init serial for Paradox (UART0)
  Serial.begin(9600);
  Serial.flush();
  // move Paradox on UART1
  Serial.swap();
  // flush serial buffer
  flush_serial_buffer();

  // initialize wifi
  init_wifi();
  // get ntp time
  ntp_time();
  // take care of certs
  certification();
  // setup & connect mqtt
  mqtt_setup();

  // flush serial buffer before loop
  flush_serial_buffer();
}

void loop()
{
  if (!client.connected())
  {
    mqtt_connect();
  }

  client.loop();

  yield();

  if (command)
  {
    panel_command();
  }

  if (got_paradox_data)
  {
    send_to_mqtt();
  }

  yield();

  if (got_data)
  {
    check_data();
  }

  yield();

  if (Serial.available() >= paradox_message_length)
  {
    // read_data returns true if paradox_rx is filled
    got_data = read_data();
  }

  yield();

  if ((unsigned long)(millis() - last_heartbeat) > heartbeat_period)
  {
    last_heartbeat = millis();
    client.publish(MQTT_HB_TOPIC, panel_connected ? "C" : "nc", false, 0);
  }

  yield();

  if ((unsigned long)(millis() - pdlh) > PDHP)
  {
    if (panel_connected)
    {
      panel_status();
      send_panel_status_to_mqtt();
      pdlh = millis();
    }
  }

  yield();

  if (!panel_connected)
  {
    panel_login();
  }

  yield();

  if (set_time)
    panel_set_time();

  yield();
}

void flush_serial_buffer()
{
  Serial.flush();
  while (Serial.read() >= 0)
  {
    yield();
  }
}